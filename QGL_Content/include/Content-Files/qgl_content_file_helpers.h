#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Buffers/qgl_content_dictionary_metadata_buffer.h"
#include "include/Content-Buffers/qgl_content_dict_entry_buffer.h"
#include "include/Content-Buffers/qgl_content_file_header_buffer.h"
#include "include/Content-Files/qgl_content_variant_entry.h"
#include "include/qgl_file_handle.h"

namespace qgl::content::content_file_helpers
{
   /*
    Reads the file header from a content file and returns it.
    The file must be opened with read permissions.
    This does not validate the data read.
    */
   extern HRESULT load_header(const file_handle& hndl,
                              CONTENT_FILE_HEADER_BUFFER* out_p) noexcept;
   
   /*
    Reads the dictionary metadata from a content file at the offset.
    The offset is stored in the file's header.
    The file must be opened with read permissions.
    This does not validate the data read.
    */
   extern HRESULT load_dictionary_metadata(
      const file_handle& hndl,
      size_t dictionaryOffset,
      CONTENT_DICTIONARY_METADATA_BUFFER* out_p) noexcept;

   /*
    Reads a dictionary entry from a content file and returns it.
    The file must be opened with read permissions.
    This does not validate the data read.
    */
   extern HRESULT load_dictionary_entry(
      const file_handle& hndl,
      size_t entryOffset,
      CONTENT_DICTIONARY_ENTRY_BUFFER* out_p) noexcept;

   /*
    Writes a file header to a content file.
    The file must be opened with write permissions.
    */
   extern HRESULT write_header(const file_handle& hndl,
                            const CONTENT_FILE_HEADER_BUFFER& hdr) noexcept;

   /*
    Writes the dictionary metadata to a content file.
    The file must be opened with write permissions.
    */
   extern HRESULT write_dictionary_metadata(
      const file_handle& hndl,
      const CONTENT_DICTIONARY_METADATA_BUFFER& meta,
      size_t offset) noexcept;

   /*
    Writes a dictionary entry to the content file.
    The file must be opened with write permissions.
    */
   extern HRESULT write_dictionary_entry(
      const file_handle& hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
      size_t offset) noexcept;

   /*
    Writes the content data to the content file.
    The file must be opened with write permissions.
    */
   extern HRESULT write_content_data(
      const file_handle& hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
      const entries::data_content_entry& contentData) noexcept;

   /*
    Writes the shared content path to the content file.
    The file must be opened with write permissions.
    */
   extern HRESULT write_shared_data_path(
      const file_handle& hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
      const entries::shared_content_entry& path) noexcept;

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
   extern size_t shared_entry_data_size(const content_variant_entry* e);

   extern bool valid_content_file_size(const file_handle& hndl);
}
