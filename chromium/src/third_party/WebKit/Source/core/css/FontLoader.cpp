/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

#include "config.h"
#include "core/css/FontLoader.h"

#include "bindings/v8/Dictionary.h"
#include "core/css/CSSFontFaceLoadEvent.h"
#include "core/css/CSSFontFaceSource.h"
#include "core/css/CSSFontSelector.h"
#include "core/css/CSSParser.h"
#include "core/css/CSSSegmentedFontFace.h"
#include "core/css/StylePropertySet.h"
#include "core/css/resolver/StyleResolver.h"
#include "core/dom/Document.h"
#include "core/page/FrameView.h"
#include "core/platform/HistogramSupport.h"

namespace WebCore {

static const int defaultFontSize = 10;
static const char* const defaultFontFamily = "sans-serif";

class LoadFontCallback : public CSSSegmentedFontFace::LoadFontCallback {
public:
    static PassRefPtr<LoadFontCallback> create(int numLoading, PassRefPtr<VoidCallback> loadCallback, PassRefPtr<VoidCallback> errorCallback)
    {
        return adoptRef<LoadFontCallback>(new LoadFontCallback(numLoading, loadCallback, errorCallback));
    }

    static PassRefPtr<LoadFontCallback> createFromParams(const Dictionary& params, const FontFamily& family)
    {
        RefPtr<VoidCallback> onsuccess;
        RefPtr<VoidCallback> onerror;
        params.get("onsuccess", onsuccess);
        params.get("onerror", onerror);
        if (!onsuccess && !onerror)
            return 0;
        int numFamilies = 0;
        for (const FontFamily* f = &family; f; f = f->next())
            numFamilies++;
        return LoadFontCallback::create(numFamilies, onsuccess, onerror);
    }

    virtual void notifyLoaded(CSSSegmentedFontFace*) OVERRIDE;
    virtual void notifyError(CSSSegmentedFontFace*) OVERRIDE;
    void loaded(Document*);
    void error(Document*);
private:
    LoadFontCallback(int numLoading, PassRefPtr<VoidCallback> loadCallback, PassRefPtr<VoidCallback> errorCallback)
        : m_numLoading(numLoading)
        , m_errorOccured(false)
        , m_loadCallback(loadCallback)
        , m_errorCallback(errorCallback)
    { }

