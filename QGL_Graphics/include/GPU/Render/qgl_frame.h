#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Interfaces/qgl_igraphics_device.h"
#include "include/qgl_window.h"
#include "include/GPU/Descriptors/qgl_rtv_descriptor_heap.h"
#include "include/GPU/Descriptors/qgl_dsv_descriptor_heap.h"

#include "include/GPU/Render/qgl_render_target.h"
#include "include/GPU/Render/qgl_depth_stencil.h"
#include "include/GPU/Render/qgl_viewport.h"
#include "include/GPU/Render/qgl_scissor.h"
#include "include/GPU/Render/qgl_rasterizer.h"
#include "include/GPU/Render/qgl_blender.h"

namespace qgl::graphics::gpu::render
{
   class iframe : iqgl
   {
      public:
      /*
       Returns a pointer to the render target.
       */
      virtual render_target* frame_buffer() noexcept = 0;

      virtual const render_target* const_frame_buffer() const noexcept = 0;

      /*
       Returns a pointer to the stencil.
       */
      virtual depth_stencil* frame_stencil() noexcept = 0;

      virtual const depth_stencil* const_frame_stencil() const noexcept = 0;

      /*
       Returns a pointer to the viewport.
       The viewport defines the space of the render target that will be
       presented.
       */
      virtual viewport* frame_viewport() noexcept = 0;

      virtual const viewport* const_frame_viewport() const noexcept = 0;

      /*
       Returns a pointer to the scissor.
       By default, the scissor uses the same area as the viewport.
       */
      virtual scissor* frame_scissor() noexcept = 0;

      virtual const scissor* const_frame_scissor() const noexcept = 0;

      virtual rasterizer* frame_rasterizer() noexcept = 0;

      virtual const rasterizer* const_frame_rasterizer() const noexcept = 0;

      virtual blender* frame_blender() noexcept = 0;

      virtual const blender* const_frame_blender() const noexcept = 0;

      /*
       Acquires the necessary resources so rendering operations can be queued
       to this render. Engines must call this before recording render commands.
       */
      virtual void acquire(static_ptr_ref<igraphics_device> dev_p) = 0;

      /*
       Releases the render's resources so it can be presented.
       Engines must call release before flushing the D3D11On12 context and
       presenting the render on the swap chain.
       */
      virtual void release(static_ptr_ref<igraphics_device> dev_p) = 0;

      /*
       Disposes of render resources so the render can be rebuilt.
       */
      virtual void dispose(static_ptr_ref<igraphics_device> dev_p) = 0;
   };

   struct IFRAME_CREATION_PARAMS
   {
      static_ptr_ref<gpu::dsv_descriptor_heap> DSVHeap;
      static_ptr_ref<gpu::rtv_descriptor_heap> RTVHeap;
      static_ptr_ref<window> Window;
      static_ptr_ref<content::buffers::DEPTH_STENCIL_BUFFER> DepthStencilConfig;
      static_ptr_ref<content::buffers::RASTERIZER_BUFFER> RasterizerConfig;
      static_ptr_ref<content::buffers::BLENDER_BUFFER> BlenderConfig;
      UINT FrameIndex;
   };

   /*
    Constructs a frame.
    */
   extern "C"[[nodiscard]] QGL_GRAPHICS_API HRESULT QGL_CC
      qgl_make_frame(static_ptr_ref<igraphics_device> dev_p,
                     IFRAME_CREATION_PARAMS* params_p,
                     qgl_version_t v,
                     iframe** out_p) noexcept;
}