#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Buffers/qgl_content_metadata_buffer.h"

namespace qgl::content
{
    /*
     Dictionaries store entries, which map to content data.
     */
   struct LIB_EXPORT CONTENT_DICTIONARY_ENTRY_BUFFER
   {
      public:
      /*
       Default constructor.
       Sets the offset to 0, flags to default, metadata to default, and
       content's data size to 0.
       */
      CONTENT_DICTIONARY_ENTRY_BUFFER();

      CONTENT_DICTIONARY_ENTRY_BUFFER(size_t entrySize,
                                      CONTENT_METADATA_BUFFER& entryInfo,
                                      size_t entryOffset = -1);

      /*
       Copy constructor.
       */
      CONTENT_DICTIONARY_ENTRY_BUFFER(
         const CONTENT_DICTIONARY_ENTRY_BUFFER& r) = default;

     /*
      Move constructor.
      */
      CONTENT_DICTIONARY_ENTRY_BUFFER(
         CONTENT_DICTIONARY_ENTRY_BUFFER&& r) = default;

     /*
      Destructor. Do not mark virtual.
      */
      ~CONTENT_DICTIONARY_ENTRY_BUFFER() noexcept = default;

      /*
       Returns the offset, in bytes, to the entry's data.
       */
      inline size_t offset() const noexcept
      {
         return static_cast<size_t>(m_offset);
      }

      /*
       Returns the size, in bytes, of the entry's data.
       */
      inline size_t size() const noexcept
      {
         return static_cast<size_t>(m_size);
      }

      /*
       Returns the data's flags.
       */
      inline uint32_t flags() const noexcept
      {
         return m_flags;
      }

      /*
       Returns a reference to the content's metadata.
       */
      inline const CONTENT_METADATA_BUFFER& metadata() const noexcept
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

      CONTENT_DICTIONARY_ENTRY_BUFFER& operator=(
         CONTENT_DICTIONARY_ENTRY_BUFFER r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      template<class DictionaryEntryForwardIterator, class DataForwardIterator>
      friend void write_dictionary_data(
         const winrt::file_handle& hndl,
         size_t startOffset,
         DictionaryEntryForwardIterator firstDictEntry,
         DictionaryEntryForwardIterator lastDictEntry,
         DataForwardIterator firstContentData,
         DataForwardIterator lastContentData);

      private:

       /*
       Offset in the file (in bytes) to the object's data.
       */
      uint64_t m_offset;

      /*
       Size (in bytes) of the object's data.
       */
      uint64_t m_size;

      /*
       Object flags.
       */
      uint32_t m_flags;

      /*
       Information about the object that this dictionary entry points to.
       */
      CONTENT_METADATA_BUFFER m_info;

      static constexpr uint32_t DEFAULT_FLAGS = 0;
   };
}