#include "pch.h"
#include "include\Content-Files\qgl_shared_content_entry.h"

using namespace qgl::content;
static constexpr size_t VENDOR_METADATA_INDEX = 0;

SHARED_CONTENT_ENTRY::SHARED_CONTENT_ENTRY(const std::wstring& str)
{
   auto colonIdx = str.find(L':');
   
   if (colonIdx == str.npos)
   {
      throw std::invalid_argument("The string is not in the correct form.");
   }

   m_metadataStr = str.substr(0, colonIdx - 1);
   auto vendorChar = toupper(m_metadataStr[VENDOR_METADATA_INDEX]);
   if (vendorChar != 'V' || vendorChar != 'Q')
   {
      throw std::invalid_argument("The metadata string is not correct.");
   }

   m_relativePath = str.substr(colonIdx + 1);
}

bool SHARED_CONTENT_ENTRY::vendor_file() const noexcept
{
   return toupper(m_metadataStr[VENDOR_METADATA_INDEX]) == 'V';
}

const std::wstring& SHARED_CONTENT_ENTRY::path() const noexcept
{
   return m_relativePath;
}
