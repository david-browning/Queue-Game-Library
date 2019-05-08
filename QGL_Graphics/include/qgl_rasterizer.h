#pragma once
#include "qgl_graphics_include.h"
#include "qgl_rasterizer_buffer.h"

namespace qgl::graphics
{
   class LIB_EXPORT rasterizer
   {
      public:
      rasterizer();

      rasterizer(const RASTERIZER_BUFFER& buff);

      rasterizer(const rasterizer& r);

      rasterizer(rasterizer&& r);

      virtual ~rasterizer();

      inline const D3D12_RASTERIZER_DESC& description() const
      {
         return m_desc;
      }

      private:
      D3D12_RASTERIZER_DESC m_desc;
   };
}