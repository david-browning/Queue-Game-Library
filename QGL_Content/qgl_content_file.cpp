#include "pch.h"
#include "include/Content-Files/qgl_content_file.h"
#include "include/Content-Files/qgl_content_file_helpers.h"
#include "include/qgl_file_helpers.h"

using namespace qgl::content;
using namespace content_file_helpers;

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
   write_dictionary_metadata(m_handle, dictMeta, dictMetaOffset);

   //Offset to where to put the next dictionary entry.
   size_t dictEntryOffset = dictMetaOffset +
      sizeof(CONTENT_DICTIONARY_METADATA_BUFFER);

   //Offset to where to put the content data.
   auto contentDataOffset = dictionary_data_offset(m_header, dictMeta);

   if (m_dict.size() != m_entryDataToWrite.size())
   {
      throw std::domain_error("There must be the same number of entries and "
                              "data.");
   }

   auto entryIt = m_dict.begin();
   auto contentIt = m_entryDataToWrite.begin();
   for (; entryIt != m_dict.end() && contentIt != m_entryDataToWrite.end();
        ++entryIt, ++contentIt)
   {
      entryIt->m_offset = contentDataOffset;
      write_dictionary_entry(m_handle, *entryIt, dictEntryOffset);

      if (entryIt->shared())
      {
         write_shared_data_path(m_handle,
                                *entryIt,
                                contentIt->shared_buffer());
      }
      else
      {
         write_content_data(m_handle,
                            *entryIt,
                            contentIt->buffer());
      }

      contentDataOffset += entryIt->size();
      dictEntryOffset += dictMeta.entry_size();
   }
}

void content_file::push_back(const CONTENT_METADATA_BUFFER& meta,
                             const DATA_CONTENT_ENTRY& buff)
{
   content_variant_entry cont(buff);
   CONTENT_DICTIONARY_ENTRY_BUFFER entry(buff.size(), meta);
   entry.shared(false);

   m_entryDataToWrite.push_back(std::move(cont));
   m_dict.push_back(std::move(entry));
}

void content_file::push_back(const CONTENT_METADATA_BUFFER& meta,
                             const SHARED_CONTENT_ENTRY& buff)
{
   content_variant_entry cont(buff);
   CONTENT_DICTIONARY_ENTRY_BUFFER entry(shared_entry_data_size(buff),
                                         meta);
   entry.shared(true);
   
   m_entryDataToWrite.push_back(std::move(cont));
   m_dict.push_back(std::move(entry));
}

void content_file::pop_back()
{
   m_dict.pop_back();
   m_entryDataToWrite.pop_back();
}

const CONTENT_DICTIONARY_ENTRY_BUFFER& content_file::front() const
{
   return m_dict.front();
}

const CONTENT_DICTIONARY_ENTRY_BUFFER& content_file::back() const
{
   return m_dict.back();
}

content_file::dictionary_iterator content_file::begin()
{
   return m_dict.begin();
}

content_file::const_dictionary_iterator content_file::begin() const
{
   return m_dict.begin();
}

content_file::const_dictionary_iterator content_file::cbegin() const
{
   return m_dict.cbegin();
}

content_file::dictionary_iterator content_file::end()
{
   return m_dict.end();
}

content_file::const_dictionary_iterator content_file::end() const
{
   return m_dict.end();
}

content_file::const_dictionary_iterator content_file::cend() const
{
   return m_dict.cend();
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

const file_handle& content_file::handle() const noexcept
{
   return m_handle;
}

void content_file::read_in()
{
   //Read the header
   m_header = load_header(m_handle);

   //Read the dictionary metadata
   auto dictMeta = load_dictionary_metadata(m_handle,
                                            m_header.dictionary_offset());

   size_t dictEntryOffset = m_header.dictionary_offset() +
      sizeof(CONTENT_DICTIONARY_METADATA_BUFFER);

   //For each entry in the dictionary:
   for (size_t i = 0; i < dictMeta.count(); i++)
   {
      auto dictEntry = load_dictionary_entry(m_handle, dictEntryOffset);
      m_dict.push_back(dictEntry);

      if (dictEntry.shared())
      {
         auto sharedContent = load_shared_data_path(m_handle, dictEntry);
         content_variant_entry cbt(sharedContent);
         m_entryDataToWrite.push_back(cbt);
      }
      else
      {
         auto content = load_content_data(m_handle, dictEntry);
         content_variant_entry cbt(content);
         m_entryDataToWrite.push_back(cbt);
      }

      dictEntryOffset += sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER);
   }
}

void content_file::check_and_throw_file_size()
{
   if (!valid_content_file_size(m_handle))
   {
      throw std::domain_error("The file is too small to be valid.");
   }
}
