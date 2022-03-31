#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::descriptors
{
#pragma pack(push, 1)
   struct vertex_layout_descriptor final
   {
      constexpr vertex_layout_descriptor()
      {
      }

      friend void swap(
         vertex_layout_descriptor& l,
         vertex_layout_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.topology, r.topology);
         swap(l.strip_cut, r.strip_cut);
      }

      vertex_layout_descriptor& operator=(vertex_layout_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      uint8_t topology = D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
      uint8_t strip_cut = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
   };
#pragma pack(pop)
}