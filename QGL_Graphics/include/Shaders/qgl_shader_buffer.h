#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Shaders/qgl_shader_descriptor.h"
#include "include/Shaders/qgl_shader_helpers.h"

namespace qgl::graphics::shaders
{
   /*
    Represents a shader that can be bound to a graphics pipeline state.
    */
   class shader_buffer final
   {
      public:
      /*
       Reads the shader data (and optionally compiles it) into this.\
       Errors are reported to the optional error reporter.
       */
      template<typename CharT = char>
      shader_buffer(shader_descriptor& desc,
             const shader_data& shaderData,
             error_reporter<CharT>* e = nullptr) :
         m_dsc(desc)
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
               if (desc.type != desc.compile_params.type)
               {
                  throw std::invalid_argument{
                     "The shader type and compile parameter type do not match."
                  };
               }
               m_shaderData = compile_shader(shaderData, desc.compile_params, e);
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
       Reads the shader data from a file (and optionally compiles it) into this.
       Errors are reported to the optional error reporter.
       */
      template<typename CharT = char>
      shader_buffer(shader_descriptor& desc,
             const qgl::sys_str& file,
             error_reporter<CharT>* e = nullptr) :
         m_dsc(desc)
      {
         m_shaderData = compile_shader(file, desc.compile_params, e);
         m_byteCode.BytecodeLength = m_shaderData.size();
         m_byteCode.pShaderBytecode = m_shaderData.data();
      }

      /*
       Copy constructor.
       */
      shader_buffer(const shader_buffer& r) :
         m_dsc(r.m_dsc),
         m_shaderData(r.m_shaderData)
      {
         m_byteCode.BytecodeLength = m_shaderData.size();
         m_byteCode.pShaderBytecode = m_shaderData.data();
      }

      /*
       Move constructor.
       */
      shader_buffer(shader_buffer&& r) noexcept:
         m_dsc(std::move(r.m_dsc)),
         m_shaderData(std::move(r.m_shaderData))
      {
         m_byteCode.BytecodeLength = m_shaderData.size();
         m_byteCode.pShaderBytecode = m_shaderData.data();
      }

      /*
       Destructor.
       */
      ~shader_buffer() noexcept = default;

      /*
       Returns the type of shader.
       */
      shader_types type() const noexcept
      {
         return m_dsc.type;
      }

      /*
       Returns a const reference to the D3D shader byte code.
       */
      const D3D12_SHADER_BYTECODE& byte_code() const noexcept
      {
         return m_byteCode;
      }

      /*
       Returns the shader vendor.
       */
      shader_vendors vendor() const noexcept
      {
         return m_dsc.vendor;
      }

      private:
      shader_descriptor m_dsc;
      shader_data m_shaderData;
      D3D12_SHADER_BYTECODE m_byteCode = { 0 };
   };
}