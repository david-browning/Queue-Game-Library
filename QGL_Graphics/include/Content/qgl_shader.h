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
      shader(descriptors::shader_descriptor& desc,
             void* shaderData,
             size_t shaderSize) :
         m_dsc(desc)
      {
         m_shaderData.resize(shaderSize);
         memcpy(m_shaderData.data(), shaderData, shaderSize);
         m_byteCode.BytecodeLength = m_shaderData.size();
         m_byteCode.pShaderBytecode = m_shaderData.data();
      }

      shader(descriptors::shader_descriptor& desc,
             shader_data& shaderData) :
         m_dsc(desc),
         m_shaderData(shaderData)
      {
         m_byteCode.BytecodeLength = m_shaderData.size();
         m_byteCode.pShaderBytecode = m_shaderData.data();
      }

      shader(descriptors::shader_descriptor& desc,
             shader_data&& shaderData) :
         m_dsc(desc),
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

   using shader_stager = typename std::list<shader*>;

   /*
    Creates a collection of shaders that will be passed to another object.
    The shader stager returned by this contains only references to the shaders.
    Freeing the shaders that were passed to this will cause this to point to
    invalid memory.
    */
   template<class ForwardIt>
   inline shader_stager make_shader_stager(ForwardIt first, ForwardIt last)
   {
      using itType = std::remove_reference<decltype(*first)>::type;
      static_assert(std::is_same<shader, itType>::value,
                    "Dereferencing first does not yield a 'shader'.");

      shader_stager ret;
      for (; first != last; first++)
      {
         ret.push_back(std::addressof(*first));
      }

      return ret;
   }

   /*
    Creates a collection of shaders that will be passed to another object.
    The shader stager returned by this contains only references to the shaders.
    Freeing the shaders that were passed to this will cause this to point to
    invalid memory.
    */
   inline shader_stager make_shader_stager(
      std::initializer_list<std::reference_wrapper<shader>> shaders)
   {
      shader_stager ret;
      for (auto& shader : shaders)
      {
         ret.push_back(std::addressof(shader.get()));
      }

      return ret;
   }

   /*
    Creates a collection of shaders that will be passed to another object.
    The shader stager returned by this contains only references to the shaders.
    Freeing the shaders that were passed to this will cause this to point to
    invalid memory.
    */
   inline shader_stager make_shader_stager(
      std::initializer_list<std::shared_ptr<shader>> shaders)
   {
      shader_stager ret;
      for (auto& shader_p : shaders)
      {
         ret.push_back(shader_p.get());
      }

      return ret;
   }

   /*
    Creates a collection of shaders that will be passed to another object.
    The shader stager returned by this contains only references to the shaders.
    Freeing the shaders that were passed to this will cause this to point to
    invalid memory.
    */
   inline shader_stager make_shader_stager(
      std::initializer_list<std::unique_ptr<shader>> shaders)
   {
      shader_stager ret;
      for (auto& shader_p : shaders)
      {
         ret.push_back(shader_p.get());
      }

      return ret;
   }
}