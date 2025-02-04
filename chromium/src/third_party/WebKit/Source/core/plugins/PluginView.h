/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PluginView_h
#define PluginView_h

#include "core/platform/ScrollTypes.h"
#include "core/platform/Widget.h"
#include <wtf/text/WTFString.h>

struct NPObject;

namespace WebKit { class WebLayer; }

namespace WebCore {

class Scrollbar;

class PluginView : public Widget {
public:
    virtual bool isPluginView() const { return true; }

    virtual WebKit::WebLayer* platformLayer() const { return 0; }
    virtual NPObject* scriptableObject() { return 0; }
    virtual bool getFormValue(String&) { return false; }
    virtual bool wantsWheelEvents() { return false; }
    virtual bool supportsKeyboardFocus() const { return false; }
    virtual bool canProcessDrag() const { return false; }

protected:
    PluginView() : Widget() { }
};

inline PluginView* toPluginView(Widget* widget)
{
    ASSERT(!widget || widget->isPluginView());
    return static_cast<PluginView*>(widget);
}

inline const PluginView* toPluginView(const Widget* widget)
{
    ASSERT(!widget || widget->isPluginView());
    return static_cast<const PluginView*>(widget);
}

// This will catch anyone doing an unnecessary cast.
void toPluginView(const PluginView*);

} // namespace WebCore

#endif // PluginView_h
