#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics
{
   enum class shader_types : uint8_t
   {
      vs = 0,
      ds = 1,
      hs = 2,
      gs = 3,
      ps = 4,
   };

   enum class shader_vendors : uint8_t
   {
      unknown = 0,
      directx = 1,
   };
}

namespace qgl::graphics::descriptors
{
#pragma pack(push, 1)
   struct shader_descriptor final
   {
      constexpr shader_descriptor()
      {

      }

      constexpr shader_descriptor(
         shader_types t,
         shader_vendors v,
         uint8_t major,
         uint8_t minor) :
         type(t), vendor(v), version_major(major), version_minor(minor)
      {

      }

      friend void swap(shader_descriptor& l, shader_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.type, r.type);
         swap(l.vendor, r.vendor);
         swap(l.version_major, r.version_major);
         swap(l.version_minor, r.version_minor);
      }

      shader_descriptor& operator=(shader_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      shader_types type = shader_types::vs;

      shader_vendors vendor = shader_vendors::unknown;

      /*
       For HLSL, this corresponds to the shader model.
       */
      uint8_t version_major = 0;

      /*
       For HLSL, this corresponds to the shader model.
       */
      uint8_t version_minor = 0;
   };
#pragma pack(pop)
}