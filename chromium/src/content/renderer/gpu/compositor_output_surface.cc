// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/renderer/gpu/compositor_output_surface.h"

#include "base/command_line.h"
#include "base/message_loop/message_loop_proxy.h"
#include "cc/output/compositor_frame.h"
#include "cc/output/compositor_frame_ack.h"
#include "cc/output/output_surface_client.h"
#include "content/common/gpu/client/command_buffer_proxy_impl.h"
#include "content/common/gpu/client/webgraphicscontext3d_command_buffer_impl.h"
#include "content/common/view_messages.h"
#include "content/public/common/content_switches.h"
#include "content/renderer/render_thread_impl.h"
#include "ipc/ipc_forwarding_message_filter.h"
#include "ipc/ipc_sync_channel.h"
#include "third_party/WebKit/public/platform/WebGraphicsContext3D.h"

using WebKit::WebGraphicsContext3D;

namespace {
// There are several compositor surfaces in a process, but they share the same
// compositor thread, so we use a simple int here to track prefer-smoothness.
int g_prefer_smoothness_count = 0;
} // namespace

namespace content {

//------------------------------------------------------------------------------

// static
IPC::ForwardingMessageFilter* CompositorOutputSurface::CreateFilter(
    base::TaskRunner* target_task_runner)
{
  uint32 messages_to_filter[] = {
    ViewMsg_UpdateVSyncParameters::ID,
    ViewMsg_SwapCompositorFrameAck::ID,
    ViewMsg_RequestBitmap::ID,
    ViewMsg_ActivateOutputSurface::ID,
#if defined(OS_ANDROID)
    ViewMsg_BeginFrame::ID
#endif
  };

  return new IPC::ForwardingMessageFilter(
      messages_to_filter, arraysize(messages_to_filter),
      target_task_runner);
}

CompositorOutputSurface::CompositorOutputSurface(
    int32 routing_id,
    WebGraphicsContext3DCommandBufferImpl* context3D,
    cc::SoftwareOutputDevice* software_device,
    bool use_swap_compositor_frame_message)
    : OutputSurface(scoped_ptr<WebKit::WebGraphicsContext3D>(context3D),
                    make_scoped_ptr(software_device)),
      use_swap_compositor_frame_message_(use_swap_compositor_frame_message),
      output_surface_filter_(
          RenderThreadImpl::current()->compositor_output_surface_filter()),
      routing_id_(routing_id),
      prefers_smoothness_(false),
      main_thread_handle_(base::PlatformThread::CurrentHandle()) {
  DCHECK(output_surface_filter_.get());
  DetachFromThread();
  message_sender_ = RenderThreadImpl::current()->sync_message_filter();
  DCHECK(message_sender_.get());
}

CompositorOutputSurface::~CompositorOutputSurface() {
  DCHECK(CalledOnValidThread());
  SetNeedsBeginFrame(false);
  if (!HasClient())
    return;
  UpdateSmoothnessTakesPriority(false);
  if (output_surface_proxy_.get())
    output_surface_proxy_->ClearOutputSurface();
  output_surface_filter_->RemoveRoute(routing_id_);
}

bool CompositorOutputSurface::BindToClient(
    cc::OutputSurfaceClient* client) {
  DCHECK(CalledOnValidThread());

  if (!cc::OutputSurface::BindToClient(client))
    return false;

  output_surface_proxy_ = new CompositorOutputSurfaceProxy(this);
  output_surface_filter_->AddRoute(
      routing_id_,
      base::Bind(&CompositorOutputSurfaceProxy::OnMessageReceived,
                 output_surface_proxy_));

  Send(new ViewHostMsg_OutputSurfaceBound(routing_id_));

  return true;
}

void CompositorOutputSurface::SwapBuffers(cc::CompositorFrame* frame) {
  if (use_swap_compositor_frame_message_) {
    Send(new ViewHostMsg_SwapCompositorFrame(routing_id_, *frame));
    DidSwapBuffers();
    return;
  }

  if (frame->gl_frame_data) {
    WebGraphicsContext3DCommandBufferImpl* command_buffer =
        static_cast<WebGraphicsContext3DCommandBufferImpl*>(context3d());
    CommandBufferProxyImpl* command_buffer_proxy =
        command_buffer->GetCommandBufferProxy();
    DCHECK(command_buffer_proxy);
    context3d()->shallowFlushCHROMIUM();
    command_buffer_proxy->SetLatencyInfo(frame->metadata.latency_info);
  }

  OutputSurface::SwapBuffers(frame);
}

void CompositorOutputSurface::OnMessageReceived(const IPC::Message& message) {
  DCHECK(CalledOnValidThread());
  if (!HasClient())
    return;
  IPC_BEGIN_MESSAGE_MAP(CompositorOutputSurface, message)
    IPC_MESSAGE_HANDLER(ViewMsg_UpdateVSyncParameters, OnUpdateVSyncParameters);
    IPC_MESSAGE_HANDLER(ViewMsg_SwapCompositorFrameAck, OnSwapAck);
    IPC_MESSAGE_HANDLER(ViewMsg_RequestBitmap, OnRequestBitmap);
    IPC_MESSAGE_HANDLER(ViewMsg_ActivateOutputSurface, OnActivate)
#if defined(OS_ANDROID)
    IPC_MESSAGE_HANDLER(ViewMsg_BeginFrame, OnBeginFrame);
#endif
  IPC_END_MESSAGE_MAP()
}

void CompositorOutputSurface::OnUpdateVSyncParameters(
    base::TimeTicks timebase, base::TimeDelta interval) {
  DCHECK(CalledOnValidThread());
  OnVSyncParametersChanged(timebase, interval);
}

#if defined(OS_ANDROID)
void CompositorOutputSurface::SetNeedsBeginFrame(bool enable) {
  DCHECK(CalledOnValidThread());
  if (needs_begin_frame_ != enable)
    Send(new ViewHostMsg_SetNeedsBeginFrame(routing_id_, enable));
  OutputSurface::SetNeedsBeginFrame(enable);
}

void CompositorOutputSurface::OnBeginFrame(const cc::BeginFrameArgs& args) {
  DCHECK(CalledOnValidThread());
  BeginFrame(args);
}
#endif  // defined(OS_ANDROID)

void CompositorOutputSurface::OnSwapAck(const cc::CompositorFrameAck& ack) {
  OnSwapBuffersComplete(&ack);
}

void CompositorOutputSurface::OnRequestBitmap(int32 request_id,
                                              const gfx::Size& size,
                                              bool wait_for_frame) {
  WebGraphicsContext3DCommandBufferImpl* context =
      reinterpret_cast<WebGraphicsContext3DCommandBufferImpl*>(context3d_.get());
  context->GetCommandBufferProxy()->RequestBitmap(
      request_id, size, wait_for_frame);
}

void CompositorOutputSurface::OnActivate() {
  if (context3d_.get())
    context3d_->activateOutputSurface();
}

bool CompositorOutputSurface::Send(IPC::Message* message) {
  return message_sender_->Send(message);
}

namespace {
#if defined(OS_ANDROID)
  void SetThreadPriorityToIdle(base::PlatformThreadHandle handle) {
    base::PlatformThread::SetThreadPriority(
       handle, base::kThreadPriority_Background);
  }
  void SetThreadPriorityToDefault(base::PlatformThreadHandle handle) {
    base::PlatformThread::SetThreadPriority(
       handle, base::kThreadPriority_Normal);
  }
#else
  void SetThreadPriorityToIdle(base::PlatformThreadHandle handle) {}
  void SetThreadPriorityToDefault(base::PlatformThreadHandle handle) {}
#endif
}

void CompositorOutputSurface::UpdateSmoothnessTakesPriority(
    bool prefers_smoothness) {
#ifndef NDEBUG
  // If we use different compositor threads, we need to
  // use an atomic int to track prefer smoothness count.
  base::PlatformThreadId g_last_thread = base::PlatformThread::CurrentId();
  DCHECK_EQ(g_last_thread, base::PlatformThread::CurrentId());
#endif
  if (prefers_smoothness_ == prefers_smoothness)
    return;
  // If this is the first surface to start preferring smoothness,
  // Throttle the main thread's priority.
  if (prefers_smoothness_ == false &&
      ++g_prefer_smoothness_count == 1) {
    SetThreadPriorityToIdle(main_thread_handle_);
  }
  // If this is the last surface to stop preferring smoothness,
  // Reset the main thread's priority to the default.
  if (prefers_smoothness_ == true &&
      --g_prefer_smoothness_count == 0) {
    SetThreadPriorityToDefault(main_thread_handle_);
  }
  prefers_smoothness_ = prefers_smoothness;
}

}  // namespace content
