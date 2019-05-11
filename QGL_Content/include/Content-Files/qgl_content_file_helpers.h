#pragma once
#include "include/qgl_content_include.h"
#include "include/qgl_content_dictionary.h"
#include "include/Content-Buffers/qgl_content_file_header_buffer.h"
#include <type_traits>

namespace qgl::content
{
   /*
    Reads the file header from a content file and returns it.
    The file must be opened with read permissions.
    This does not validate the data read.
    */
   CONTENT_FILE_HEADER_BUFFER load_header(const winrt::file_handle& hndl);

   /*
    Reads the dictionary from a content file and returns it. The dictionary
    offset is stored in the file's header.
    The file must be opened with read permissions.
    This does not validate the data read.
    */
   content_dictionary load_dictionary(const winrt::file_handle& hndl,
                                      size_t dictionaryOffset);

   /*
    Reads the content data from a content file. The data is accessed using the
    dictionary entry. Returns a vector of bytes.
    The file must be opened with read permissions.
    This does not validate the data read.
    */
   std::vector<uint8_t> load_content_data(
      const winrt::file_handle& hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry);

   /*
    Writes a file header to a content file.
    The file must be opened with write permissions.
    */
   void write_header(const winrt::file_handle& hndl,
                     const CONTENT_FILE_HEADER_BUFFER& hdr);

   /*
    Writes a dictionary to a content file.
    The file must be opened with write permissions.
    */
   void write_dictionary(const winrt::file_handle& hndl,
                         size_t dictionaryOffset,
                         const content_dictionary& dict);

   /*
    Writes the dictionary entry data to the file.
    The file must be opened with write permissions.
    */
   template<class DictionaryEntryForwardIterator, class DataForwardIterator>
   void write_dictionary_data(const winrt::file_handle& hndl,
                              size_t startOffset,
                              DictionaryEntryForwardIterator firstDictEntry,
                              DictionaryEntryForwardIterator lastDictEntry,
                              DataForwardIterator firstContentData,
                              DataForwardIterator lastContentData)
   {
      //Check that the iterator types are expected.
      auto firstData = *firstContentData;
      static_assert(std::is_same<decltype(firstData), const void*>::value,
                    "Content data iterators must point to const void*.");

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

         write_dictionary_entry_data(hndl, *firstDictEntry, *firstContentData);

         firstDictEntry++;
         firstContentData++;
      }
   }

   /*
    Writes the content data to the content file.
    */
   void write_dictionary_entry_data(const winrt::file_handle& hndl,
                                    CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
                                    const void* contentData);

   /*
    Returns the offset, in bytes, to where content data starts in a file.
    Content data comes after the file header and dictionary.
    */
   size_t dictionary_data_offset(
      const CONTENT_FILE_HEADER_BUFFER& fileHeader,
      const CONTENT_DICTIONARY_METADATA_BUFFER& dictMeta);
}
