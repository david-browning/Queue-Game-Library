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

   using frame_stager = typename std::list<frame*>;

   /*
    Creates a collection of frames that will be passed to another object.
    The frame stager returned by this contains only references to the frames.
    Freeing the frames that were passed to this will cause this to point to
    invalid memory.
    */
   template<typename ForwardIt>
   inline frame_stager make_frame_stager(ForwardIt first, ForwardIt last)
   {
      using itType = std::remove_reference<decltype(*first)>::type;
      frame_stager ret;

      if constexpr (std::is_pointer<itType>::value)
      {
         static_assert(std::is_same<frame*, itType>::value,
                    "Dereferencing first does not yield a pointer to 'frame'.");
         for (; first != last; first++)
         {
            ret.push_back(*first);
         }
      }
      else if constexpr (std::is_same<itType, std::unique_ptr<frame>>::value ||
                         std::is_same<itType, std::shared_ptr<frame>>::value)
      {
         for (; first != last; first++)
         {
            ret.push_back(first->get());
         }
      }
      else if constexpr (std::is_reference<itType>::value ||
                         std::is_same<itType, std::reference_wrapper<frame>>::value ||
                         std::is_same<itType, frame>::value)
      {
         for (; first != last; first++)
         {
            ret.push_back(std::addressof(*first));
         }
      }
      else
      {
         static_assert(false,
                       "Dereferencing first must yield a pointer or reference to a 'frame'.")
      }

      return ret;
   }

   /*
    Creates a collection of frames that will be passed to another object.
    The frame stager returned by this contains only references to the frames.
    Freeing the frames that were passed to this will cause this to point to
    invalid memory.
    */
   frame_stager make_frame_stager(
      std::initializer_list<std::reference_wrapper<frame>> frms)
   {
      frame_stager ret;
      for (auto& frm : frms)
      {
         ret.push_back(std::addressof(frm.get()));
      }

      return ret;
   }

   /*
    Creates a collection of frames that will be passed to another object.
    The frame stager returned by this contains only references to the frames.
    Freeing the frames that were passed to this will cause this to point to
    invalid memory.
    */
   inline frame_stager make_frame_stager(
      std::initializer_list<std::shared_ptr<frame>> frms)
   {
      frame_stager ret;
      for (auto& frm_p : frms)
      {
         ret.push_back(frm_p.get());
      }

      return ret;
   }

   /*
    Creates a collection of frames that will be passed to another object.
    The frame stager returned by this contains only references to the frames.
    Freeing the frames that were passed to this will cause this to point to
    invalid memory.
    */
   inline frame_stager make_frame_stager(
      std::initializer_list<std::unique_ptr<frame>> frms)
   {
      frame_stager ret;
      for (auto& frm_p : frms)
      {
         ret.push_back(frm_p.get());
      }

      return ret;
   }
}