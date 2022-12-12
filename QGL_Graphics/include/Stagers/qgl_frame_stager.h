#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/qgl_frame.h"

namespace qgl::stagers
{
   /*
    Used to pass frame pointers to an object.
    */
   using frame_stager = typename std::list<const graphics::gpu::frame*>;

   /*
    Gets the address of a frame.
    */
   inline const graphics::gpu::frame* frame_address(
      const graphics::gpu::frame* p) noexcept
   {
      return p;
   }
   
   /*
    Gets the address of a frame.
    */
   inline const graphics::gpu::frame* frame_address(
      const graphics::gpu::frame& f) noexcept
   {
      return std::addressof(f);
   }
   
   /*
    Gets the address of a frame.
    */
   inline const graphics::gpu::frame* frame_address(
      const std::reference_wrapper<graphics::gpu::frame>& f) noexcept
   {
      return std::addressof(f.get());
   }
   
   /*
    Gets the address of a frame.
    */
   inline const graphics::gpu::frame* frame_address(
      const std::unique_ptr<graphics::gpu::frame>& p) noexcept
   {
      return p.get();
   }
   
   /*
    Gets the address of a frame.
    */
   inline const graphics::gpu::frame* frame_address(
      const std::shared_ptr<graphics::gpu::frame>& p) noexcept
   {
      return p.get();
   }

   /*
    Creates a collection of frames that will be passed to another object.
    The frame stager returned by this contains only references to the frames.
    Freeing the frames that were passed to this will cause this to point to
    invalid memory.
    FrameForwardIt must point to some kind of frame pointer or reference. Do
    not pass the entire frame object.
    */
   template<typename FrameForwardIt>
   inline frame_stager make_frame_stager(
      FrameForwardIt first, FrameForwardIt last)
   {
      frame_stager ret;
      for (; first != last; first++)
      {
         ret.push_back(frame_address(*first));
      }

      return ret;
   }

   /*
    Creates a collection of frames that will be passed to another object.
    The frame stager returned by this contains only references to the frames.
    Freeing the frames that were passed to this will cause this to point to
    invalid memory.
    FrameT must point to some kind of frame pointer or reference. Do not pass
    the entire frame object.
    */
   template<class FrameT>
   inline frame_stager make_frame_stager(
      std::initializer_list<FrameT> frames)
   {
      return make_frame_stager(frames.begin(), frames.end());
   }
}