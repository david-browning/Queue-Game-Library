#include "pch.h"
#include "include/qgl_file_helpers.h"
#include "include/Content-Files/qgl_content_file_helpers.h"

using NumCharsType = uint16_t;

namespace qgl::content::content_file_helpers
{
   CONTENT_FILE_HEADER_BUFFER load_header(const file_handle* hndl)
   {
      CONTENT_FILE_HEADER_BUFFER ret;
      read_file_sync(hndl, sizeof(ret), 0, &ret);
      return ret;
   }

   CONTENT_DICTIONARY_METADATA_BUFFER load_dictionary_metadata(
      const file_handle* hndl,
      size_t dictionaryOffset)
   {
      CONTENT_DICTIONARY_METADATA_BUFFER ret;
      read_file_sync(hndl, sizeof(ret), dictionaryOffset, &ret);
      return ret;
   }

   CONTENT_DICTIONARY_ENTRY_BUFFER load_dictionary_entry(
      const file_handle* hndl,
      size_t entryOffset)
   {
      CONTENT_DICTIONARY_ENTRY_BUFFER ret;
      read_file_sync(hndl, sizeof(ret), entryOffset, &ret);
      return ret;
   }

   DATA_CONTENT_ENTRY load_content_data(
      const file_handle* hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry)
   {
      auto entrySize = entry.size();
      std::vector<uint8_t> ret;
      ret.resize(entrySize);
      read_file_sync(hndl, entrySize, entry.offset(), ret.data());
      return ret;
   }

   SHARED_CONTENT_ENTRY load_shared_data_path(
      const file_handle* hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry)
   {
      //First 8 bytes are the number of characters in the path.
      //Next bytes is the path. It is a wide string. Not null-terminated.
      NumCharsType numChars = 0;
      read_file_sync(hndl, sizeof(numChars), entry.offset(), &numChars);

      std::wstring path(numChars, L'\0');
      read_file_sync(hndl,
                     numChars * sizeof(wchar_t),
                     entry.offset() + sizeof(numChars),
                     path.data());
      return SHARED_CONTENT_ENTRY(path.c_str());
   }

   void write_header(const file_handle* hndl,
                     const CONTENT_FILE_HEADER_BUFFER& hdr)
   {
      write_file_sync(hndl, sizeof(hdr), 0, &hdr);
   }

   void write_dictionary_metadata(
      const file_handle* hndl,
      const CONTENT_DICTIONARY_METADATA_BUFFER& meta,
      size_t offset)
   {
      write_file_sync(hndl, sizeof(meta), offset, &meta);
   }

   void write_dictionary_entry(const file_handle* hndl,
                               const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
                               size_t offset)
   {
      write_file_sync(hndl, sizeof(entry), offset, &entry);
   }

   void write_content_data(const file_handle* hndl,
                           const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
                           const DATA_CONTENT_ENTRY& contentData)
   {
      write_file_sync(hndl, entry.size(), entry.offset(), contentData.data());
   }

   void write_shared_data_path(const file_handle* hndl,
                               const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
                               const SHARED_CONTENT_ENTRY& path)
   {
      //First 8 bytes are the number of characters in the path.
      //Next bytes is the path. It is a wide string. Not null-terminated.
      NumCharsType numChars = static_cast<NumCharsType>(path.size());
      write_file_sync(hndl, sizeof(numChars), entry.offset(), &numChars);
      write_file_sync(hndl,
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

   bool valid_content_file_size(const file_handle* hndl)
   {
      //Get the file size
      auto sz = file_size(hndl);

      //If the size is less than expected, throw an exception.
      static constexpr size_t minValidFileSize =
         sizeof(CONTENT_FILE_HEADER_BUFFER) +
         sizeof(CONTENT_DICTIONARY_METADATA_BUFFER);

      return sz >= minValidFileSize;
   }
}