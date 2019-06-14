#include "pch.h"
#include "include/Content-Buffers/qgl_content_dictionary_metadata_buffer.h"

namespace qgl::content
{
   static constexpr uint64_t DEFAULT_FLAGS = 0;

   CONTENT_DICTIONARY_METADATA_BUFFER::CONTENT_DICTIONARY_METADATA_BUFFER() :
      m_count(0),
      m_entrySize(0),
      m_reserved1(0),
      m_flags(DEFAULT_FLAGS)
   {
   }

   CONTENT_DICTIONARY_METADATA_BUFFER::CONTENT_DICTIONARY_METADATA_BUFFER(
      size_t count,
      size_t entrySize) :
      m_count(count),
      m_entrySize(entrySize),
      m_reserved1(0),
      m_flags(DEFAULT_FLAGS)
   {
   }

   size_t CONTENT_DICTIONARY_METADATA_BUFFER::count() const noexcept
   {
      return static_cast<size_t>(m_count);
   }

   size_t CONTENT_DICTIONARY_METADATA_BUFFER::entry_size() const noexcept
   {
      return static_cast<size_t>(m_entrySize);
   }

   uint64_t CONTENT_DICTIONARY_METADATA_BUFFER::flags() const noexcept
   {
      return m_flags;
   }
}