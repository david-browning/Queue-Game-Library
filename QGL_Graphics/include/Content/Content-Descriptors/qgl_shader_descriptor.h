#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics
{
   enum class SHADER_TYPES : uint8_t
   {
      SHADER_TYPE_VS = 0,
      SHADER_TYPE_DS = 1,
      SHADER_TYPE_HS = 2,
      SHADER_TYPE_GS = 3,
      SHADER_TYPE_PS = 4,
   };

   enum class SHADER_VENDORS : uint8_t
   {
      SHADER_VENDOR_UNKNOWN = 0,
      SHADER_VENDOR_DIRECTX = 1,
   };
}

namespace qgl::graphics::descriptors
{
#pragma pack(push, 1)
   struct shader_descriptor final
   {
      constexpr shader_descriptor() :
         type(SHADER_TYPES::SHADER_TYPE_VS),
         vendor(SHADER_VENDORS::SHADER_VENDOR_UNKNOWN),
         version_major(0), version_minor(0), reserved1(0)
      {

      }

      constexpr shader_descriptor(
         SHADER_TYPES t,
         SHADER_VENDORS v,
         uint8_t major,
         uint8_t minor) :
         type(t), vendor(v), version_major(major), version_minor(minor),
         reserved1(0)
      {

      }

      shader_descriptor(const shader_descriptor&) = default;

      shader_descriptor(shader_descriptor&&) = default;

      ~shader_descriptor() noexcept = default;

      friend void swap(shader_descriptor& l, shader_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.type, r.type);
         swap(l.vendor, r.vendor);
         swap(l.version_major, r.version_major);
         swap(l.version_minor, r.version_minor);
         swap(l.reserved1, r.reserved1);
      }

      shader_descriptor& operator=(shader_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      SHADER_TYPES type;

      SHADER_VENDORS vendor;

      /*
       For HLSL, this corresponds to the shader model.
       */
      uint8_t version_major;

      /*
       For HLSL, this corresponds to the shader model.
       */
      uint8_t version_minor;


      uint32_t reserved1;

   };
#pragma pack(pop)

   static_assert(sizeof(shader_descriptor) == 8,
              "SHADER_HEADER_BUFFER must be 8 bytes.");
}