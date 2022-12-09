#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Descriptors/qgl_vertex_element_descriptor.h"

namespace qgl::descriptors
{
#pragma pack(push, 1)
   struct vertex_layout_descriptor final
   {
      static constexpr size_t MAX_ELEMENTS = 32;

      constexpr vertex_layout_descriptor()
      {
      }

      constexpr vertex_layout_descriptor(
         D3D12_PRIMITIVE_TOPOLOGY_TYPE topo,
         D3D12_INDEX_BUFFER_STRIP_CUT_VALUE strip) :
         topology(static_cast<uint8_t>(topo)),
         strip_cut(static_cast<uint8_t>(strip))
      {
      }

      constexpr vertex_layout_descriptor(
         const vertex_layout_descriptor&) = default;

      constexpr vertex_layout_descriptor(
         vertex_layout_descriptor&&) noexcept = default;

      ~vertex_layout_descriptor() noexcept = default;

      friend void swap(
         vertex_layout_descriptor& l,
         vertex_layout_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.topology, r.topology);
         swap(l.strip_cut, r.strip_cut);
         swap(l.flags, r.flags);
         swap(l.elements, r.elements);
         swap(l.element_count, r.element_count);

      }

      vertex_layout_descriptor& operator=(vertex_layout_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      qgl::mem::flags<8> flags = 0;
      uint8_t topology = D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
      uint8_t strip_cut = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
      uint8_t element_count = 0;
      fixed_buffer<vertex_element_descriptor, MAX_ELEMENTS> elements;
   };
#pragma pack(pop)
}