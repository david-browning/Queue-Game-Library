#pragma once
#include "include/qgl_content_include.h"
#include "include/qgl_content_info_buffer.h"

namespace qgl::content
{
   struct LIB_EXPORT CONTENT_DICTIONARY_ENTRY_BUFFER
   {
      public:
      CONTENT_DICTIONARY_ENTRY_BUFFER();

      CONTENT_DICTIONARY_ENTRY_BUFFER(size_t entrySize,
                                      CONTENT_INFO_BUFFER& entryInfo,
                                      size_t entryOffset = -1);

      CONTENT_DICTIONARY_ENTRY_BUFFER(const CONTENT_DICTIONARY_ENTRY_BUFFER& r);

      CONTENT_DICTIONARY_ENTRY_BUFFER(CONTENT_DICTIONARY_ENTRY_BUFFER&& r);
      
      ~CONTENT_DICTIONARY_ENTRY_BUFFER() noexcept = default;

      inline const size_t offset() const noexcept
      {
         return static_cast<size_t>(m_offset);
      }

      inline void offset(size_t of)
      {
         m_offset = static_cast<uint64_t>(of);
      }

      inline const size_t size() const noexcept
      {
         return static_cast<size_t>(m_size);
      }

      inline void size(size_t sz) noexcept
      {
         m_size = static_cast<uint64_t>(sz);
      }

      inline const uint32_t flags() const noexcept
      {
         return m_flags;
      }

      inline uint32_t& flags() noexcept
      {
         return m_flags;
      }

      inline const CONTENT_INFO_BUFFER& type() const noexcept
      {
         return m_info;
      }

      inline CONTENT_INFO_BUFFER& type() noexcept
      {
         return m_info;
      }
      
      friend void swap(CONTENT_DICTIONARY_ENTRY_BUFFER& first, 
                       CONTENT_DICTIONARY_ENTRY_BUFFER& second) noexcept
      {
         using std::swap;
         swap(first.m_offset, second.m_offset);
         swap(first.m_size, second.m_size);
         swap(first.m_flags, second.m_flags);
         swap(first.m_info, second.m_info);
      }

      CONTENT_DICTIONARY_ENTRY_BUFFER& operator=(CONTENT_DICTIONARY_ENTRY_BUFFER r)  noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:

       //Offset in the file (in bytes) to the object's data.
      uint64_t m_offset;

      //Size (in bytes) of the object's data.
      uint64_t m_size;

      //Object flags.
      uint32_t m_flags;

      //Information about the object that this dictionary entry points to.
      CONTENT_INFO_BUFFER m_info;
   };
}