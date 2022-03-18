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
      frame();

      frame(const frame&) = delete;

      frame(frame&&) = default;

      const render_target& frame_buffer() const noexcept;
      render_target& frame_buffer() noexcept;

      const scissor& frame_scissor() const noexcept;
      scissor& frame_scissor() noexcept;

      const viewport& frame_viewport() const noexcept;
      viewport& frame_viewport() noexcept;

      const depth_stencil& frame_stencil() const noexcept;
      depth_stencil& frame_stencil() noexcept;

      const rasterizer& frame_rasterizer() const noexcept;
      rasterizer& frame_rasterizer() noexcept;

      const blender& frame_blender() const noexcept;
      blender& frame_blender() noexcept;

   };
}