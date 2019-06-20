#include "pch.h"
#include "include/Content/qgl_shader.h"

namespace qgl::content
{
   shader::shader(const buffers::SHADER_HEADER_BUFFER* hdr_p,
                  const void* data,
                  size_t dataSizeBytes,
                  const wchar_t* name,
                  content_id id) :
      m_header(*hdr_p),
      content_item(name, id,
                   RESOURCE_TYPES::RESOURCE_TYPE_SHADER,
                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_SHADER_COMPILED)
   {
      m_data = new uint8_t[dataSizeBytes];
      memcpy(m_data, data, dataSizeBytes);
      m_byteCode.BytecodeLength = dataSizeBytes;
      m_byteCode.pShaderBytecode = m_data;
   }

   shader::shader(const shader& r) :
      m_header(r.m_header),
      content_item(r)
   {
      delete[] m_data;
      const auto dataSize = r.m_byteCode.BytecodeLength;
      m_data = new uint8_t[dataSize];
      memcpy(m_data, r.m_data, dataSize);

      m_byteCode.BytecodeLength = dataSize;
      m_byteCode.pShaderBytecode = m_data;
   }

   shader::shader(shader&& r) :
      m_header(std::move(r.m_header)),
      content_item(std::move(r))
   {
      delete[] m_data;
      m_data = r.m_data;
      m_byteCode = r.m_byteCode;
      
      r.m_data = nullptr;
      r.m_byteCode.BytecodeLength = 0;
      r.m_byteCode.pShaderBytecode = nullptr;
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

   buffers::SHADER_TYPES shader::type() const noexcept
   {
      return static_cast<buffers::SHADER_TYPES>(m_header.Type);
   }

   buffers::SHADER_VENDORS shader::vendor() const noexcept
   {
      return static_cast<buffers::SHADER_VENDORS>(m_header.Vendor);
   }
}