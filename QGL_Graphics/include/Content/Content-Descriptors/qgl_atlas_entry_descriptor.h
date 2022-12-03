#pragma once
#include "include/qgl_graphics_include.h"
#include <include/Content/Content-Descriptors/qgl_vector_descriptor.h>

namespace qgl::graphics::descriptors
{
   /*
    Describes an area in a texture atlas.
    */
#pragma pack(push, 1)
   struct atlas_entry_descriptor final
   {
      static constexpr size_t ENTRY_NAME_MAX_SIZE = 32;

      constexpr atlas_entry_descriptor()
      {

      }

      auto top() const noexcept
      {
         return rect[0];
      }

      auto left() const noexcept
      {
         return rect[1];
      }

      auto bottom() const noexcept
      {
         return rect[2];
      }

      auto right() const noexcept
      {
         return rect[3];
      }

      friend void swap(atlas_entry_descriptor& l, 
                       atlas_entry_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.flags, r.flags);
         swap(l.reserved, r.reserved);
         swap(l.rect, r.rect);
      }

      atlas_entry_descriptor& operator=(atlas_entry_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Bit set of flags
       */
      mem::flags<32> flags = 0;

      uint32_t reserved = 0;

      /*
       The top left vertex is encoded in the x,y fields. The bottom right
       vertex is encoded in the z, w.
       */
      vector_descriptor rect;

      /*
       Name of this entry so the texture region can be queried later.
       */
      fixed_buffer<char, ENTRY_NAME_MAX_SIZE> name;
   };
#pragma pack(pop)
}