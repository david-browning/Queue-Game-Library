#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics
{
   enum class SHADER_TYPES
   {
      SHADER_TYPE_VERTEX,
      SHADER_TYPE_PIXEL,
      SHADER_TYPE_GEOMETRY,
      SHADER_TYPE_DOMAIN,
      SHADER_TYPE_HULL,
   };

   template<SHADER_TYPES ShaderT>
   class ishader
   {
      public:
      ishader(const winrt::hstring& csoFile)
      {
         p_allocate_cso(p_open_file(csoFile));
         p_setup_bytecode();
      }

      ishader(const ishader& r) :
         m_byteCode(r.m_byteCode),
         m_byteCodeData(r.m_byteCodeData)
      {
      }

      ishader(ishader&& r) :
         m_byteCode(std::move(r.m_byteCode)),
         m_byteCodeData(std::move(r.m_byteCodeData))
      {
      }

      virtual ~ishader()
      {

      }

      inline const D3D12_SHADER_BYTECODE& byte_code() const noexcept
      {
         return m_byteCode;
      }

      inline D3D12_SHADER_BYTECODE& byte_code() noexcept
      {
         return m_byteCode;
      }

      private:
      winrt::file_handle p_open_file(const winrt::hstring& filePath)
      {
         CREATEFILE2_EXTENDED_PARAMETERS openParameters;
         openParameters.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
         openParameters.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
         openParameters.dwFileFlags = FILE_FLAG_OVERLAPPED | FILE_FLAG_SEQUENTIAL_SCAN;
         openParameters.dwSecurityQosFlags = SECURITY_ANONYMOUS;
         openParameters.hTemplateFile = nullptr;
         openParameters.lpSecurityAttributes = nullptr;

         //Open the shader file.
         return qgl::content::open_file_read(filePath, openParameters);
      }

      /*
       Opens a cso file which is an already compiled shader.
       */
      void p_allocate_cso(const winrt::file_handle& csoFile)
      {
         //Since the file was already compiled, just read in the raw byte data.
         //The file handle has already been opened.
         auto allocationSize = qgl::content::file_size(csoFile);
         m_byteCodeData.resize(allocationSize);

         //Read the file into memory.
         qgl::content::read_file_sync(csoFile, allocationSize, 0, m_byteCodeData.data());

      }

      /*
       Opens an hlsl file which needs to be compiled.
       */
      void p_allocate_hlsl(const winrt::file_handle& hlslFile)
      {
         //The file has not been compiled. First, read the ascii code in:
         auto allocationSize = qgl::content::file_size(hlslFile);
         std::vector<uint8_t> srcCode(allocationSize);
         qgl::content::read_file_sync(hlslFile, allocationSize, 0, srcCode.data());

         //Compile the source code.
         //TODO: Implement this in the future?
         winrt::throw_hresult(E_NOTIMPL);
      }

      void p_setup_bytecode()
      {
         m_byteCode.BytecodeLength = m_byteCodeData.size();
         m_byteCode.pShaderBytecode = reinterpret_cast<void*>(m_byteCodeData.data());
      }

      D3D12_SHADER_BYTECODE m_byteCode;
      std::vector<uint8_t> m_byteCodeData;
   };
}