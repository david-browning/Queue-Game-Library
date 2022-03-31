#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Descriptors/qgl_shader_descriptor.h"

namespace qgl::graphics
{
   class shader
   {
      public:
      using shader_data = typename std::vector<std::byte>;

      /*
       Creates a shader from the give shader byte code. The byte code must be
       compiled. It cannot be source code.
       */
      shader(descriptors::shader_descriptor&& desc,
             void* shaderData,
             size_t shaderSize) :
         m_dsc(std::forward<descriptors::shader_descriptor>(desc))
      {
         m_shaderData.resize(shaderSize);
         memcpy(m_shaderData.data(), shaderData, shaderSize);
         m_byteCode.BytecodeLength = m_shaderData.size();
         m_byteCode.pShaderBytecode = m_shaderData.data();
      }

      shader(descriptors::shader_descriptor&& desc,
             shader_data&& shaderData) :
         m_dsc(std::forward<descriptors::shader_descriptor>(desc)),
         m_shaderData(std::forward<shader_data>(shaderData))
      {
         m_byteCode.BytecodeLength = m_shaderData.size();
         m_byteCode.pShaderBytecode = m_shaderData.data();
      }

      /*
       Copy constructor.
       */
      shader(const shader&) = delete;

      /*
       Move constructor.
       */
      shader(shader&&) = default;

      /*
       Destructor.
       */
      ~shader() noexcept = default;

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
      descriptors::shader_descriptor m_dsc;
      shader_data m_shaderData;
      D3D12_SHADER_BYTECODE m_byteCode;
   };
}