#include "pch.h"
#include "include/qgl_file_helpers.h"
#include "include/Content-Files/qgl_content_file_helpers.h"

using NumCharsType = uint16_t;

namespace qgl::content::content_file_helpers
{
   HRESULT load_header(const file_handle& hndl,
                       CONTENT_FILE_HEADER_BUFFER* out_p) noexcept
   {
      return read_file_sync(&hndl,
                            sizeof(CONTENT_FILE_HEADER_BUFFER),
                            0,
                            out_p);
   }

   HRESULT load_dictionary_metadata(
      const file_handle& hndl,
      size_t dictionaryOffset,
      CONTENT_DICTIONARY_METADATA_BUFFER* out_p) noexcept
   {
      return read_file_sync(&hndl,
                            sizeof(CONTENT_DICTIONARY_METADATA_BUFFER),
                            dictionaryOffset,
                            out_p);
   }

   HRESULT load_dictionary_entry(
      const file_handle& hndl,
      size_t entryOffset,
      CONTENT_DICTIONARY_ENTRY_BUFFER* out_p) noexcept
   {
      return read_file_sync(&hndl,
                            sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER),
                            entryOffset,
                            out_p);
   }

   HRESULT write_header(
      const file_handle& hndl,
      const CONTENT_FILE_HEADER_BUFFER& hdr) noexcept
   {
      return write_file_sync(&hndl,
                             sizeof(hdr),
                             0,
                             &hdr);
   }

   HRESULT write_dictionary_metadata(
      const file_handle& hndl,
      const CONTENT_DICTIONARY_METADATA_BUFFER& meta,
      size_t offset) noexcept
   {
      return write_file_sync(&hndl,
                             sizeof(meta),
                             offset,
                             &meta);
   }

   HRESULT write_dictionary_entry(
      const file_handle& hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
      size_t offset) noexcept
   {
      return write_file_sync(&hndl,
                             sizeof(entry),
                             offset,
                             &entry);
   }

   HRESULT write_content_data(
      const file_handle& hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
      const entries::data_content_entry& contentData) noexcept
   {
      return write_file_sync(&hndl,
                             entry.size(),
                             entry.offset(),
                             contentData.data());
   }

   HRESULT write_shared_data_path(
      const file_handle& hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
      const entries::shared_content_entry& path) noexcept
   {
      //First 8 bytes are the number of characters in the path.
      //Next bytes is the path. It is a wide string. Not null-terminated.
      NumCharsType numChars = static_cast<NumCharsType>(path.size());
      auto hr = write_file_sync(&hndl,
                                sizeof(numChars),
                                entry.offset(),
                                &numChars);
      if (FAILED(hr))
      {
         return hr;
      }

      return write_file_sync(&hndl,
                             sizeof(wchar_t) * numChars,
                             entry.offset() + sizeof(numChars),
                             path.buffer());

   }

   size_t dictionary_data_offset(
      const CONTENT_FILE_HEADER_BUFFER& fileHeader,
      const CONTENT_DICTIONARY_METADATA_BUFFER& dictMeta)
   {
      return fileHeader.dictionary_offset() + sizeof(dictMeta) +
         (dictMeta.count() * dictMeta.entry_size());
   }

   size_t shared_entry_data_size(
      const content_variant_entry* e)
   {
      return sizeof(NumCharsType) + (sizeof(wchar_t) *
                                     e->shared_buffer().size());
   }

   bool valid_content_file_size(const file_handle& hndl)
   {
      //Get the file size
      size_t sz = 0;
      if (FAILED(file_size(&hndl, &sz)))
      {
         return false;
      }

      //If the size is less than expected, throw an exception.
      static constexpr size_t minValidFileSize =
         sizeof(CONTENT_FILE_HEADER_BUFFER) +
         sizeof(CONTENT_DICTIONARY_METADATA_BUFFER);

      return sz >= minValidFileSize;
   }
}