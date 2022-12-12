#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Shaders/qgl_shader_compile_params.h"

namespace qgl::graphics::shaders
{
   enum class shader_vendors : uint8_t
   {
      unknown = 0,
      directx = 1,
   };

   enum class shader_payloads : uint8_t
   {
      unknown = 0,
      compiled = 1,
      source = 2,
   };
   
   /*
    Converts a DirectX shader type to a QGL shader type.
    */
   extern "C" QGL_GRAPHICS_API shader_types QGL_CC to_shader_type(
      D3D12_SHADER_VERSION_TYPE t) noexcept;

   /*
    Describes how to load shader data.
    */
#pragma pack(push, 1)
   struct shader_descriptor final
   {
      constexpr shader_descriptor()
      {

      }

      constexpr shader_descriptor(const shader_descriptor&) = default;

      constexpr shader_descriptor(shader_descriptor&&) noexcept = default;

      ~shader_descriptor() noexcept = default;

      friend void swap(shader_descriptor& l, shader_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.type, r.type);
         swap(l.vendor, r.vendor);
         swap(l.payload, r.payload);
         swap(l.reserved1, r.reserved1);
         swap(l.compile_params, r.compile_params);
      }

      shader_descriptor& operator=(shader_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Type of shader.
       */
      shader_types type = shader_types::unknown;

      shader_vendors vendor = shader_vendors::unknown;

      /*
       How to read the shader data.
       */
      shader_payloads payload = shader_payloads::unknown;

      uint8_t reserved1 = 0;

      /*
       Only valid if the payload is "source".
       */
      shader_compile_params compile_params;
   };
#pragma pack(pop)
}