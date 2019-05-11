#include "pch.h"
#include "include/qgl_content_project.h"
#include "include/qgl_file_helpers.h"
#include "include/qgl_content_dictionary_buffer.h"

using FilePathLenT = uint32_t;

qgl::content::content_project::content_project() :
   m_entries(0)
{

}

void qgl::content::content_project::clear()
{
   m_entries.clear();
}

void qgl::content::content_project::emplace_back(
   const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
   const FileStringT& absPath)
{
   m_entries.emplace_back(entry, absPath);
}

void qgl::content::content_project::pop_back()
{
   m_entries.pop_back();
}

void qgl::content::content_project::emplace(
   const_iterator position,
   const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
   const FileStringT& absPath)
{
   m_entries.emplace(position, entry, absPath);
}

qgl::content::content_project::iterator qgl::content::content_project::erase(
   const_iterator position)
{
   return m_entries.erase(position);
}

qgl::content::content_project::iterator qgl::content::content_project::erase(
   const_iterator first,
   const_iterator last)
{
   return m_entries.erase(first, last);
}

qgl::content::content_project::iterator qgl::content::content_project::begin() noexcept
{
   return m_entries.begin();
}

qgl::content::content_project::const_iterator qgl::content::content_project::begin() const noexcept
{
   return m_entries.begin();
}

qgl::content::content_project::const_iterator qgl::content::content_project::cbegin() const noexcept
{
   return m_entries.cbegin();
}

qgl::content::content_project::iterator qgl::content::content_project::end() noexcept
{
   return m_entries.end();
}

qgl::content::content_project::const_iterator qgl::content::content_project::end() const noexcept
{
   return m_entries.end();
}

qgl::content::content_project::const_iterator qgl::content::content_project::cend() const noexcept
{
   return m_entries.cend();
}

qgl::content::content_project qgl::content::load_project(const winrt::hstring& absPath)
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

   auto fileHandle = open_file_readwrite(absPath, openParameters);

   size_t offset = 0;
   content_project ret;
   CONTENT_FILE_HEADER_BUFFER hdr;
   //Read the header.
   read_file_sync(fileHandle, sizeof(hdr), offset, &hdr);
   ret.header(hdr);
   offset += sizeof(hdr);

   //Read the number of dictionary entries.
   uint32_t numEntries = 0;
   read_file_sync<decltype(numEntries)>(fileHandle, sizeof(numEntries), offset, &numEntries);
   offset += sizeof(numEntries);

   for (uint32_t i = 0; i < numEntries; i++)
   {
      //Read the dictionary entry.
      CONTENT_DICTIONARY_ENTRY_BUFFER entry;
      static constexpr auto dictEntrySize = sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER);
      read_file_sync(fileHandle, dictEntrySize, offset, &entry);
      offset += dictEntrySize;

      //Read the number of characters in the path.
      FilePathLenT numChars = 0;
      read_file_sync(fileHandle, sizeof(numChars), offset, &numChars);
      offset += sizeof(numChars);

      //Allocate a string large enough for the path.
      std::wstring path(numChars, L'\0');

      //Read the path into the string.
      read_file_sync(fileHandle, numChars * sizeof(path.front()), offset, path.c_str());
      offset += numChars * sizeof(path.front());

      //Create an entry for the data we read in.
      ret.emplace_back(entry, winrt::hstring(path));
   }

   return ret;
}

void qgl::content::save_project(const qgl::content::content_project& proj,
                                const winrt::hstring& absPath)
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

   auto fileHandle = open_file_readwrite(absPath, openParameters);

   size_t offset = 0;

  //Truncate the file.
   truncate_file(fileHandle);

   //Write the header
   auto& hdr = proj.header();
   write_file_sync(fileHandle, sizeof(hdr), offset, &hdr);
   offset += sizeof(hdr);

   //Write the number of dictionary entries.
   auto numEntries = static_cast<uint32_t>(proj.count());
   write_file_sync(fileHandle, sizeof(numEntries), offset, &numEntries);
   offset += sizeof(numEntries);

   //For each dictionary entry:
   for (auto& entry : proj)
   {
      auto& dictEntry = entry.first;
      const auto dictEntrySize = sizeof(dictEntry);

      auto& path = entry.second;
      const auto filePathLen = static_cast<FilePathLenT>(path.size() + 1); //Add 1 for the null.
      const auto filePathSize = sizeof(path.front()) * filePathLen;

      //Write the dictionary entry.
      write_file_sync(fileHandle, dictEntrySize, offset, &dictEntry);
      offset += dictEntrySize;

      //Write the number of characters long the path is.
      write_file_sync(fileHandle, sizeof(filePathSize), offset, &filePathLen);
      offset += sizeof(filePathLen);

      //Write the file path.
      write_file_sync(fileHandle, filePathSize, offset, path.c_str());
      offset += filePathSize;
   }
}
