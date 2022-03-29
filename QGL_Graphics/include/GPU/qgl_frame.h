#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Render/qgl_scissor.h"
#include "include/GPU/Render/qgl_viewport.h"
#include "include/GPU/Render/qgl_blender.h"
#include "include/GPU/Render/qgl_rasterizer.h"
#include "include/GPU/Render/qgl_depth_stencil.h"
#include "include/GPU/Render/qgl_render_target.h"

namespace qgl::graphics::gpu
{
   struct frame_params
   {
      std::shared_ptr<render_target> frame_buffer;
      std::shared_ptr<scissor> frame_scissor;
      std::shared_ptr<viewport> frame_viewport;
      std::shared_ptr<rasterizer> frame_rasterizer;
      std::shared_ptr<depth_stencil> frame_stencil;
      std::shared_ptr<blender> frame_blender;
      size_t frameIdx;
   };

   class frame
   {
      public:
      frame(frame_params&& params) :
         m_hndls(std::forward<frame_params>(params))
      {

      }

      frame(const frame&) = delete;

      frame(frame&&) = default;

      /*
       Returns a pointer to the render target.
       */
      const render_target& frame_buffer() const noexcept
      {
         return *m_hndls.frame_buffer;
      }

      render_target& frame_buffer() noexcept
      {
         return *m_hndls.frame_buffer;
      }

      /*
       Returns a pointer to the scissor.
       By default, the scissor uses the same area as the viewport.
       */
      const scissor& frame_scissor() const noexcept
      {
         return *m_hndls.frame_scissor;
      }

      scissor& frame_scissor() noexcept
      {
         return *m_hndls.frame_scissor;
      }

      /*
       Returns a pointer to the viewport.
       The viewport defines the space of the render target that will be
       presented.
       */
      const viewport& frame_viewport() const noexcept
      {
         return *m_hndls.frame_viewport;
      }

      viewport& frame_viewport() noexcept
      {
         return *m_hndls.frame_viewport;
      }

      /*
       Returns a pointer to the stencil.
       */
      const depth_stencil& frame_stencil() const noexcept
      {
         return *m_hndls.frame_stencil;
      }

      depth_stencil& frame_stencil() noexcept
      {
         return *m_hndls.frame_stencil;
      }

      const rasterizer& frame_rasterizer() const noexcept
      {
         return *m_hndls.frame_rasterizer;
      }

      rasterizer& frame_rasterizer() noexcept
      {
         return *m_hndls.frame_rasterizer;
      }

      const blender& frame_blender() const noexcept
      {
         return *m_hndls.frame_blender;
      }

      blender& frame_blender() noexcept
      {
         return *m_hndls.frame_blender;
      }

      void release()
      {
         m_hndls.frame_stencil->release();
         m_hndls.frame_buffer->release();
      }

      void acquire()
      {
         m_hndls.frame_buffer->acquire();
         m_hndls.frame_stencil->acquire();
      }

      private:
      frame_params m_hndls;
   };
}