#include "pch.h"
#include "include/Content-Files/qgl_content_file.h"

using namespace qgl::content;

content_file::content_file(const winrt::hstring& filePath)
{
   bool existingFile = file_exists(filePath);
   m_handle = open_file_readwrite(filePath);

   if (existingFile)
   {
      check_and_throw_file_size();
      read_in();
   }
}

content_file::content_file(const winrt::Windows::Storage::StorageFile& f)
{
   bool existingFile = file_size(f) > 0;
   m_handle = open_file_readwrite(f);

   if (existingFile)
   {
      check_and_throw_file_size();
      read_in();
   }
}

void content_file::flush()
{
   //Write the header.
   content_file_helpers::write_header(m_handle, m_header);

   //Create a dictionary metadata buffer;
   CONTENT_DICTIONARY_METADATA_BUFFER dictMeta(
      m_entryDataToWrite.size(),
      sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER));

   //Write the dictionary metadata
   size_t dictMetaOffset = m_header.dictionary_offset();
   content_file_helpers::write_dictionary_metadata(m_handle,
                                                   dictMeta,
                                                   dictMetaOffset);

   //Offset to where to put the next dictionary entry.
   size_t dictEntryOffset = dictMetaOffset +
      sizeof(CONTENT_DICTIONARY_METADATA_BUFFER);

   //Offset to where to put the content data.
   auto contentDataOffset =
      content_file_helpers::dictionary_data_offset(m_header, dictMeta);

   for (auto& entry : m_entryDataToWrite)
   {
      //Write the dictionary entry
      auto& dictEntry = entry.first;
      content_file_helpers::write_dictionary_entry(m_handle,
                                                   dictEntry,
                                                   dictEntryOffset);

      //Write the content data
      if (dictEntry.shared())
      {
         auto& path = std::get<VARIANT_INDEX_PATH>(entry.second);
         content_file_helpers::write_shared_data_path(m_handle,
                                                      dictEntry,
                                                      path);
      }
      else
      {
         auto& buffer = std::get<VARIANT_INDEX_BUFFER>(entry.second);
         content_file_helpers::write_content_data(m_handle,
                                                  dictEntry,
                                                  buffer);
      }
   }
}

const CONTENT_DICTIONARY_ENTRY_BUFFER&content_file::at(size_t idx) const
{
   return m_entryDataToWrite.at(idx).first;
}

void content_file::push_back(const CONTENT_METADATA_BUFFER& meta,
                                const content_data_buffer_t& buff)
{
}

void content_file::push_back(const CONTENT_METADATA_BUFFER& meta,
                                const shared_content_data_buffer_t& buff)
{
}

const CONTENT_FILE_HEADER_BUFFER& content_file::header() const noexcept
{
   return m_header;
}

CONTENT_FILE_HEADER_BUFFER& content_file::header()
{
   return m_header;
}

size_t content_file::size() const noexcept
{
   return m_entryDataToWrite.size();
}

const winrt::file_handle& content_file::handle() const noexcept
{
   return m_handle;
}

void content_file::read_in()
{
}

void content_file::check_and_throw_file_size()
{
}
