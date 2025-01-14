/*
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
 * Copyright (C) 2013 Google Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef ContextLifecycleNotifier_h
#define ContextLifecycleNotifier_h

#include "core/dom/ActiveDOMObject.h"
#include "wtf/HashSet.h"
#include "wtf/PassOwnPtr.h"

namespace WTF {
class MemoryObjectInfo;
}

namespace WebCore {

class ActiveDOMObject;
class ContextLifecycleObserver;
class ScriptExecutionContext;

class ContextLifecycleNotifier {
public:
    static PassOwnPtr<ContextLifecycleNotifier> create(ScriptExecutionContext*);

    virtual ~ContextLifecycleNotifier();

    typedef HashSet<ContextLifecycleObserver*> ContextObserverSet;
    typedef HashSet<ActiveDOMObject*> ActiveDOMObjectSet;

    const ActiveDOMObjectSet& activeDOMObjects() const { return m_activeDOMObjects; }

    virtual void addObserver(ContextLifecycleObserver*, ContextLifecycleObserver::Type as);
    virtual void removeObserver(ContextLifecycleObserver*, ContextLifecycleObserver::Type as);

    void notifyResumingActiveDOMObjects();
    void notifySuspendingActiveDOMObjects(ActiveDOMObject::ReasonForSuspension);
    void notifyStoppingActiveDOMObjects();

    bool contains(ActiveDOMObject* object) const { return m_activeDOMObjects.contains(object); }
    bool canSuspendActiveDOMObjects();
    bool hasPendingActivity() const;

    void reportMemoryUsage(WTF::MemoryObjectInfo*) const;

protected:
    explicit ContextLifecycleNotifier(ScriptExecutionContext*);

    enum IterationType {
        IteratingNone,
        IteratingOverActiveDOMObjects,
        IteratingOverContextObservers,
        IteratingOverDocumentObservers
    };

    IterationType m_iterating;

private:
    ScriptExecutionContext* m_context;
    ContextObserverSet m_contextObservers;
    ActiveDOMObjectSet m_activeDOMObjects;
    bool m_inDestructor;
};

inline PassOwnPtr<ContextLifecycleNotifier> ContextLifecycleNotifier::create(ScriptExecutionContext* context)
{
    return adoptPtr(new ContextLifecycleNotifier(context));
}

} // namespace WebCore

#endif // ContextLifecycleNotifier_h

