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
   class frame
   {
      public:
      frame(render_target&& rt,
            scissor&& scsr,
            viewport&& vp,
            rasterizer&& rstrzr,
            blender&& blndr,
            depth_stencil&& dstncl) :
         m_rt(std::forward<render_target>(rt)),
         m_scissor(std::forward<scissor>(scsr)),
         m_viewport(std::forward<viewport>(vp)),
         m_rasterizer(std::forward<rasterizer>(rstrzr)),
         m_blender(std::forward<blender>(blndr)),
         m_dstencil(std::forward<depth_stencil>(dstncl))
      {

      }

      frame(const frame&) = delete;

      frame(frame&&) = default;

      virtual ~frame() = default;

      size_t index() const noexcept
      {
         return m_rt.index();
      }

      /*
       Returns a pointer to the render target.
       */
      const render_target& frame_buffer() const noexcept
      {
         return m_rt;
      }

      render_target& frame_buffer() noexcept
      {
         return m_rt;
      }

      /*
       Returns a pointer to the scissor.
       By default, the scissor uses the same area as the viewport.
       */
      const scissor& frame_scissor() const noexcept
      {
         return m_scissor;
      }

      scissor& frame_scissor() noexcept
      {
         return m_scissor;
      }

      /*
       Returns a pointer to the viewport.
       The viewport defines the space of the render target that will be
       presented.
       */
      const viewport& frame_viewport() const noexcept
      {
         return m_viewport;
      }

      viewport& frame_viewport() noexcept
      {
         return m_viewport;
      }

      /*
       Returns a pointer to the stencil.
       */
      const depth_stencil& frame_stencil() const noexcept
      {
         return m_dstencil;
      }

      depth_stencil& frame_stencil() noexcept
      {
         return m_dstencil;
      }

      const rasterizer& frame_rasterizer() const noexcept
      {
         return m_rasterizer;
      }

      rasterizer& frame_rasterizer() noexcept
      {
         return m_rasterizer;
      }

      const blender& frame_blender() const noexcept
      {
         return m_blender;
      }

      blender& frame_blender() noexcept
      {
         return m_blender;
      }

      void release()
      {
         m_dstencil.release();
         m_rt.release();
      }

      void acquire()
      {
         m_rt.acquire();
         m_dstencil.acquire();
      }

      private:
      render_target m_rt;
      scissor m_scissor;
      viewport m_viewport;
      rasterizer m_rasterizer;
      blender m_blender;
      depth_stencil m_dstencil;
   };
}