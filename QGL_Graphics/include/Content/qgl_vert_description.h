#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_vert_elem_buffer.h"

namespace qgl::content
{
   /*
    Describes the vertex layout for the input assembler.
    */
   class QGL_GRAPHICS_API vertex_description : public content_item
   {
      public:
      /* 
       elements: Array of VERTEX_ELEMENT_BUFFERs. The data in elements is 
        copied so the array can be freed.
       numElements: Number of elements in the array. 
       name: Name of this content.
       id: ID assigned to this content.
       Throws std::invalid_argument if two elements occupy the same index or if 
       no element occupies a particular index.
       */
      vertex_description(const buffers::VERTEX_ELEMENT_BUFFER* elements,
                         size_t numElements,
                         const wchar_t* name,
                         const qgl::content::content_id id);

      /*
       Copy constructor.
       */
      vertex_description(const vertex_description&);

      /*
       Move constructor.
       */
      vertex_description(vertex_description&&);

      /*
       Destructor
       */
      virtual ~vertex_description() noexcept;

      /*
       Returns the number of input element descriptions.
       */
      size_t size() const noexcept;

      /*
       Returns a pointer to the array of element descriptions.
       */
      const D3D12_INPUT_ELEMENT_DESC* data() const noexcept;

      private:
      struct impl;
      impl* m_impl = nullptr;
   };
}