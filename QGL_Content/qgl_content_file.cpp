#include "pch.h"
#include "include/qgl_file_helpers.h"
#include "include/qgl_content_file.h"
#include "include/qgl_content_project.h"
#include <variant>

//Opens a file for reading.
qgl::content::content_file::content_file(const winrt::hstring& filePath) :
   m_header(),
   m_entryDataToWrite(),
   m_dictionary(),
   m_handle(),
   m_writeMode(false)
{
   p_open_file_read(filePath);
   p_load_header();
   p_load_dictionary();
}

//Opens a new file for writing.
qgl::content::content_file::content_file(const winrt::hstring& objName,
                                         qgl_version_t comVersion,
                                         RESOURCE_TYPES resourceType,
                                         CONTENT_LOADER_IDS loaderType,
                                         const winrt::hstring& filePath) :
   m_header(CONTENT_INFO_BUFFER(resourceType, loaderType, objName)),
   m_handle(),
   m_entryDataToWrite(),
   m_dictionary(),
   m_writeMode(true)
{
   p_open_file_write(filePath);
   p_load_header();
   p_load_dictionary();
}

//Opens a new file for writing
qgl::content::content_file::content_file(const CONTENT_HEADER_BUFFER& headr,
                                         const winrt::hstring& filePath) :
   m_header(headr),
   m_handle(),
   m_entryDataToWrite(),
   m_dictionary(),
   m_writeMode(true)
{
   p_open_file_write(filePath);
}

qgl::content::content_file::~content_file()
{
   //File handle is automatically closed.
}

void qgl::content::content_file::flush()
{
   if (m_writeMode)
   {
      p_write_header();
      p_write_dictionary();
      p_write_dict_data();
   }
   else
   {
      throw std::bad_variant_access();
   }
}

void qgl::content::content_file::p_open_file_read(const winrt::hstring & filePath)
{
   CREATEFILE2_EXTENDED_PARAMETERS openParameters;
   openParameters.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
   openParameters.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
   openParameters.dwFileFlags = FILE_FLAG_OVERLAPPED | FILE_FLAG_SEQUENTIAL_SCAN;
   openParameters.dwSecurityQosFlags = SECURITY_ANONYMOUS;
   openParameters.hTemplateFile = nullptr;
   openParameters.lpSecurityAttributes = nullptr;

   m_handle = open_file_read(filePath, openParameters);

   if (!m_handle)
   {
      winrt::throw_last_error();
   }
}

void qgl::content::content_file::p_open_file_write(const winrt::hstring & filePath)
{
   CREATEFILE2_EXTENDED_PARAMETERS openParameters = { 0 };
   openParameters.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
   openParameters.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
   openParameters.dwFileFlags = FILE_FLAG_OVERLAPPED | FILE_FLAG_SEQUENTIAL_SCAN;
   openParameters.dwSecurityQosFlags = SECURITY_ANONYMOUS;
   openParameters.hTemplateFile = nullptr;
   SECURITY_ATTRIBUTES sa;
   sa.bInheritHandle = TRUE;
   sa.nLength = sizeof(sa);
   sa.lpSecurityDescriptor = nullptr;
   openParameters.lpSecurityAttributes = &sa;

   m_handle = open_file_write(filePath, openParameters);

   if (!m_handle)
   {
      winrt::throw_last_error();
   }
}

void qgl::content::content_file::p_load_header()
{
   read_file_sync(m_handle, sizeof(m_header), 0, &m_header);
}

void qgl::content::content_file::p_load_dictionary()
{
   //Read the dictionary header
   size_t offset = sizeof(m_header);
   CONTENT_DICTIONARY_BUFFER dictHeader;
   read_file_sync<CONTENT_DICTIONARY_BUFFER>(m_handle, sizeof(CONTENT_DICTIONARY_BUFFER), offset, &dictHeader);

   //Create the dictionary.
   m_dictionary = content_dictionary(dictHeader);

   //Read each dictionary entry into the array.
   offset += sizeof(dictHeader);
   CONTENT_DICTIONARY_ENTRY_BUFFER entry;
   for (uint64_t i = 0; i < dictHeader.count(); i++)
   {
      //Read the entry
      read_file_sync<CONTENT_DICTIONARY_ENTRY_BUFFER>(m_handle, sizeof(entry), offset, &entry);

      //Store the entry in the dictionary
      m_dictionary.at(i) = entry;

      //Update the offset.
      offset += sizeof(entry);
   }
}

void qgl::content::content_file::p_write_header()
{
   constexpr size_t headerOffset = 0;
   constexpr size_t headerSize = sizeof(m_header);
   write_file_sync(m_handle, headerSize, headerOffset, &m_header);
}

void qgl::content::content_file::p_write_dictionary()
{
   //Write the dictionary header.
   constexpr size_t dictHeaderOffset = sizeof(CONTENT_HEADER_BUFFER);
   constexpr size_t dictHeaderSize = sizeof(CONTENT_DICTIONARY_BUFFER);
   write_file_sync<CONTENT_DICTIONARY_BUFFER>(m_handle,
                                              dictHeaderSize,
                                              dictHeaderOffset,
                                              &m_dictionary.buffer());

   //Offset to where to write the dictionary entry.
   size_t entryOffset = dictHeaderOffset + dictHeaderSize;

   //Offset to where the dictionary data is.
   const auto dictCount = count();
   size_t dataOffset = entryOffset + (sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER) * dictCount);

   //Write all the dictionary entries.
   for (auto it = m_dictionary.begin(); it != m_dictionary.end(); ++it)
   {
      //Update the entry's offset before writing the entry.
      it->offset(dataOffset);

      write_file_sync<CONTENT_DICTIONARY_ENTRY_BUFFER>(m_handle,
                                                       sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER),
                                                       entryOffset,
                                                       &*it);

      entryOffset += sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER);
      dataOffset += it->size();
   }
}

void qgl::content::content_file::p_write_dict_data()
{
   size_t i = 0;
   for (auto it = m_dictionary.cbegin(); it != m_dictionary.cend(); ++it, i++)
   {
      write_file_sync(m_handle, it->size(), it->offset(), m_entryDataToWrite.at(i));
   }
}
