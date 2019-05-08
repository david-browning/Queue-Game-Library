#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content
{
   struct LIB_EXPORT CONTENT_DICTIONARY_BUFFER
   {
      public:
      CONTENT_DICTIONARY_BUFFER();

      CONTENT_DICTIONARY_BUFFER(size_t count,
                                size_t entrySize,
                                uint64_t flags);

      CONTENT_DICTIONARY_BUFFER(const CONTENT_DICTIONARY_BUFFER& r);

      CONTENT_DICTIONARY_BUFFER(CONTENT_DICTIONARY_BUFFER&& r);   

      inline size_t count() const
      {
         return static_cast<size_t>(m_count);
      }

      inline size_t entry_size() const
      {
         return static_cast<size_t>(m_entrySize);
      }

      inline auto flags() const
      {
         return m_flags;
      }

      friend void swap(CONTENT_DICTIONARY_BUFFER& first, 
                       CONTENT_DICTIONARY_BUFFER& second) noexcept
      {
         using std::swap;
         swap(first.m_count, second.m_count);
         swap(first.m_entrySize, second.m_entrySize);
         swap(first.m_flags, second.m_flags);
         swap(first.m_reserved1, second.m_reserved1);
      }

      CONTENT_DICTIONARY_BUFFER& operator=(CONTENT_DICTIONARY_BUFFER r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:
      uint64_t m_count;

      //Size (in bytes) of each entry in the dictionary.
      uint64_t m_entrySize;

      //Reserved right now.
      uint64_t m_flags;

      uint64_t m_reserved1;
   };
}