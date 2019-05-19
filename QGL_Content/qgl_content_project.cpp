#include "pch.h"
#include "include\Content-Project\qgl_content_project.h"
#include "include/qgl_file_helpers.h"

qgl::content::content_project::content_project(const winrt::hstring& filePath)
{
   auto exists = file_exists(filePath);
   m_handle = open_file_readwrite(filePath);
   if(exists)
   {
      read_in();
   }
}

void qgl::content::content_project::flush()
{
   size_t offset = 0;

   //Write the magic number
   write_file_sync(m_handle, sizeof(QGL_CONTENT_PROJECT_MAGIC_NUMBER),
                   offset, &QGL_CONTENT_PROJECT_MAGIC_NUMBER);
   offset += sizeof(QGL_CONTENT_PROJECT_MAGIC_NUMBER);

   //Write the metadata
   write_file_sync(m_handle, sizeof(m_hdr), offset, &m_hdr);
   offset += sizeof(m_hdr);

   //Write the number of entries.
   uint64_t numEntries = static_cast<uint64_t>(size());
   write_file_sync(m_handle, sizeof(numEntries), offset, &numEntries);
   offset += sizeof(numEntries);

   //For each entry:
   for (const auto& entry : *this)
   {
      //Write the separator.
      write_file_sync(m_handle,
                      sizeof(QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER),
                      offset,
                      &QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER);
      offset += sizeof(QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER);

      //Write the metadata
      write_file_sync(m_handle, sizeof(entry.first), offset,
                      &entry.first);
      offset += sizeof(entry.first);

      //Write the number of charcters in the path.
      uint64_t numChars = static_cast<uint64_t>(entry.second.size());
      write_file_sync(m_handle, numChars, offset, &numChars);
      offset += sizeof(numChars);

      //Write the path. Each character in the path is 2 bytes.
      write_file_sync(m_handle, numChars * sizeof(wchar_t), offset,
                      entry.second.c_str());
      offset += sizeof(wchar_t) * numChars;
   }
}

void qgl::content::content_project::read_in()
{
   size_t offset = 0;
   
   //Read the magic number and check it.
   uint64_t readMagicNumber = 0;
   read_file_sync(m_handle, sizeof(readMagicNumber), offset, &readMagicNumber);
   offset += sizeof(readMagicNumber);
   if (readMagicNumber != QGL_CONTENT_PROJECT_MAGIC_NUMBER)
   {
       
   }

   //Read the metadata
   read_file_sync(m_handle, sizeof(m_hdr), offset, &m_hdr);
   offset += sizeof(sizeof(m_hdr));

   //Read the number of entries. 8 bytes.
   uint64_t numEntries = 0;
   read_file_sync(m_handle, sizeof(numEntries), offset, &numEntries);
   offset += sizeof(numEntries);

   //For each entry to read:
   while (numEntries > 0)
   {
      //Read the magic number and check it.
      read_file_sync(m_handle, sizeof(readMagicNumber), offset,
                     &readMagicNumber);
      offset += sizeof(readMagicNumber);
      if (readMagicNumber != QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER)
      {

      }

      //Read the metadata
      CONTENT_METADATA_BUFFER meta;
      read_file_sync(m_handle, sizeof(meta), offset, &meta);
      offset += sizeof(meta);

      //Read the number of characters
      uint64_t numChars = 0;
      read_file_sync(m_handle, sizeof(numChars), offset, &numChars);
      offset += sizeof(numChars);

      //Read the path wstring. Need to read as a wstring because hstring does
      //not support resize.
      std::wstring path;
      path.resize(numChars);
      read_file_sync(m_handle, numChars * sizeof(wchar_t), 
                     offset, path.data());
      offset += sizeof(numChars * sizeof(wchar_t));

      //Emplace back a new entry.
      emplace_back(meta, winrt::hstring(path));

      numEntries--;
   }
}
