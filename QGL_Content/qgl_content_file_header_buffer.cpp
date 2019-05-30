#include "pch.h"
#include "include/Content-Buffers/qgl_content_file_header_buffer.h"

namespace qgl::content
{
   CONTENT_FILE_HEADER_BUFFER::CONTENT_FILE_HEADER_BUFFER() :
      m_offset(sizeof(*this)),
      m_reserved1(0),
      m_reserved2(0),
      m_reserved3(0)
   {
   }

   CONTENT_FILE_HEADER_BUFFER::CONTENT_FILE_HEADER_BUFFER(
      const CONTENT_METADATA_BUFFER* metadata) :
      m_offset(sizeof(*this)),
      m_metadata(*metadata),
      m_reserved1(0),
      m_reserved2(0),
      m_reserved3(0)
   {
   }

   const CONTENT_METADATA_BUFFER* CONTENT_FILE_HEADER_BUFFER::metadata() 
      const noexcept
   {
      return &m_metadata;
   }

   uint64_t CONTENT_FILE_HEADER_BUFFER::dictionary_offset() const noexcept
   {
      return m_offset;
   }
}