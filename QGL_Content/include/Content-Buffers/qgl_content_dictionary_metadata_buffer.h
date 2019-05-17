#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content
{
    /*
     This is metadata for a content dictionary.
     */
   struct LIB_EXPORT CONTENT_DICTIONARY_METADATA_BUFFER
   {
      public:
      static constexpr uint64_t DEFAULT_FLAGS = 0;

      /*
       Default constructor.
       This metadata buffer has no dictionary entries, default flags, and
       the entry size is 0.
       */
      CONTENT_DICTIONARY_METADATA_BUFFER();

      CONTENT_DICTIONARY_METADATA_BUFFER(size_t count,
                                         size_t entrySize,
                                         uint64_t flags = DEFAULT_FLAGS);

      CONTENT_DICTIONARY_METADATA_BUFFER(
         const CONTENT_DICTIONARY_METADATA_BUFFER& r) = default;

      CONTENT_DICTIONARY_METADATA_BUFFER(
         CONTENT_DICTIONARY_METADATA_BUFFER&& r) = default;

     /*
      Returns the number of items in the dictionary.
      */
      inline size_t count() const noexcept
      {
         return static_cast<size_t>(m_count);
      }

      /*
       Returns the size, in bytes, of a dictionary entry.
       */
      inline size_t entry_size() const noexcept
      {
         return static_cast<size_t>(m_entrySize);
      }

      /*
       Returns the dictionary flags.
       */
      inline auto flags() const noexcept
      {
         return m_flags;
      }

      friend void swap(CONTENT_DICTIONARY_METADATA_BUFFER& first,
                       CONTENT_DICTIONARY_METADATA_BUFFER& second) noexcept
      {
         using std::swap;
         swap(first.m_count, second.m_count);
         swap(first.m_entrySize, second.m_entrySize);
         swap(first.m_flags, second.m_flags);
         swap(first.m_reserved1, second.m_reserved1);
      }

      CONTENT_DICTIONARY_METADATA_BUFFER& operator=(
         CONTENT_DICTIONARY_METADATA_BUFFER r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      friend bool operator==(
         const CONTENT_DICTIONARY_METADATA_BUFFER& r,
         const CONTENT_DICTIONARY_METADATA_BUFFER& l) noexcept
      {
         return r.m_count == l.m_count &&
            r.m_entrySize == l.m_entrySize &&
            r.m_flags == l.m_flags &&
            r.m_reserved1 == l.m_reserved1;
      }

      private:

      /*
       Number of entries in the dictionary.
       */
      uint64_t m_count;

      /*
       Size (in bytes) of each entry in the dictionary.
       */
      uint64_t m_entrySize;

      /*
       Reserved right now.
       */
      uint64_t m_flags;

      uint64_t m_reserved1;
   };
}