    int m_numLoading;
    bool m_errorOccured;
    RefPtr<VoidCallback> m_loadCallback;
    RefPtr<VoidCallback> m_errorCallback;
};

void LoadFontCallback::loaded(Document* document)
{
    m_numLoading--;
    if (m_numLoading || !document)
        return;

    if (m_errorOccured) {
        if (m_errorCallback)
            document->fontloader()->scheduleCallback(m_errorCallback.release());
    } else {
        if (m_loadCallback)
            document->fontloader()->scheduleCallback(m_loadCallback.release());
    }
}

void LoadFontCallback::error(Document* document)
{
    m_errorOccured = true;
    loaded(document);
}

void LoadFontCallback::notifyLoaded(CSSSegmentedFontFace* face)
{
    loaded(face->fontSelector()->document());
}

void LoadFontCallback::notifyError(CSSSegmentedFontFace* face)
{
    error(face->fontSelector()->document());
}

FontLoader::FontLoader(Document* document)
    : ActiveDOMObject(document)
    , m_document(document)
    , m_loadingCount(0)
    , m_timer(this, &FontLoader::timerFired)
{
    suspendIfNeeded();
}

FontLoader::~FontLoader()
{
}

EventTargetData* FontLoader::eventTargetData()
{
    return &m_eventTargetData;
}

EventTargetData* FontLoader::ensureEventTargetData()
{
    return &m_eventTargetData;
}

const AtomicString& FontLoader::interfaceName() const
{
    return eventNames().interfaceForFontLoader;
}

ScriptExecutionContext* FontLoader::scriptExecutionContext() const
{
    return ActiveDOMObject::scriptExecutionContext();
}

void FontLoader::didLayout()
{
    if (m_document->page() && m_document->page()->mainFrame() == m_document->frame())
        m_histogram.record();
    if (!RuntimeEnabledFeatures::fontLoadEventsEnabled())
        return;
    if (m_loadingCount || (!m_pendingDoneEvent && m_fontsReadyCallbacks.isEmpty()))
        return;
    if (!m_timer.isActive())
        m_timer.startOneShot(0);
}

void FontLoader::timerFired(Timer<FontLoader>*)
{
    firePendingEvents();
    firePendingCallbacks();
    fireDoneEventIfPossible();
}

void FontLoader::scheduleEvent(PassRefPtr<Event> event)
{
    m_pendingEvents.append(event);
    if (!m_timer.isActive())
        m_timer.startOneShot(0);
}

void FontLoader::firePendingEvents()
{
    if (m_pendingEvents.isEmpty())
        return;

    Vector<RefPtr<Event> > pendingEvents;
    m_pendingEvents.swap(pendingEvents);
    for (size_t index = 0; index < pendingEvents.size(); ++index)
        dispatchEvent(pendingEvents[index].release());
}

void FontLoader::scheduleCallback(PassRefPtr<VoidCallback> callback)
{
    m_pendingCallbacks.append(callback);
    if (!m_timer.isActive())
        m_timer.startOneShot(0);
}

void FontLoader::firePendingCallbacks()
{
    if (m_pendingCallbacks.isEmpty())
        return;

    Vector<RefPtr<VoidCallback> > pendingCallbacks;
    m_pendingCallbacks.swap(pendingCallbacks);
    for (size_t index = 0; index < pendingCallbacks.size(); ++index)
        pendingCallbacks[index]->handleEvent();
}

void FontLoader::beginFontLoading(CSSFontFaceRule* rule)
{
    m_histogram.incrementCount();
    if (!RuntimeEnabledFeatures::fontLoadEventsEnabled())
        return;

    ++m_loadingCount;
    if (m_loadingCount == 1 && !m_pendingDoneEvent)
        scheduleEvent(CSSFontFaceLoadEvent::createForFontFaceRule(eventNames().loadingEvent, rule));
    scheduleEvent(CSSFontFaceLoadEvent::createForFontFaceRule(eventNames().loadstartEvent, rule));
    m_pendingDoneEvent.clear();
}

void FontLoader::fontLoaded(CSSFontFaceRule* rule)
{
    if (!RuntimeEnabledFeatures::fontLoadEventsEnabled())
        return;
    scheduleEvent(CSSFontFaceLoadEvent::createForFontFaceRule(eventNames().loadEvent, rule));
    queueDoneEvent(rule);
}

void FontLoader::loadError(CSSFontFaceRule* rule, CSSFontFaceSource* source)
{
    if (!RuntimeEnabledFeatures::fontLoadEventsEnabled())
        return;
    // FIXME: We should report NetworkError in case of timeout, etc.
    String errorName = (source && source->isDecodeError()) ? "InvalidFontDataError" : ExceptionCodeDescription(NOT_FOUND_ERR).name;
    scheduleEvent(CSSFontFaceLoadEvent::createForError(rule, DOMError::create(errorName)));
    queueDoneEvent(rule);
}

void FontLoader::queueDoneEvent(CSSFontFaceRule* rule)
{
    ASSERT(m_loadingCount > 0);
    --m_loadingCount;
    if (!m_loadingCount) {
        ASSERT(!m_pendingDoneEvent);
        m_pendingDoneEvent = CSSFontFaceLoadEvent::createForFontFaceRule(eventNames().loadingdoneEvent, rule);
    }
}

void FontLoader::notifyWhenFontsReady(PassRefPtr<VoidCallback> callback)
{
    m_fontsReadyCallbacks.append(callback);
    if (!m_timer.isActive())
        m_timer.startOneShot(0);
}

void FontLoader::fireDoneEventIfPossible()
{
    if (!m_pendingEvents.isEmpty() || !m_pendingCallbacks.isEmpty())
        return;
    if (m_loadingCount || (!m_pendingDoneEvent && m_fontsReadyCallbacks.isEmpty()))
        return;

    if (FrameView* view = m_document->view()) {
        if (view->needsLayout())
            return;
        m_document->updateStyleIfNeeded();
        if (view->needsLayout())
            return;
    }

    if (m_pendingDoneEvent)
        dispatchEvent(m_pendingDoneEvent.release());

    if (!m_fontsReadyCallbacks.isEmpty()) {
        Vector<RefPtr<VoidCallback> > callbacks;
        m_fontsReadyCallbacks.swap(callbacks);
        for (size_t index = 0; index < callbacks.size(); ++index)
            callbacks[index]->handleEvent();
    }
}

void FontLoader::loadFont(const Dictionary& params)
{
    // FIXME: The text member of params is ignored.
    String fontString;
    if (!params.get("font", fontString))
        return;
    Font font;
    if (!resolveFontStyle(fontString, font))
        return;
    RefPtr<LoadFontCallback> callback = LoadFontCallback::createFromParams(params, font.family());

    for (const FontFamily* f = &font.family(); f; f = f->next()) {
        CSSSegmentedFontFace* face = m_document->styleResolver()->fontSelector()->getFontFace(font.fontDescription(), f->family());
        if (!face) {
            if (callback)
                callback->error(m_document);
            continue;
        }
        face->loadFont(font.fontDescription(), callback);
    }
}

bool FontLoader::checkFont(const String& fontString, const String&)
{
    // FIXME: The second parameter (text) is ignored.
    Font font;
    if (!resolveFontStyle(fontString, font))
        return false;
    for (const FontFamily* f = &font.family(); f; f = f->next()) {
        CSSSegmentedFontFace* face = m_document->styleResolver()->fontSelector()->getFontFace(font.fontDescription(), f->family());
        if (!face || !face->checkFont())
            return false;
    }
    return true;
}

static void applyPropertyToCurrentStyle(StyleResolver* styleResolver, CSSPropertyID id, const RefPtr<StylePropertySet>& parsedStyle)
{
    styleResolver->applyPropertyToCurrentStyle(id, parsedStyle->getPropertyCSSValue(id).get());
}

bool FontLoader::resolveFontStyle(const String& fontString, Font& font)
{
    // Interpret fontString in the same way as the 'font' attribute of CanvasRenderingContext2D.
    RefPtr<MutableStylePropertySet> parsedStyle = MutableStylePropertySet::create();
    CSSParser::parseValue(parsedStyle.get(), CSSPropertyFont, fontString, true, CSSStrictMode, 0);
    if (parsedStyle->isEmpty())
        return false;

    String fontValue = parsedStyle->getPropertyValue(CSSPropertyFont);
    if (fontValue == "inherit" || fontValue == "initial")
        return false;

    RefPtr<RenderStyle> style = RenderStyle::create();

    FontFamily fontFamily;
    fontFamily.setFamily(defaultFontFamily);

    FontDescription defaultFontDescription;
    defaultFontDescription.setFamily(fontFamily);
    defaultFontDescription.setSpecifiedSize(defaultFontSize);
    defaultFontDescription.setComputedSize(defaultFontSize);

    style->setFontDescription(defaultFontDescription);

    style->font().update(style->font().fontSelector());

    // Now map the font property longhands into the style.
    StyleResolver* styleResolver = m_document->styleResolver();
    styleResolver->applyPropertyToStyle(CSSPropertyFontFamily, parsedStyle->getPropertyCSSValue(CSSPropertyFontFamily).get(), style.get());
    applyPropertyToCurrentStyle(styleResolver, CSSPropertyFontStyle, parsedStyle);
    applyPropertyToCurrentStyle(styleResolver, CSSPropertyFontVariant, parsedStyle);
    applyPropertyToCurrentStyle(styleResolver, CSSPropertyFontWeight, parsedStyle);

    // As described in BUG66291, setting font-size and line-height on a font may entail a CSSPrimitiveValue::computeLengthDouble call,
    // which assumes the fontMetrics are available for the affected font, otherwise a crash occurs (see http://trac.webkit.org/changeset/96122).
    // The updateFont() calls below update the fontMetrics and ensure the proper setting of font-size and line-height.
    styleResolver->updateFont();
    applyPropertyToCurrentStyle(styleResolver, CSSPropertyFontSize, parsedStyle);
    styleResolver->updateFont();
    applyPropertyToCurrentStyle(styleResolver, CSSPropertyLineHeight, parsedStyle);

    font = style->font();
    font.update(styleResolver->fontSelector());
    return true;
}

void FontLoader::FontLoadHistogram::record()
{
    if (m_recorded)
        return;
    m_recorded = true;
    HistogramSupport::histogramCustomCounts("WebFont.WebFontsInPage", m_count, 1, 100, 50);
}

} // namespace WebCore
