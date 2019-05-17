#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Dictionary/qgl_content_dictionary.h"
#include "include/Content-Buffers/qgl_content_file_header_buffer.h"
#include <type_traits>

namespace qgl::content
{
   using content_data_buffer_t = std::vector<uint8_t>;

   /*
    Reads the file header from a content file and returns it.
    The file must be opened with read permissions.
    This does not validate the data read.
    Throws an exception if there is an error reading.
    */
   extern LIB_EXPORT CONTENT_FILE_HEADER_BUFFER load_header(
      const winrt::file_handle& hndl);

   /*
    Reads the dictionary metadata from a content file at the offset.
    The offset is stored in the file's header.
    The file must be opened with read permissions.
    This does not validate the data read.
    Throws an exception if there is an error reading.
    */
   extern LIB_EXPORT CONTENT_DICTIONARY_METADATA_BUFFER
      load_dictionary_metadata(
         const winrt::file_handle& hndl,
         size_t dictionaryOffset);

   /*
    Reads a dictionary entry from a content file and returns it.
    The file must be opened with read permissions.
    This does not validate the data read.
    Throws an exception if there is an error reading.
    */
   extern LIB_EXPORT CONTENT_DICTIONARY_ENTRY_BUFFER load_dictionary_entry(
      const winrt::file_handle& hndl,
      size_t entryOffset);

   /*
    Reads the dictionary from a content file and returns it. The dictionary
    offset is stored in the file's header.
    The file must be opened with read permissions.
    This does not validate the data read.
    Throws an exception if there is an error reading.
    */
   extern LIB_EXPORT content_dictionary load_dictionary(
      const winrt::file_handle& hndl,
      size_t dictionaryOffset);

   /*
    Reads the content data from a content file. The data is accessed using the
    dictionary entry. Returns a vector of bytes.
    The file must be opened with read permissions.
    This does not validate the data read.
    Throws an exception if there is an error reading.
    */
   extern LIB_EXPORT std::vector<uint8_t> load_content_data(
      const winrt::file_handle& hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry);

   /*
    Writes a file header to a content file.
    The file must be opened with write permissions.
    */
   extern LIB_EXPORT void write_header(const winrt::file_handle& hndl,
                                       const CONTENT_FILE_HEADER_BUFFER& hdr);

   /*
    Writes the dictionary metadata to a content file.
    The file must be opened with write permissions.
    */
   extern LIB_EXPORT void write_dictionary_metadata(
      const winrt::file_handle& hndl,
      const CONTENT_DICTIONARY_METADATA_BUFFER& meta,
      size_t offset);

   /*
    Writes a dictionary entry to the content file.
    The file must be opened with write permissions.
    */
   extern LIB_EXPORT void write_dictionary_entry(
      const winrt::file_handle& hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
      size_t offset);

   /*
    Writes a dictionary to a content file.
    The file must be opened with write permissions.
    */
   extern LIB_EXPORT void write_dictionary(const winrt::file_handle& hndl,
                                           size_t dictionaryOffset,
                                           const content_dictionary& dict);

   /*
    Writes the content data to the content file.
    */
   extern LIB_EXPORT void write_content_data(
      const winrt::file_handle& hndl,
      CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
      const void* contentData);




















   /*
    Returns the offset, in bytes, to where content data starts in a file.
    Content data comes after the file header and dictionary.
    */
   extern LIB_EXPORT size_t dictionary_data_offset(
      const CONTENT_FILE_HEADER_BUFFER& fileHeader,
      const CONTENT_DICTIONARY_METADATA_BUFFER& dictMeta);

   extern LIB_EXPORT bool valid_content_file_size(
      const winrt::file_handle& hndl);

   extern LIB_EXPORT bool valid_content_file(const winrt::file_handle& hndl);

   /*
    Writes the content data, stored in a dictionary, to a content file.
    The file handle must be opened with write permissions.
    startOffset is the offset, in bytes, to where content data begins. This
    can be found using dictionary_data_offset().
    The dictionary iterators point to a CONTENT_DICTIONARY_ENTRY_BUFFER that 
    describes the content's size and metadata. Size and metadata must be valid.
    */
   template<class DictionaryEntryForwardIterator, class DataForwardIterator>
   void write_dictionary_content(const winrt::file_handle& hndl,
                                 size_t startOffset,
                                 DictionaryEntryForwardIterator firstDictEntry,
                                 DictionaryEntryForwardIterator lastDictEntry,
                                 DataForwardIterator firstContentData,
                                 DataForwardIterator lastContentData)
   {
      //Check that the iterator types are expected.
      auto firstData = *firstContentData;
      static_assert(
         std::is_same<decltype(firstData), content_data_buffer_t>::value,
         "Content data iterators must point to content_data_buffer_t.");

      auto firstEntry = *firstDictEntry;
      static_assert(
         std::is_same<decltype(firstEntry),
         CONTENT_DICTIONARY_ENTRY_BUFFER>::value,
         "Dictionary iterators must be a CONTENT_DICTIONARY_ENTRY_BUFFER");

      //Check that there are the same number of dictionary entries as data 
      //pointers.
      if (std::distance(firstContentData, lastContentData) !=
          std::distance(firstDictEntry, lastDictEntry))
      {
         throw std::invalid_argument("There must be an equal number of "
                                     "dictionary entries and content data "
                                     "pointers.");
      }

      while (firstDictEntry != lastDictEntry)
      {
         //Adjust the dictionary entry's data pointer.
         firstDictEntry->offset() = startOffset;
         startOffset += firstDictEntry->size();

         write_content_data(hndl, *firstDictEntry, *firstContentData);

         firstDictEntry++;
         firstContentData++;
      }
   }
}
