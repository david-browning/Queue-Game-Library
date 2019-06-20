#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content::buffers
{
   #pragma pack(push, 1)
   struct QGL_GRAPHICS_API VERTEX_ELEMENT_BUFFER final
   {
      public:

      /*
       Default constructor. The element is invalid and all fields need to be 
       overwritten before using this.
       */
      VERTEX_ELEMENT_BUFFER();

      /*
       Constructor.
       semantic: Shader semantic associated with this element in a shader input 
        signature. Must be null-terminated
       semanticIdx: The semantic index for the element. A semantic index 
        modifies a semantic, with an integer index number.
       fmt: Describes the size of the element. This is usually a vector or 
        matrix.
       inputSlot: Which vertex buffer this binds to during input assembly.
       elementIdx: Index of this element in a vertex description.
       Throws std::invalid_argument if the semantic name is too long.
       */
      VERTEX_ELEMENT_BUFFER(
         const char* semantic,
         uint8_t semanticIdx,
         DXGI_FORMAT fmt,
         uint8_t inputSlot,
         uint8_t elementIdx);

      /*
       Copy constructor.
       */
      VERTEX_ELEMENT_BUFFER(const VERTEX_ELEMENT_BUFFER&) = default;

      /*
       Move constructor.
       */
      VERTEX_ELEMENT_BUFFER(VERTEX_ELEMENT_BUFFER&&) = default;

      /*
       Destructor. Do not mark as virtual.
       */
      ~VERTEX_ELEMENT_BUFFER() noexcept = default;

      /*
       Swaps the contents of l with r.
       */
      inline friend void swap(VERTEX_ELEMENT_BUFFER& l,
                              VERTEX_ELEMENT_BUFFER& r) noexcept
      {
         using std::swap;
         swap(l.SemanticName, r.SemanticName);
         swap(l.SemanticIndex, r.SemanticIndex);
         swap(l.Format, r.Format);
         swap(l.Slot, r.Slot);
         swap(l.Class, r.Class);
         swap(l.Index, r.Index);
      }

      /*
       Assignment operator.
       */
      inline VERTEX_ELEMENT_BUFFER& operator=(VERTEX_ELEMENT_BUFFER r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Returns true if the contents of l equal the contents of r.
       */
      inline friend bool operator==(const VERTEX_ELEMENT_BUFFER& l,
                                    const VERTEX_ELEMENT_BUFFER& r) noexcept
      {
         return l.SemanticIndex == r.SemanticIndex &&
            l.Format == r.Format &&
            l.Slot == r.Slot &&
            l.Class == r.Class &&
            r.Index == l.Index &&
            strcmp(l.SemanticName, r.SemanticName) == 0;
      }

      /*
       Maximum number of character in a semantic name.
       */
      static constexpr size_t MAX_SEMANTIC_NAME_LEN = 32;

      /*
       Semantic name.
       May or may not end with a null-terminator.
       */
      char SemanticName[MAX_SEMANTIC_NAME_LEN] = { 0 };

      /*
       The semantic index for the element. A semantic index modifies a 
       semantic, with an integer index number. A semantic index is only needed 
       in a case where there is more than one element with the same semantic. 
       For example, a 4x4 matrix would have four components each with the 
       semantic name matrix, however each of the four component would have 
       different semantic indices (0, 1, 2, and 3).
       */
      uint32_t SemanticIndex;

      /*
       Format of the element data. See DXGI_FORMAT.
       */
      uint16_t Format;

      uint8_t Slot;

      /*
       A value that identifies the input data class for a single input slot.
       By default, this is D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA.
       */
      uint8_t Class;

      /*
       Index of this element in a complete vertex description.
       */
      uint8_t Index;

      uint8_t Reserved1;

      uint16_t Reserved2;

      uint32_t Reserved3;

      uint32_t Reserved4;

      uint8_t Padding[12] = { 0 };
   };
   #pragma pack(pop)

   static_assert(sizeof(VERTEX_ELEMENT_BUFFER) == 64,
                 "Vertex Element buffer should be 64 bytes.");

}