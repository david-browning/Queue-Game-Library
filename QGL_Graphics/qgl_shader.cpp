#include "pch.h"
#include "include/Content/qgl_shader.h"

namespace qgl::content
{
   shader::shader(const void* data, 
                  size_t dataSizeBytes, 
                  const wchar_t* name,
                  content_id id) :
      content_item(name, id,
                   RESOURCE_TYPES::RESOURCE_TYPE_SHADER,
                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_SHADER_COMPILED)
   {
      m_data = new uint8_t[dataSizeBytes];
      memcpy(m_data, data, dataSizeBytes);
      m_byteCode.BytecodeLength = dataSizeBytes;
      m_byteCode.pShaderBytecode = m_data;
   }

   shader::shader(const wchar_t* csoFile,
                  const wchar_t* name,
                  content_id id) :
      content_item(name, id,
                   RESOURCE_TYPES::RESOURCE_TYPE_SHADER,
                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_SHADER_COMPILED)
   {
      file_handle csoHandle;
      auto hr = open_file_read(csoFile, &csoHandle);
      winrt::check_hresult(hr);

      m_byteCode.BytecodeLength = 0; 
      hr = file_size(&csoHandle, &m_byteCode.BytecodeLength);
      winrt::check_hresult(hr);

      m_data = new uint8_t[m_byteCode.BytecodeLength];
      read_file_sync(&csoHandle, 
                     m_byteCode.BytecodeLength,
                     0, 
                     m_data);
   }

   shader::shader(const shader& r) :
      content_item(r)
   {
      delete[] m_data;
      auto dataSize = r.m_byteCode.BytecodeLength;
      m_data = new uint8_t[dataSize];
      memcpy(m_data, r.m_data, dataSize);

      m_byteCode.BytecodeLength = dataSize;
      m_byteCode.pShaderBytecode = m_data;
   }

   shader::shader(shader&& r) :
      content_item(std::move(r))
   {
      delete[] m_data;
      m_data = r.m_data;
      m_byteCode = r.m_byteCode;
   }

   shader::~shader() noexcept
   {
      delete m_data;
      m_data = nullptr;
   }

   const D3D12_SHADER_BYTECODE* shader::byte_code() const noexcept
   {
      return &m_byteCode;
   }
}