#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Dictionary/qgl_content_dictionary.h"
#include "include/Content-Buffers/qgl_content_file_header_buffer.h"
#include <type_traits>

namespace qgl::content
{
   using content_data_buffer_t = std::vector<uint8_t>;
}

namespace qgl::content::content_file_helpers
{
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

   extern LIB_EXPORT bool valid_content_file_magic_number(uint64_t magicNum);

   extern LIB_EXPORT bool valid_content_file(const winrt::file_handle& hndl);
}
