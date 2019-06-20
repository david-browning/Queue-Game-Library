#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content::buffers
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

   /*
    This is the header that comes before the shader data in a shader file or 
    shader entry.
    */
   struct QGL_GRAPHICS_API SHADER_HEADER_BUFFER final
   {
      public:
      /*
       Default constructor.
       */
      SHADER_HEADER_BUFFER();

      /*
       Constructor.
       */
      SHADER_HEADER_BUFFER(SHADER_TYPES type,
                           SHADER_VENDORS vendor);

      /*
       Copy constructor.
       */
      SHADER_HEADER_BUFFER(const SHADER_HEADER_BUFFER&) = default;

      /*
       Move constructor.
       */
      SHADER_HEADER_BUFFER(SHADER_HEADER_BUFFER&&) = default;

      /*
       Destructor. Do not mark virtual.
       */
      ~SHADER_HEADER_BUFFER() noexcept = default;

      inline friend void swap(SHADER_HEADER_BUFFER& l, 
                       SHADER_HEADER_BUFFER& r) noexcept
      {
         using std::swap;
         swap(l.Type, r.Type);
         swap(l.Vendor, r.Vendor);
         swap(l.VersionMajor, r.VersionMajor);
         swap(l.VersionMinor, r.VersionMinor);
         swap(l.Reserved1, r.Reserved1);
      }

      inline SHADER_HEADER_BUFFER& operator=(SHADER_HEADER_BUFFER r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      inline friend bool operator==(const SHADER_HEADER_BUFFER& l,
                                    const SHADER_HEADER_BUFFER& r) noexcept
      {
         return l.Type == r.Type &&
            l.Vendor == r.Vendor &&
            l.VersionMajor == r.VersionMajor &&
            l.VersionMinor == r.VersionMinor;
      }

      /*
       Type of shader. See SHADER_TYPES.
       */
      uint8_t Type;

      /*
       Shader Vendor. See SHADER_VENDORS.
       */
      uint8_t Vendor;

      /*
       For HLSL, this corresponds to the shader model.
       */
      uint8_t VersionMajor;

      /*
       For HLSL, this corresponds to the shader model.
       */
      uint8_t VersionMinor;


      uint32_t Reserved1;
   };

   static_assert(sizeof(SHADER_HEADER_BUFFER) == 8,
                 "SHADER_HEADER_BUFFER must be 8 bytes.");
}