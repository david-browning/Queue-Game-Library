#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics
{
   enum class texure_depth_types : uint8_t
   {
      normal = 0,
      hdr = 1,
   };
}

namespace qgl::graphics::descriptors
{
#pragma pack(push, 1)
   /*
    Describes how to render a texture.
    This is extra metadata not included in a DDS texture.
    */
   struct texture_descriptor final
   {
      constexpr texture_descriptor()
      {

      }

      texture_descriptor(const texture_descriptor&) = default;

      texture_descriptor(texture_descriptor&&) = default;

      ~texture_descriptor() noexcept = default;

      /*
       A value to clamp sample LOD values to. For example, if you specify 2.0f
       for the clamp value, you ensure that no individual sample accesses a
       mip level less than 2.0f.
       */
      math::rational<int32_t> clamp;

      /*
       Index of the most detailed mipmap level to use.
       */
      uint16_t highest_mip = 0;

      /*
       The index of the first texture to use in an array of textures.
       */
      uint16_t index = 0;

      texure_depth_types depth_type = texure_depth_types::normal;

      uint8_t reserved2 = 0;
   };
#pragma pack(pop)
}