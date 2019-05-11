#include "pch.h"
#include "include/qgl_file_helpers.h"
#include "include/Content-Files/qgl_content_file_helpers.h"

namespace qgl::content
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

   content_dictionary load_dictionary(const winrt::file_handle& hndl,
                                      size_t dictionaryOffset)
   {
      content_dictionary ret(load_dictionary_metadata(hndl, dictionaryOffset));

      size_t entryOffset = dictionaryOffset +
         sizeof(CONTENT_DICTIONARY_METADATA_BUFFER);

      for (size_t i = 0; i < ret.size(); i++)
      {
         ret[i] = load_dictionary_entry(hndl, entryOffset);
         entryOffset += sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER);
      }

      return ret;
   }

   std::vector<uint8_t> load_content_data(
      const winrt::file_handle& hndl,
      const CONTENT_DICTIONARY_ENTRY_BUFFER& entry)
   {
      auto entrySize = entry.size();
      std::vector<uint8_t> ret;
      ret.resize(entrySize);
      read_file_sync(hndl, entrySize, entry.offset(), ret.data());
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

   void write_dictionary(const winrt::file_handle& hndl,
                         size_t dictionaryOffset,
                         const content_dictionary& dict)
   {
      //Write the dictionary metadata.
      write_dictionary_metadata(hndl, dict.buffer(), dictionaryOffset);

      //Write each entry.
      size_t entryOffset = dictionaryOffset +
         sizeof(CONTENT_DICTIONARY_METADATA_BUFFER);

      for (auto& it = dict.cbegin(); it != dict.cend(); ++it)
      {
         write_dictionary_entry(hndl, *it, entryOffset);
         entryOffset += sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER);
      }
   }

   void write_dictionary_entry_data(
      const winrt::file_handle& hndl,
      CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
      const void* contentData)
   {
      write_file_sync(hndl, entry.size(), entry.offset(), contentData);
   }

   size_t dictionary_data_offset(
      const CONTENT_FILE_HEADER_BUFFER& fileHeader,
      const CONTENT_DICTIONARY_METADATA_BUFFER& dictMeta)
   {
      return fileHeader.dictionary_offset() + sizeof(dictMeta) +
         (dictMeta.count() * dictMeta.entry_size());
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
   
   bool valid_content_file(const winrt::file_handle & hndl)
   {
      return true;
   }
}