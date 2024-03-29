#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::shaders
{
#pragma pack(push, 1)
   struct vertex_element_descriptor final
   {
      /*
       Default constructor. The element is invalid and all fields need to be
       overwritten before using this.
       */
      constexpr vertex_element_descriptor()
      {

      }

      /*
       Constructor.
       semantic: Shader semantic associated with this element in a shader input
        signature.
       semanticIdx: The semantic index for the element. A semantic index
        modifies a semantic, with an integer index number.
       fmt: Describes the size of the element. This is usually a vector or
        matrix.
       Throws std::invalid_argument if the semantic name is too long.
       */
      vertex_element_descriptor(const std::string& semantic,
                                uint8_t semanticIdx,
                                DXGI_FORMAT fmt) :
         semantic_name(semantic.c_str(), semantic.size()),
         semantic_index(semanticIdx),
         format(static_cast<uint16_t>(fmt)),
         data_class(D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
      {

      }

      constexpr vertex_element_descriptor(
         const vertex_element_descriptor&) = default;

      constexpr vertex_element_descriptor(vertex_element_descriptor&&) = default;

      ~vertex_element_descriptor() noexcept = default;

      friend void swap(
         vertex_element_descriptor& l,
         vertex_element_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.semantic_name, r.semantic_name);
         swap(l.semantic_index, r.semantic_index);
         swap(l.format, r.format);
         swap(l.data_class, r.data_class);
      }

      vertex_element_descriptor& operator=(vertex_element_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Maximum number of character in a semantic name.
       */
      static constexpr size_t MAX_SEMANTIC_NAME_LEN = 32;

      /*
       Semantic name.
       May or may not end with a null-terminator.
       */
      qgl::fixed_buffer<char, MAX_SEMANTIC_NAME_LEN> semantic_name = { '\0' };

      /*
       The semantic index for the element. A semantic index modifies a
       semantic, with an integer index number. A semantic index is only needed
       in a case where there is more than one element with the same semantic.
       For example, a 4x4 matrix would have four components each with the
       semantic name matrix, however each of the four component would have
       different semantic indices (0, 1, 2, and 3).
       */
      uint32_t semantic_index = 0;

      /*
       format of the element data. See DXGI_FORMAT.
       */
      uint16_t format = DXGI_FORMAT_UNKNOWN;

      /*
       A value that identifies the input data class for a single input slot.
       */
      input_data_class_t data_class = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
   };
#pragma pack(pop)
}