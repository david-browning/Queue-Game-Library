#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Shaders/qgl_shader_compile_params.h"

namespace qgl::graphics::shaders
{
#pragma pack(push, 1)
   /*
    Parameters needed to load a shader library.
    */
   struct shader_lib_descriptor final
   {
      constexpr shader_lib_descriptor()
      {

      }

      constexpr shader_lib_descriptor(const shader_lib_descriptor&) = default;

      constexpr shader_lib_descriptor(
         shader_lib_descriptor&&) noexcept = default;

      ~shader_lib_descriptor() noexcept = default;

      friend void swap(shader_lib_descriptor& l, 
                       shader_lib_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.vendor, r.vendor);
         swap(l.payload, r.payload);
         swap(l.compile_params, r.compile_params);
      }

      shader_lib_descriptor& operator=(shader_lib_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      shader_vendors vendor = shader_vendors::unknown;

      shader_payloads payload = shader_payloads::unknown;

      /*
       Only valid if the payload is "source".
       */
      shader_compile_params compile_params;
   };
#pragma pack(pop)
}