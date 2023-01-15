#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Shaders/qgl_shader.h"
#include "include/Shaders/qgl_shader_lib.h"
#include "include/Shaders/qgl_shader_meta_types.h"

namespace qgl::graphics::shaders
{
   /*
    Holds metadata about a shader. This object is expensive to construct as it
    disassembles the compiled shader code passed to it.
    If you expect to find a buffer or variable and it is not present in the
    metadata, it is probably because the buffer was not used by any shader
    function.
    */
   class shader_metadata final
   {
      public:
      /*
       Decompiles the shader and pulls metadata from it.
       */
      shader_metadata(const shader& s)
      {
         m_type = s.type();
         construct_shader(s);
      }

      /*
       Decompiles the shader library and pulls metadata from it.
       */
      shader_metadata(const shader_lib& s)
      {
         m_type = shader_types::lib;
         construct_lib(s);
      }

      shader_metadata(const shader_metadata&) = default;

      shader_metadata(shader_metadata&&) noexcept = default;

      ~shader_metadata() noexcept = default;

      /*
       The type of shader.
       */
      shader_types type() const noexcept
      {
         return m_type;
      }

      /*
       Returns the shader disassembly code. This is not the original
       HSLS source code.
       */
      const std::string& disassembly() const noexcept
      {
         return m_disassembly;
      }

      /*
       Gets details about a function with name "name".
       */
      const shader_func_info& function(const std::string& name)
      {
         return m_funcs.at(name);
      }

      /*
       Gets details about a buffer with name "name".
       */
      const shader_buffer_info& buffer(const std::string& name)
      {
         return m_buffers.at(name);
      }

      /* 
       Begins iterating through the function names.
       */
      auto cbegin_funcs() const noexcept
      {
         return map_key_const_iterator<std::string, decltype(m_funcs)>{
            m_funcs };
      }

      /*
       Ends iterating through the function names.
       */
      auto cend_funcs() const noexcept
      {
         return map_key_const_iterator<std::string, decltype(m_funcs)>{
            m_funcs, nullptr };
      }

      /*
       Begins iterating through the buffer names.
       */
      auto cbegin_buffers() const noexcept
      {
         return map_key_const_iterator<std::string, decltype(m_buffers)>{
            m_buffers };
      }

      /*
       Ends iterating through the buffer names.
       */
      auto cend_buffers() const noexcept
      {
         return map_key_const_iterator<std::string, decltype(m_buffers)>{
            m_buffers, nullptr };
      }

      /*
       Begins iterating through the shader input parameters.
       */
      auto cbegin_inputs() const noexcept
      {
         return m_inputParams.cbegin();
      }

      /*
       Ends iterating through the shader input parameters.
       */
      auto cend_inputs() const noexcept
      {
         return m_inputParams.cend();
      }

      /*
       Begins iterating through the shader output parameters.
       */
      auto cbegin_outputs() const noexcept
      {
         return m_outputParams.cbegin();
      }

      /*
       Ends iterating through the shader output parameters.
       */
      auto cend_output() const noexcept
      {
         return m_outputParams.cend();
      }

      private:
      void construct_shader(const shader& s)
      {
         check_result(D3DReflect(
            s.byte_code().pShaderBytecode,
            s.byte_code().BytecodeLength,
            IID_PPV_ARGS(m_reflect_p.put())));

         get_source(s.byte_code());
         get_shader_info();
         get_input_params();
         get_output_params();
         get_buffer_info();
         get_variables();
      }

      void construct_lib(const shader_lib& s)
      {
         check_result(D3DReflectLibrary(
            s.byte_code().pShaderBytecode,
            s.byte_code().BytecodeLength,
            IID_PPV_ARGS(m_lib_p.put())));

         get_source(s.byte_code());
         get_lib_info();
         get_functions();
      }

      void get_shader_info()
      {
         check_result(m_reflect_p->GetDesc(&m_desc));
      }

      // Gets information about the parameters passed to the shader's entry 
      // point.
      void get_input_params()
      {
         for (UINT i = 0; i < m_desc.InputParameters; i++)
         {
            D3D12_SIGNATURE_PARAMETER_DESC desc;
            check_result(m_reflect_p->GetInputParameterDesc(
               i, &desc));
            m_inputParams.emplace_back(std::move(desc));
         }
      }

