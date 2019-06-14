#include "pch.h"
#include "include/Content-Buffers/qgl_content_dict_entry_buffer.h"

namespace qgl::content
{
   CONTENT_DICTIONARY_ENTRY_BUFFER::CONTENT_DICTIONARY_ENTRY_BUFFER() :
      m_offset(static_cast<uint64_t>(-1)),
      m_flags(DEFAULT_FLAGS),
      m_size(static_cast<uint64_t>(0)),
      m_reserved1(static_cast<uint64_t>(0)),
      m_info()
   {
   }

   CONTENT_DICTIONARY_ENTRY_BUFFER::CONTENT_DICTIONARY_ENTRY_BUFFER(
      size_t entrySize,
      const CONTENT_METADATA_BUFFER* entryInfo,
      size_t entryOffset) :
      m_offset(entryOffset),
      m_flags(DEFAULT_FLAGS),
      m_reserved1(static_cast<uint64_t>(0)),
      m_size(entrySize),
      m_info(*entryInfo)
   {

   }

   size_t CONTENT_DICTIONARY_ENTRY_BUFFER::offset() const noexcept
   {
      return static_cast<size_t>(m_offset);
   }

   size_t CONTENT_DICTIONARY_ENTRY_BUFFER::size() const noexcept
   {
      return static_cast<size_t>(m_size);
   }

   uint64_t CONTENT_DICTIONARY_ENTRY_BUFFER::flags() const noexcept
   {
      return m_flags;
   }

   const CONTENT_METADATA_BUFFER* CONTENT_DICTIONARY_ENTRY_BUFFER::metadata()
      const noexcept
   {
      return &m_info;
   }

   CONTENT_METADATA_BUFFER* CONTENT_DICTIONARY_ENTRY_BUFFER::metadata()
      noexcept
   {
      return &m_info;
   }
}