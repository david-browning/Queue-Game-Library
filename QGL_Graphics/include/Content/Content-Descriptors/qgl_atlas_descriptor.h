#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Descriptors/qgl_atlas_entry_descriptor.h"

namespace qgl::graphics::descriptors
{
   /*
    Contains information about a texture atlas. This does not store the actual
    image.
    */
#pragma pack(push, 1)
   struct atlas_descriptor final
   {
      constexpr atlas_descriptor()
      {

      }

      friend void swap(atlas_descriptor& l, atlas_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.flags, r.flags);
         swap(l.count, r.count);
         swap(l.padding, r.padding);
      }

      atlas_descriptor& operator=(atlas_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       */
      mem::flags<32> flags = 0;

      /*
       Number of elements in the atlas.
       */
      uint16_t count = 0;

      /*
       The amount of padding (in pixels) around each image in the atlas.
       The distance between two images then is 2 * padding.
       */
      uint16_t padding = 4;
   };
#pragma pack(pop)
}