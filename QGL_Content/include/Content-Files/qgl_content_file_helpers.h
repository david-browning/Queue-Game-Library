#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Dictionary/qgl_content_dictionary.h"
#include "include/Content-Buffers/qgl_content_file_header_buffer.h"
#include "include/Content-Files/qgl_content_variant_entry.h"
#include "include/qgl_file_handle.h"

namespace qgl::content::content_file_helpers
{
   /*
    Reads the file header from a content file and returns it.
    The file must be opened with read permissions.
    This does not validate the data read.
    Throws an exception if there is an error reading.
    */
   extern CONTENT_FILE_HEADER_BUFFER load_header(const file_handle* hndl);

   /*
    Reads the dictionary metadata from a content file at the offset.
    The offset is stored in the file's header.
    The file must be opened with read permissions.
    This does not validate the data read.
    Throws an exception if there is an error reading.
    */
   extern CONTENT_DICTIONARY_METADATA_BUFFER load_dictionary_metadata(
      const file_handle* hndl,
      size_t dictionaryOffset);

   /*
    Reads a dictionary entry from a content file and returns it.
    The file must be opened with read permissions.
    This does not validate the data read.
    Throws an exception if there is an error reading.
    */
   extern CONTENT_DICTIONARY_ENTRY_BUFFER load_dictionary_entry(
      const file_handle* hndl,
      size_t entryOffset);

   /*
    Reads the content data from a content file. The data is accessed using the
    dictionary entry. Returns a vector of bytes.
    The file must be opened with read permissions.
    This does not validate the data read.
    Throws an exception if there is an error reading.
    */
   extern DATA_CONTENT_ENTRY load_content_data(
      const file_handle* hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry);

   /*
    Reads the shared entry from a content file. The path is access using the
    dictionary entry. Retuns the shared entry path.
    The file must be opened with read permissions.
    This does not validate the data read.
    Throws an exception if there is an error reading.
    */
   extern SHARED_CONTENT_ENTRY load_shared_data_path(
      const file_handle* hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry);

   /*
    Writes a file header to a content file.
    The file must be opened with write permissions.
    */
   extern void write_header(const file_handle* hndl,
                            const CONTENT_FILE_HEADER_BUFFER& hdr);

   /*
    Writes the dictionary metadata to a content file.
    The file must be opened with write permissions.
    */
   extern void write_dictionary_metadata(
      const file_handle* hndl,
      const CONTENT_DICTIONARY_METADATA_BUFFER& meta,
      size_t offset);

   /*
    Writes a dictionary entry to the content file.
    The file must be opened with write permissions.
    */
   extern void write_dictionary_entry(
      const file_handle* hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
      size_t offset);

   /*
    Writes the content data to the content file.
    The file must be opened with write permissions.
    */
   extern void write_content_data(
      const file_handle* hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
      const DATA_CONTENT_ENTRY& contentData);

   /*
    Writes the shared content path to the content file.
    The file must be opened with write permissions.
    */
   extern void write_shared_data_path(
      const file_handle* hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
      const SHARED_CONTENT_ENTRY& path);

   /*
    Returns the offset, in bytes, to where content data starts in a file.
    Content data comes after the file header and dictionary.
    */
   extern size_t dictionary_data_offset(
      const CONTENT_FILE_HEADER_BUFFER& fileHeader,
      const CONTENT_DICTIONARY_METADATA_BUFFER& dictMeta);

   /*
    Gets the size of the buffer. The buffer is the size of an 8 byte integer
    and sizeof(wchar_t) * number of characters in the path.
    */
   extern size_t shared_entry_data_size(const SHARED_CONTENT_ENTRY& data);

   extern bool valid_content_file_size(const file_handle* hndl);
}
