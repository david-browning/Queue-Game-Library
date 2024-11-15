#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Shaders/qgl_shader_lib_descriptor.h"

namespace qgl::graphics::shaders
{
   /*
    Represents a library of shader functions and buffers. This could be an
    HLSL include file or compiled shader code. The shader source/compiled code
    must have at least 1 exported function or the constructor will fail.
    */
   class shader_lib final
   {
      public:
      /*
       Reads the shader library (and optionally compiles it) into this.
       Errors are reported to the optional error reporter. 
       */
      template<typename CharT = char, class SRWTraits = srw_traits>
      shader_lib(const shader_lib_descriptor& desc,
                 const graphics::shaders::shader_data& shaderData,
                 error_reporter<CharT, SRWTraits>* e = nullptr)
      {
         switch (desc.payload)
         {
            case shader_payloads::compiled:
            {
               m_shaderData = shaderData;
               break;
            }
            case shader_payloads::source:
            {
               m_shaderData = compile_shader_library(
                  shaderData, desc.compile_params, e);
               break;
            }
            default:
            {
               throw std::out_of_range("The payload type is unknown.");
            }
         }

         m_byteCode.BytecodeLength = m_shaderData.size();
         m_byteCode.pShaderBytecode = m_shaderData.data();
      }

      /*
       Reads the shader library from a file (and optionally compiles it) into 
       this.
       Errors are reported to the optional error reporter.
       */
      template<typename CharT = char, class SRWTraits = srw_traits>
      shader_lib(const shader_lib_descriptor& desc,
                 const qgl::sys_str& file,
                 error_reporter<CharT, SRWTraits>* e = nullptr)
      {
         m_shaderData = compile_shader_library(
            file, desc.compile_params, e);
         m_byteCode.BytecodeLength = m_shaderData.size();
         m_byteCode.pShaderBytecode = m_shaderData.data();
      }

      shader_lib(const shader_lib&) = default;

      shader_lib(shader_lib&&) noexcept = default;

      virtual ~shader_lib() noexcept = default;

      /*
       Returns a const reference to the D3D shader byte code.
       */
      const D3D12_SHADER_BYTECODE& byte_code() const noexcept
      {
         return m_byteCode;
      }

      private:
      shader_data m_shaderData;
      D3D12_SHADER_BYTECODE m_byteCode = { 0 };
   };
}