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
#include "V8MessageEvent.h"

#include "bindings/v8/SerializedScriptValue.h"
#include "core/dom/MessageEvent.h"

#include "V8ArrayBuffer.h"
#include "V8Blob.h"
#include "V8MessagePort.h"
#include "V8Window.h"
#include "bindings/v8/V8Binding.h"

namespace WebCore {

void V8MessageEvent::dataAttrGetterCustom(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    MessageEvent* event = V8MessageEvent::toNative(info.Holder());

    v8::Handle<v8::Value> result;
    switch (event->dataType()) {
    case MessageEvent::DataTypeScriptValue: {
        ScriptValue scriptValue = event->dataAsScriptValue();
        if (scriptValue.hasNoValue())
            result = v8Null(info.GetIsolate());
        else
            result = scriptValue.v8Value();
        break;
    }

    case MessageEvent::DataTypeSerializedScriptValue:
        if (RefPtr<SerializedScriptValue> serializedValue = event->dataAsSerializedScriptValue()) {
            MessagePortArray ports = event->ports();
            result = serializedValue->deserialize(info.GetIsolate(), &ports);
        } else
            result = v8Null(info.GetIsolate());
        break;

    case MessageEvent::DataTypeString: {
        String stringValue = event->dataAsString();
        result = v8String(stringValue, info.GetIsolate());
        break;
    }

    case MessageEvent::DataTypeBlob:
        result = toV8Fast(event->dataAsBlob(), info, event);
        break;

    case MessageEvent::DataTypeArrayBuffer:
        result = toV8Fast(event->dataAsArrayBuffer(), info, event);
        break;
    }

    // Overwrite the data attribute so it returns the cached result in future invocations.
    // This custom handler (dataAccessGetter) will not be called again.
    v8::PropertyAttribute dataAttr = static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::ReadOnly);
    info.Holder()->ForceSet(name, result, dataAttr);
    v8SetReturnValue(info, result);
}

void V8MessageEvent::initMessageEventMethodCustom(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    MessageEvent* event = V8MessageEvent::toNative(args.Holder());
    String typeArg = toWebCoreString(args[0]);
    bool canBubbleArg = args[1]->BooleanValue();
    bool cancelableArg = args[2]->BooleanValue();
    ScriptValue dataArg = ScriptValue(args[3]);
    String originArg = toWebCoreString(args[4]);
    String lastEventIdArg = toWebCoreString(args[5]);

    DOMWindow* sourceArg = 0;
    if (args[6]->IsObject()) {
        v8::Handle<v8::Object> wrapper = v8::Handle<v8::Object>::Cast(args[6]);
        v8::Handle<v8::Object> window = wrapper->FindInstanceInPrototypeChain(V8Window::GetTemplate(args.GetIsolate(), worldTypeInMainThread(args.GetIsolate())));
        if (!window.IsEmpty())
            sourceArg = V8Window::toNative(window);
    }
    OwnPtr<MessagePortArray> portArray;

    if (!isUndefinedOrNull(args[7])) {
        portArray = adoptPtr(new MessagePortArray);
        if (!getMessagePortArray(args[7], *portArray, args.GetIsolate()))
            return;
    }
    event->initMessageEvent(typeArg, canBubbleArg, cancelableArg, dataArg, originArg, lastEventIdArg, sourceArg, portArray.release());
}

void V8MessageEvent::webkitInitMessageEventMethodCustom(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    initMessageEventMethodCustom(args);
}


} // namespace WebCore
