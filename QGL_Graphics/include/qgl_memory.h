#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics::low
{
   static constexpr size_t TEXTURE_MS_ALIGN = 4096 * 1024;
   static constexpr size_t BUFFER_ALIGN = 64 * 1024;

   enum class ALIGNMENTS : size_t
   {
      ALIGNMENT_BUFFER = TEXTURE_MS_ALIGN,
      ALIGNMENT_TEXTURE = BUFFER_ALIGN,
      ALIGNMENT_TEXTURE_MS = BUFFER_ALIGN,
   };

   template<size_t UAlignment, typename AddressT>
   constexpr AddressT align(AddressT address)
   {
      static_assert(!(UAlignment == 0 || (UAlignment & (UAlignment - 1))),
                    "UAlignment must be a power of 2.");

      return (address + (UAlignment - 1) & ~(UAlignment - 1));
   }
}