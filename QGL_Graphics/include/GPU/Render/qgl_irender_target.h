#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::gpu
{
   class irender_target
   {
      public:

      virtual ~irender_target() noexcept = default;

      virtual DXGI_FORMAT format() const noexcept = 0;

      virtual D3D12_CPU_DESCRIPTOR_HANDLE where() const noexcept = 0;
   };
}