#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Shaders/qgl_vertex_element_descriptor.h"

namespace qgl::graphics::shaders
{
#pragma pack(push, 1)
   struct vertex_layout_descriptor final
   {
      static constexpr size_t MAX_ELEMENTS = 32;

      constexpr vertex_layout_descriptor()
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
         swap(l.data_class, r.data_class);
         swap(l.slot, r.slot);
         swap(l.elements, r.elements);
         swap(l.reserved, r.reserved);
         swap(l.flags, r.flags);
         swap(l.element_count, r.element_count);

      }

      vertex_layout_descriptor& operator=(vertex_layout_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       How the geometry is presented to the input assembler.
       */
      topology_t topology = D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;

      /*
       Only valid when topology type is triangle strip.
       See strip_cut_t for more details.
       */
      strip_cut_t strip_cut = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;

      /*
       How to classify the data for the input assembler.
       */
      input_data_class_t data_class = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
      
      /*
       Which slot to use when there are multiple vertex buffers.
       */
      input_slot_t slot = 0;

      /*
       Number of vertex element descriptors.
       */
      uint8_t element_count = 0;

      uint8_t reserved = 0;

      /*
       Flags. Currently unused.
       */
      qgl::mem::flags<16> flags = 0;

      /*
       Vertex elements.
       */
      fixed_buffer<vertex_element_descriptor, MAX_ELEMENTS> elements;
   };
#pragma pack(pop)
}