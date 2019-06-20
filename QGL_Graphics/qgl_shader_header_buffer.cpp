#include "pch.h"
#include "include/Content/Content-Buffers/qgl_shader_header_buffer.h"

namespace qgl::content::buffers
{
   SHADER_HEADER_BUFFER::SHADER_HEADER_BUFFER() :
      Type(static_cast<uint8_t>(SHADER_TYPES::SHADER_TYPE_VS)),
      Vendor(static_cast<uint8_t>(SHADER_VENDORS::SHADER_VENDOR_UNKNOWN)),
      VersionMajor(0),
      VersionMinor(0),
      Reserved1(0)
   {
   }

   SHADER_HEADER_BUFFER::SHADER_HEADER_BUFFER(SHADER_TYPES type, 
                                              SHADER_VENDORS vendor) :
      Type(static_cast<uint8_t>(type)),
      Vendor(static_cast<uint8_t>(vendor)),
      VersionMajor(0),
      VersionMinor(0),
      Reserved1(0)
   {
   }
}