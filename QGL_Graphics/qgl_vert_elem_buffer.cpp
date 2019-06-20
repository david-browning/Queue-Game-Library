#include "pch.h"
#include "include/Content/Content-Buffers/qgl_vert_elem_buffer.h"

namespace qgl::content::buffers
{
   VERTEX_ELEMENT_BUFFER::VERTEX_ELEMENT_BUFFER() :
      SemanticIndex(0),
      Format(DXGI_FORMAT_UNKNOWN),
      Slot(0),
      Class(D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA),
      Index(0),
      Reserved1(0),
      Reserved2(0),
      Reserved3(0),
      Reserved4(0)
   {
      
   }

   VERTEX_ELEMENT_BUFFER::VERTEX_ELEMENT_BUFFER(const char* semantic,
                                                uint8_t semanticIdx,
                                                DXGI_FORMAT fmt,
                                                uint8_t inputSlot,
                                                uint8_t elementIdx) :
      SemanticIndex(semanticIdx),
      Format(static_cast<decltype(Format)>(fmt)),
      Slot(inputSlot),
      Class(D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA),
      Index(elementIdx),
      Reserved1(0),
      Reserved2(0),
      Reserved3(0),
      Reserved4(0)
   {
      auto nameLen = mem_length(semantic, '\0');
      if (nameLen > MAX_SEMANTIC_NAME_LEN)
      {
         throw std::invalid_argument("The semantic name is too long.");
      }

      memcpy(SemanticName, semantic, sizeof(char) * nameLen);
   }
}