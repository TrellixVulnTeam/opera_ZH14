// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_OUTPUT_DIRECT_RENDERER_H_
#define CC_OUTPUT_DIRECT_RENDERER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "cc/base/cc_export.h"
#include "cc/output/renderer.h"
#include "cc/resources/resource_provider.h"
#include "cc/resources/scoped_resource.h"

namespace cc {

class ResourceProvider;

// This is the base class for code shared between the GL and software
// renderer implementations.  "Direct" refers to the fact that it does not
// delegate rendering to another compositor.
class CC_EXPORT DirectRenderer : public Renderer {
 public:
  virtual ~DirectRenderer();

  ResourceProvider* resource_provider() const { return resource_provider_; }

  virtual bool CanReadPixels() const OVERRIDE;
  virtual void DecideRenderPassAllocationsForFrame(
      const RenderPassList& render_passes_in_draw_order) OVERRIDE;
  virtual bool HaveCachedResourcesForRenderPassId(RenderPass::Id id) const
      OVERRIDE;
  virtual void DrawFrame(RenderPassList* render_passes_in_draw_order) OVERRIDE;

  struct CC_EXPORT DrawingFrame {
    DrawingFrame();
    ~DrawingFrame();

    const RenderPass* root_render_pass;
    const RenderPass* current_render_pass;
    const ScopedResource* current_texture;

    gfx::RectF root_damage_rect;

    int top_border;

    gfx::Transform projection_matrix;
    gfx::Transform window_matrix;
  };

  void SetEnlargePassTextureAmountForTesting(gfx::Vector2d amount);

 protected:
  DirectRenderer(RendererClient* client,
                 OutputSurface* output_surface,
                 ResourceProvider* resource_provider);

  class CachedResource : public ScopedResource {
   public:
    static scoped_ptr<CachedResource> Create(
        ResourceProvider* resource_provider) {
      return make_scoped_ptr(new CachedResource(resource_provider));
    }
    virtual ~CachedResource() {}

    bool is_complete() const { return is_complete_; }
    void set_is_complete(bool is_complete) { is_complete_ = is_complete; }

   protected:
    explicit CachedResource(ResourceProvider* resource_provider)
        : ScopedResource(resource_provider),
          is_complete_(false) {}

   private:
    bool is_complete_;

    DISALLOW_COPY_AND_ASSIGN(CachedResource);
  };

  static gfx::RectF QuadVertexRect();
  static void QuadRectTransform(gfx::Transform* quad_rect_transform,
                                const gfx::Transform& quad_transform,
                                const gfx::RectF& quad_rect);
  void InitializeViewport(DrawingFrame* frame,
                          gfx::Rect draw_rect,
                          gfx::Rect viewport_rect,
                          gfx::Size surface_size);
  gfx::Rect MoveFromDrawToWindowSpace(const gfx::RectF& draw_rect) const;

  static gfx::RectF ComputeScissorRectForRenderPass(const DrawingFrame* frame);
  void SetScissorStateForQuad(const DrawingFrame* frame, const DrawQuad& quad);
  void SetScissorStateForQuadWithRenderPassScissor(
      const DrawingFrame* frame,
      const DrawQuad& quad,
      const gfx::RectF& render_pass_scissor,
      bool* should_skip_quad);

  static gfx::Size RenderPassTextureSize(const RenderPass* render_pass);
  static GLenum RenderPassTextureFormat(const RenderPass* render_pass);

  void DrawRenderPass(DrawingFrame* frame, const RenderPass* render_pass);
  bool UseRenderPass(DrawingFrame* frame, const RenderPass* render_pass);

  virtual void BindFramebufferToOutputSurface(DrawingFrame* frame) = 0;
  virtual bool BindFramebufferToTexture(DrawingFrame* frame,
                                        const ScopedResource* resource,
                                        gfx::Rect target_rect) = 0;
  virtual void ClearTop(gfx::Size viewport_size, int border) = 0;
  virtual void SetDrawViewport(gfx::Rect window_space_viewport) = 0;
  virtual void SetScissorTestRect(gfx::Rect scissor_rect) = 0;
  virtual void ClearFramebuffer(DrawingFrame* frame) = 0;
  virtual void DoDrawQuad(DrawingFrame* frame, const DrawQuad* quad) = 0;
  virtual void BeginDrawingFrame(DrawingFrame* frame) = 0;
  virtual void FinishDrawingFrame(DrawingFrame* frame) = 0;
  virtual void FinishDrawingQuadList();
  virtual bool FlippedFramebuffer() const = 0;
  virtual void EnsureScissorTestEnabled() = 0;
  virtual void EnsureScissorTestDisabled() = 0;

  virtual void CopyCurrentRenderPassToBitmap(
      DrawingFrame* frame,
      scoped_ptr<CopyOutputRequest> request) = 0;

  ScopedPtrHashMap<RenderPass::Id, CachedResource> render_pass_textures_;
  OutputSurface* output_surface_;
  ResourceProvider* resource_provider_;

  // For use in coordinate conversion, this stores the output rect, viewport
  // rect (= unflipped version of glViewport rect), and the size of target
  // framebuffer. During a draw, this stores the values for the current render
  // pass; in between draws, they retain the values for the root render pass of
  // the last draw.
  gfx::Rect current_draw_rect_;
  gfx::Rect current_viewport_rect_;
  gfx::Size current_surface_size_;

 private:
  gfx::Vector2d enlarge_pass_texture_amount_;

  DISALLOW_COPY_AND_ASSIGN(DirectRenderer);
};

}  // namespace cc

#endif  // CC_OUTPUT_DIRECT_RENDERER_H_
