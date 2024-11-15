#pragma once
#include "include/qgl_graphics_include.h"
#include <QGLStruct.h>
#include <QGLMemory.h>

namespace qgl::graphics::shaders
{
   /*
    Different types of shaders.
    */
   enum class shader_types : uint8_t
   {
      unknown = 0,
      vs = 1,
      ds = 2,
      hs = 3,
      gs = 4,
      ps = 5,
      cs = 6,
      lib = 7,
   };

   /*
    Describes how to compile a shader or shader library.
    */
#pragma pack(push, 1)
   struct shader_compile_params final
   {
      static constexpr size_t MAX_COMPILE_NAME_LEN = 32;
      static constexpr size_t MAX_SHADER_NAME_LEN = 64;

      constexpr shader_compile_params() :
         shader_model(5),
         effect_type(0),
         type(shader_types::unknown),
         reserved1(0),
         flags(0)
      {

      }

      constexpr shader_compile_params(const shader_compile_params&) = default;

      constexpr shader_compile_params(
         shader_compile_params&&) noexcept = default;

      ~shader_compile_params() noexcept = default;

      friend void swap(shader_compile_params& l,
                       shader_compile_params& r) noexcept
      {
         using std::swap;
         swap(l.effect_type, r.effect_type);
         swap(l.entry, r.entry);
         swap(l.name, r.name);
         swap(l.flags, r.flags);
         swap(l.shader_model, r.shader_model);
         swap(l.type, r.type);
         swap(l.reserved1, r.reserved1);
      }

      /*
       Name of the shader. This can be any 64 character alphanumeric string.
       */
      fixed_buffer<char, MAX_SHADER_NAME_LEN> name = { 0 };

      /*
       The name of the shader entry point function where shader execution
       begins. Ignored for shader libraries
       */
      fixed_buffer<char, MAX_COMPILE_NAME_LEN> entry = { 0 };

      /*
       Which shader model to compile as.
       */
      uint8_t shader_model;

      /*
       Which effect type to compile as.
       */
      uint8_t effect_type;

      /*
       Type of shader to compile as.
       */
      shader_types type;

      uint8_t reserved1;

      /*
       Low 32 bits map to the flags1 parameter of D3DCompile.
       High 32 bits map to the flags2 parameter of D3DCompile.
       */
      qgl::mem::flags<64, true> flags;

   };
#pragma pack(pop)
}