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

   HRESULT load_content_data(const file_handle& hndl,
                             const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
                             DATA_CONTENT_ENTRY* out_p) noexcept
   {
      auto entrySize = entry.size();
      std::vector<uint8_t> ret;
      ret.resize(entrySize);
      auto hr = read_file_sync(&hndl,
                               entrySize,
                               entry.offset(),
                               ret.data());
      if (FAILED(hr))
      {
         return hr;
      }

      *out_p = DATA_CONTENT_ENTRY(ret);
      return S_OK;
   }

   HRESULT load_shared_data_path(const file_handle& hndl,
                                 const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
                                 SHARED_CONTENT_ENTRY* out_p) noexcept
   {
      //First 8 bytes are the number of characters in the path.
      //Next bytes is the path. It is a wide string. Not null-terminated.
      NumCharsType numChars = 0;
      auto hr = read_file_sync(&hndl,
                               sizeof(numChars),
                               entry.offset(),
                               &numChars);
      if (FAILED(hr))
      {
         return hr;
      }

      std::wstring path(numChars, L'\0');
      hr = read_file_sync(&hndl,
                          numChars * sizeof(wchar_t),
                          entry.offset() + sizeof(numChars),
                          path.data());
      if (FAILED(hr))
      {
         return hr;
      }

      *out_p = SHARED_CONTENT_ENTRY(path.c_str());

      return S_OK;
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
      const DATA_CONTENT_ENTRY& contentData) noexcept
   {
      return write_file_sync(&hndl,
                             entry.size(),
                             entry.offset(),
                             contentData.data());
   }

   HRESULT write_shared_data_path(const file_handle& hndl,
                                  const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
                                  const SHARED_CONTENT_ENTRY& path) noexcept
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
                             path.data());

   }

   size_t dictionary_data_offset(
      const CONTENT_FILE_HEADER_BUFFER& fileHeader,
      const CONTENT_DICTIONARY_METADATA_BUFFER& dictMeta)
   {
      return fileHeader.dictionary_offset() + sizeof(dictMeta) +
         (dictMeta.count() * dictMeta.entry_size());
   }

   size_t shared_entry_data_size(
      const SHARED_CONTENT_ENTRY* data)
   {
      return sizeof(NumCharsType) + (sizeof(wchar_t) * data->size());
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