#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Descriptors/qgl_sampler_descriptor.h"

namespace qgl::descriptors
{
   enum class texure_depth_types : uint8_t
   {
      normal = 0,
      hdr = 1,
   };

   enum class texture_types : uint8_t
   {
      unknown = 0,
      dds = 1,
      wic = 2,
      hdr = 3,
      tga = 4,
   };

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

      constexpr texture_descriptor(const texture_descriptor&) = default;

      constexpr texture_descriptor(texture_descriptor&&) noexcept = default;

      ~texture_descriptor() noexcept = default;

      texture_descriptor(const texture_descriptor&) = default;

      texture_descriptor(texture_descriptor&&) = default;

      ~texture_descriptor() noexcept = default;

      friend void swap(texture_descriptor& l, texture_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.flags, r.flags);
         swap(l.load_flags, r.load_flags);
         swap(l.index, r.index);
         swap(l.clamp, r.clamp);
         swap(l.highest_mip, r.highest_mip);
         swap(l.type, r.type);
         swap(l.depth_type, r.depth_type);
         swap(l.resource_flags, r.resource_flags);
         swap(l.image_flags, r.image_flags);
         swap(l.sampler, r.sampler);
      }

      texture_descriptor& operator=(texture_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       */
      mem::flags<16> flags = 0;

      /*
       */
      uint32_t load_flags = DirectX::CREATETEX_FLAGS::CREATETEX_DEFAULT;

      /*
       The index of the first texture to use in an array of textures.
       Only valid for DDS textures.
       */
      uint16_t index = 0;

      /*
       A value to clamp sample LOD values to. For example, if you specify 2.0f
       for the clamp value, you ensure that no individual sample accesses a
       mip level less than 2.0f.
       Only valid for DDS textures.
       */
      math::rational<int32_t> clamp;

      /*
       Index of the most detailed mipmap level to use.
       Only valid for DDS textures.
       */
      uint16_t highest_mip = 0;


      texture_types type = texture_types::unknown;

      texure_depth_types depth_type = texure_depth_types::normal;

      /*
       */
      uint32_t resource_flags = D3D12_RESOURCE_FLAG_NONE;

      /*
       Interpret this field based on "type". These image_flags could be a 
       WIC_FLAGS, DDS_FLAGS, TGA_FLAGS
       */
      mem::flags<32> image_flags = 0;

      /*
       How to sample this texture.
       */
      sampler_descriptor sampler;
   };
#pragma pack(pop)
}