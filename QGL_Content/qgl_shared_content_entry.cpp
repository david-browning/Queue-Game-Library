#include "pch.h"
#include "include\Content-Files\qgl_shared_content_entry.h"

namespace qgl::content
{
   static constexpr size_t VENDOR_METADATA_INDEX = 0;

   SHARED_CONTENT_ENTRY::SHARED_CONTENT_ENTRY(const std::wstring& str) :
      SHARED_CONTENT_ENTRY(str.data(), str.size())
   {
   }

   SHARED_CONTENT_ENTRY::SHARED_CONTENT_ENTRY(const wchar_t* str, size_t len)
   {
      m_bufferSize = len;
      m_buffer = new wchar_t[m_bufferSize + 1];
      MemoryCopy(m_buffer, str, m_bufferSize);
      m_buffer[m_bufferSize] = L'\0';
      
      size_t colonIdx = MemoryIndex(m_buffer, L':', m_bufferSize);
      if (colonIdx == static_cast<size_t>(-1))
      {
         throw std::invalid_argument("The string is not in the correct form.");
      }

      m_metadataStr = new wchar_t[colonIdx + 1];
      MemoryCopy(m_metadataStr, m_buffer, colonIdx);
      m_metadataStr[colonIdx] = L'\0';
      auto vendorChar = toupper(m_metadataStr[VENDOR_METADATA_INDEX]);
      if (vendorChar != 'V' && vendorChar != 'Q')
      {
         throw std::invalid_argument("The metadata string is not correct.");
      }

      auto relativePathLen = m_bufferSize - colonIdx;
      m_relativePath = new wchar_t[relativePathLen + 1];
      MemoryCopy(m_relativePath, str + colonIdx + 1, relativePathLen);
      m_relativePath[relativePathLen] = L'\0';
   }

   SHARED_CONTENT_ENTRY::SHARED_CONTENT_ENTRY(const SHARED_CONTENT_ENTRY& c)
   {
      m_bufferSize = c.m_bufferSize;
      
      //Copy the entire buffer.
      m_buffer = new wchar_t[m_bufferSize + 1];
      MemoryCopy(m_buffer, c.m_buffer, m_bufferSize);
      m_buffer[m_bufferSize] = L'\0';

      //Copy the metadata string
      size_t colonIdx = MemoryIndex(m_buffer, L':', m_bufferSize);
      m_metadataStr = new wchar_t[colonIdx + 1];
      MemoryCopy(m_metadataStr, c.m_metadataStr, colonIdx);
      m_metadataStr[colonIdx] = L'\0';

      //Copy the relative path string
      auto relativePathLen = m_bufferSize - colonIdx;
      m_relativePath = new wchar_t[relativePathLen + 1];
      MemoryCopy(m_relativePath, c.m_relativePath, relativePathLen);
      m_relativePath[relativePathLen] = L'\0';
   }

   SHARED_CONTENT_ENTRY::SHARED_CONTENT_ENTRY(SHARED_CONTENT_ENTRY&& m)
   {
      m_bufferSize = m.m_bufferSize;
      m_buffer = m.m_buffer;
      m_relativePath = m.m_relativePath;
      m_metadataStr = m.m_metadataStr;

      m.m_metadataStr = m.m_relativePath = m.m_buffer = nullptr;

      #ifdef DEBUG
      m.m_bufferSize = static_cast<size_t>(-1);
      #endif
   }

   SHARED_CONTENT_ENTRY::~SHARED_CONTENT_ENTRY() noexcept
   {
      delete[] m_buffer;
      delete[] m_metadataStr;
      delete[] m_relativePath;

      #ifdef DEBUG
      m_buffer = m_relativePath = m_metadataStr = nullptr;
      m_bufferSize = static_cast<size_t>(-1);
      #endif
   }

   bool SHARED_CONTENT_ENTRY::vendor_file() const noexcept
   {
      return toupper(m_metadataStr[VENDOR_METADATA_INDEX]) == 'V';
   }

   size_t qgl::content::SHARED_CONTENT_ENTRY::size() const noexcept
   {
      return m_bufferSize;
   }

   const void * qgl::content::SHARED_CONTENT_ENTRY::data() const
   {
      return m_buffer;
   }

   const wchar_t* SHARED_CONTENT_ENTRY::path() const noexcept
   {
      return m_relativePath;
   }
}