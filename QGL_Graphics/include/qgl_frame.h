#pragma once
#include "qgl_graphics_include.h"
#include "qgl_d3d_device.h"
#include "qgl_rtv_descriptor_heap.h"
#include "qgl_dsv_descriptor_heap.h"
#include "qgl_render_target.h"
#include "qgl_depth_stencil.h"
#include "qgl_viewport.h"
#include "qgl_scissor.h"
#include "qgl_window.h"

namespace qgl::graphics
{
   class LIB_EXPORT frame
   {
      public:
      frame(const graphics_device& dev,
            size_t frameIndex,
            rtv_descriptor_heap& rtvHeap,
            const DEPTH_STENCIL_BUFFER& buffer,
            dsv_descriptor_heap& dsvHeap,
            const window& wnd);

      frame(const frame& r) = delete;

      frame(frame&& r);

      virtual ~frame() = default;

      inline const render_target& frame_buffer() const
      {
         return m_renderTarget;
      }

      inline const depth_stencil& frame_stencil() const
      {
         return m_depthStencil;
      }

      inline render_target& frame_buffer()
      {
         return m_renderTarget;
      }

      inline depth_stencil& frame_stencil()
      {
         return m_depthStencil;
      }

      inline viewport& frame_viewport()
      {
         return m_vp;
      }

      inline scissor& frame_scissor()
      {
         return m_scissor;
      }

      /*
       Releases the frame and all references it owns.
       */
      void release(const winrt::com_ptr<d3d11_device>& d3d11Dev_p)
      {
         m_renderTarget.p_release(d3d11Dev_p);
      }

      friend void swap(frame& first, frame& second) noexcept
      {
         using std::swap;
         swap(first.m_renderTarget, second.m_renderTarget);
         swap(first.m_depthStencil, second.m_depthStencil);
      }

      frame& operator=(frame r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:

      render_target m_renderTarget;
      depth_stencil m_depthStencil;
      viewport m_vp;
      scissor m_scissor;
   };

   template<template<class>class ContainerT = std::vector, class FrameT = frame>
   ContainerT<std::shared_ptr<FrameT>> make_frames(const graphics_device& gDev,
                                                   rtv_descriptor_heap& rtvHeap,
                                                   const DEPTH_STENCIL_BUFFER& stnclBuffer,
                                                   dsv_descriptor_heap& dsvHeap,
                                                   const window& wnd)
   {
      const auto numFrames = gDev.buffers();
      ContainerT<std::shared_ptr<FrameT>> ret(numFrames);
      for (size_t i = 0; i < numFrames; i++)
      {
         //Make the frame
         ret[i] = std::make_shared<FrameT>(gDev,
                                           i,
                                           rtvHeap,
                                           stnclBuffer,
                                           dsvHeap,
                                           wnd);
      }

      return ret;
   }

   template<template<class>class ContainerT = std::vector, class FrameT = frame>
   void make_frames(const graphics_device& gDev,
                    rtv_descriptor_heap& rtvHeap,
                    const DEPTH_STENCIL_BUFFER& stnclBuffer,
                    dsv_descriptor_heap& dsvHeap,
                    const window& wnd,
                    ContainerT<std::shared_ptr<FrameT>>& ret)
   {
      const auto numFrames = gDev.buffers();
      for (size_t i = 0; i < numFrames; i++)
      {
         *(ret[i]) = frame(gDev,
                           i,
                           rtvHeap,
                           stnclBuffer,
                           dsvHeap,
                           wnd);
      }
   }
}