#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/igpu_buffer.h"
#include "include/Content/Content-Buffers/qgl_rasterizer_buffer.h"

namespace qgl::graphics::content
{
   class QGL_GRAPHICS_API rasterizer
   {
      public:
      rasterizer();

      rasterizer(const content::buffers::RASTERIZER_BUFFER* buff);

      rasterizer(const rasterizer&) = default;

      rasterizer(rasterizer&&) = default;

      virtual ~rasterizer() noexcept = default;

      const D3D12_RASTERIZER_DESC* description() const;

      private:
      D3D12_RASTERIZER_DESC m_desc;
   };
}