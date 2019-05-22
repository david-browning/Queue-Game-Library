#include "pch.h"
#include "include/qgl_file_helpers.h"
#include "include/Content-Files/qgl_content_file_helpers.h"

using NumCharsType = uint16_t;

namespace qgl::content::content_file_helpers
{
   CONTENT_FILE_HEADER_BUFFER load_header(const winrt::file_handle& hndl)
   {
      CONTENT_FILE_HEADER_BUFFER ret;
      read_file_sync(hndl, sizeof(ret), 0, &ret);
      return ret;
   }

   CONTENT_DICTIONARY_METADATA_BUFFER load_dictionary_metadata(
      const winrt::file_handle& hndl,
      size_t dictionaryOffset)
   {
      CONTENT_DICTIONARY_METADATA_BUFFER ret;
      read_file_sync(hndl, sizeof(ret), dictionaryOffset, &ret);
      return ret;
   }

   CONTENT_DICTIONARY_ENTRY_BUFFER load_dictionary_entry(
      const winrt::file_handle& hndl,
      size_t entryOffset)
   {
      CONTENT_DICTIONARY_ENTRY_BUFFER ret;
      read_file_sync(hndl, sizeof(ret), entryOffset, &ret);
      return ret;
   }

   content_data_buffer_t load_content_data(
      const winrt::file_handle& hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry)
   {
      auto entrySize = entry.size();
      std::vector<uint8_t> ret;
      ret.resize(entrySize);
      read_file_sync(hndl, entrySize, entry.offset(), ret.data());
      return ret;
   }

   shared_content_data_buffer_t load_shared_data_path(
      const winrt::file_handle& hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry)
   {
      //First 8 bytes are the number of characters in the path.
      //Next bytes is the path. It is a wide string. Not null-terminated.
      NumCharsType numChars = 0;
      read_file_sync(hndl, sizeof(numChars), entry.offset(), &numChars);

      shared_content_data_buffer_t ret(numChars, L'\0');
      read_file_sync(hndl, 
                     numChars * sizeof(wchar_t), 
                     entry.offset() + sizeof(numChars),
                     ret.data());
      return ret;
   }

   void write_header(const winrt::file_handle& hndl,
                     const CONTENT_FILE_HEADER_BUFFER& hdr)
   {
      write_file_sync(hndl, sizeof(hdr), 0, &hdr);
   }

   void write_dictionary_metadata(
      const winrt::file_handle& hndl,
      const CONTENT_DICTIONARY_METADATA_BUFFER& meta,
      size_t offset)
   {
      write_file_sync(hndl, sizeof(meta), offset, &meta);
   }

   void write_dictionary_entry(const winrt::file_handle& hndl,
                               const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
                               size_t offset)
   {
      write_file_sync(hndl, sizeof(entry), offset, &entry);
   }

   void write_content_data(
      const winrt::file_handle& hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
      const content_data_buffer_t& contentData)
   {
      write_file_sync(hndl, entry.size(), entry.offset(), contentData.data());
   }

   void write_shared_data_path(const winrt::file_handle& hndl,
                               const CONTENT_DICTIONARY_ENTRY_BUFFER& entry, 
                               const shared_content_data_buffer_t& path)
   {
      //First 8 bytes are the number of characters in the path.
      //Next bytes is the path. It is a wide string. Not null-terminated.
      NumCharsType numChars = path.size();
      write_file_sync(hndl, sizeof(numChars), entry.offset(), &numChars);
      write_file_sync(hndl, 
                      sizeof(wchar_t) * numChars, 
                      entry.offset() + sizeof(numChars),
                      path.c_str());
   }

   size_t dictionary_data_offset(
      const CONTENT_FILE_HEADER_BUFFER& fileHeader,
      const CONTENT_DICTIONARY_METADATA_BUFFER& dictMeta)
   {
      return fileHeader.dictionary_offset() + sizeof(dictMeta) +
         (dictMeta.count() * dictMeta.entry_size());
   }

   size_t shared_entry_data_size(
      const shared_content_data_buffer_t& data)
   {
      return sizeof(NumCharsType) + (sizeof(wchar_t) * data.size());
   }

   bool valid_content_file_size(const winrt::file_handle& hndl)
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