      // Gets information about the shader entry point's return type.
      void get_output_params()
      {
         for (UINT i = 0; i < m_desc.OutputParameters; i++)
         {
            D3D12_SIGNATURE_PARAMETER_DESC desc;
            check_result(m_reflect_p->GetOutputParameterDesc(
               i, &desc));
            m_outputParams.emplace_back(std::move(desc));
         }
      }

      // Gets information about the buffers the shader uses.
      void get_buffer_info()
      {
         for (UINT i = 0; i < m_desc.ConstantBuffers; i++)
         {
            auto b_p = m_reflect_p->GetConstantBufferByIndex(i);
            check_pointer(b_p);

            D3D12_SHADER_BUFFER_DESC bDesc;
            check_result(b_p->GetDesc(&bDesc));

            shader_var_stager vars;
            for (UINT vIdx = 0; vIdx < bDesc.Variables; vIdx++)
            {
               auto v_p = b_p->GetVariableByIndex(vIdx);
               check_pointer(v_p);

               auto t_p = v_p->GetType();
               check_pointer(t_p);
               D3D12_SHADER_TYPE_DESC tDesc;
               check_result(t_p->GetDesc(&tDesc));

               D3D12_SHADER_VARIABLE_DESC vDesc;
               check_result(v_p->GetDesc(&vDesc));
               vars.emplace_back(std::move(vDesc), std::move(tDesc));
            }

            m_buffers.emplace(std::pair<std::string, shader_buffer_info>(
               bDesc.Name, { bDesc, vars }));
         }
      }

      void get_variables()
      {
         shader_var_stager vars;
         while (false)
         {
            auto vInfo = m_reflect_p->GetVariableByName("");
            check_pointer(vInfo);

            auto t_p = vInfo->GetType();
            check_pointer(t_p);
            D3D12_SHADER_TYPE_DESC tDesc;
            check_result(t_p->GetDesc(&tDesc));

            D3D12_SHADER_VARIABLE_DESC vDesc;
            check_result(vInfo->GetDesc(&vDesc));
            vars.emplace_back(std::move(vDesc), std::move(tDesc));
         }
      }

      void get_lib_info()
      {
         check_result(m_lib_p->GetDesc(&m_libDesc));
      }

      /*
       Gets the free functions in the shader.
       */
      void get_functions()
      {
         for (UINT i = 0; i < m_libDesc.FunctionCount; i++)
         {
            auto f_p = m_lib_p->GetFunctionByIndex(i);
            check_pointer(f_p);

            D3D12_FUNCTION_DESC fDesc;
            check_result(f_p->GetDesc(&fDesc));

            shader_param_stager fParams;
            for (INT paramIdx = 0;
                 paramIdx < fDesc.FunctionParameterCount;
                 paramIdx++)
            {
               auto p_p = f_p->GetFunctionParameter(paramIdx);
               check_pointer(p_p);
               D3D12_PARAMETER_DESC pDesc;
               check_result(p_p->GetDesc(&pDesc));
               fParams.emplace_back(std::move(pDesc));
            }

            m_funcs.emplace(std::make_pair<std::string, shader_func_info>(
               fDesc.Name, { fDesc, fParams }));
         }
      }

      void get_source(const D3D12_SHADER_BYTECODE& source)
      {
         static constexpr auto COMMENT = "Disassembled by QGL.\n";
         pptr<ID3DBlob> diss_p;

         check_result(D3DDisassemble(
            source.pShaderBytecode,
            source.BytecodeLength,
            0,
            COMMENT,
            diss_p.put()));

         auto dataSize = diss_p->GetBufferSize();
         m_disassembly.resize(dataSize, '\0');
         memcpy_s(m_disassembly.data(), dataSize,
                  diss_p->GetBufferPointer(), dataSize);
      }

      shader_types m_type;
      pptr<ID3D12ShaderReflection> m_reflect_p;
      pptr<ID3D12LibraryReflection> m_lib_p;

      D3D12_SHADER_DESC m_desc;
      D3D12_LIBRARY_DESC m_libDesc;

      // Maps a function name to information about the function.
      std::unordered_map<std::string, shader_func_info> m_funcs;

      // Maps a buffer name to information about that buffer.
      std::unordered_map<std::string, shader_buffer_info> m_buffers;

      // List of input parameters to the shader's entry point.
      std::vector<shader_in_param> m_inputParams;

      // List of data types returned by the shader's entry point.
      std::vector<shader_out_param> m_outputParams;

      // The source code.
      std::string m_disassembly;
   };
}