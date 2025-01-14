/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include "V8WorkerContext.h"

#include "bindings/v8/ScheduledAction.h"
#include "bindings/v8/V8Binding.h"
#include "bindings/v8/V8Utilities.h"
#include "bindings/v8/V8WorkerContextEventListener.h"
#include "bindings/v8/WorkerScriptController.h"
#include "core/dom/ExceptionCode.h"
#include "core/inspector/ScriptCallStack.h"
#include "core/page/ContentSecurityPolicy.h"
#include "core/page/DOMTimer.h"
#include "core/workers/WorkerContext.h"
#include "modules/websockets/WebSocket.h"

namespace WebCore {

void SetTimeoutOrInterval(const v8::FunctionCallbackInfo<v8::Value>& args, bool singleShot)
{
    WorkerContext* workerContext = V8WorkerContext::toNative(args.Holder());

    int argumentCount = args.Length();
    if (argumentCount < 1)
        return;

    v8::Handle<v8::Value> function = args[0];
    int32_t timeout = argumentCount >= 2 ? args[1]->Int32Value() : 0;
    int timerId;

    WorkerScriptController* script = workerContext->script();
    if (!script)
        return;

    v8::Handle<v8::Context> v8Context = script->context();
    if (function->IsString()) {
        if (ContentSecurityPolicy* policy = workerContext->contentSecurityPolicy()) {
            if (!policy->allowEval()) {
                v8SetReturnValue(args, 0);
                return;
            }
        }
        WTF::String stringFunction = toWebCoreString(function);
        timerId = DOMTimer::install(workerContext, adoptPtr(new ScheduledAction(v8Context, stringFunction, workerContext->url(), args.GetIsolate())), timeout, singleShot);
    } else if (function->IsFunction()) {
        size_t paramCount = argumentCount >= 2 ? argumentCount - 2 : 0;
        v8::Local<v8::Value>* params = 0;
        if (paramCount > 0) {
            params = new v8::Local<v8::Value>[paramCount];
            for (size_t i = 0; i < paramCount; ++i)
                params[i] = args[i+2];
        }
        // ScheduledAction takes ownership of actual params and releases them in its destructor.
        OwnPtr<ScheduledAction> action = adoptPtr(new ScheduledAction(v8Context, v8::Handle<v8::Function>::Cast(function), paramCount, params, args.GetIsolate()));
        // FIXME: We should use a OwnArrayPtr for params.
        delete [] params;
        timerId = DOMTimer::install(workerContext, action.release(), timeout, singleShot);
    } else
        return;

    v8SetReturnValue(args, timerId);
}

void V8WorkerContext::importScriptsMethodCustom(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (!args.Length())
        return;

    Vector<String> urls;
    for (int i = 0; i < args.Length(); i++) {
        V8TRYCATCH_VOID(v8::Handle<v8::String>, scriptUrl, args[i]->ToString());
        if (scriptUrl.IsEmpty())
            return;
        urls.append(toWebCoreString(scriptUrl));
    }

    WorkerContext* workerContext = V8WorkerContext::toNative(args.Holder());

    ExceptionCode ec = 0;
    workerContext->importScripts(urls, ec);

    if (!ec)
        return;
    setDOMException(ec, args.GetIsolate());
}

void V8WorkerContext::setTimeoutMethodCustom(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    return SetTimeoutOrInterval(args, true);
}

void V8WorkerContext::setIntervalMethodCustom(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    return SetTimeoutOrInterval(args, false);
}

v8::Handle<v8::Value> toV8(WorkerContext* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    // Notice that we explicitly ignore creationContext because the WorkerContext is its own creationContext.

    if (!impl)
        return v8NullWithCheck(isolate);

    WorkerScriptController* script = impl->script();
    if (!script)
        return v8NullWithCheck(isolate);

    v8::Handle<v8::Object> global = script->context()->Global();
    ASSERT(!global.IsEmpty());
    return global;
}

v8::Handle<v8::Value> toV8ForMainWorld(WorkerContext* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    return toV8(impl, creationContext, isolate);
}

} // namespace WebCore
