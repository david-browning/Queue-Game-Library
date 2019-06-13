#include "pch.h"
#include "include\Content-Files\qgl_shared_content_entry.h"

namespace qgl::content::entries
{
   static constexpr size_t VENDOR_METADATA_INDEX = 0;

   constexpr shared_content_entry::shared_content_entry()
   {
   }

   shared_content_entry::shared_content_entry(const wchar_t* str) :
      m_completeStr(str)
   {
      auto colonIdx = m_completeStr.find(L':');
      if (colonIdx == std::wstring::npos)
      {
         throw std::invalid_argument("The string is not in the correct form.");
      }

      m_metadataStr = m_completeStr.substr(0, colonIdx);

      auto vendorChar = toupper(m_metadataStr[VENDOR_METADATA_INDEX]);
      if (vendorChar != 'V' && vendorChar != 'Q')
      {
         throw std::invalid_argument("The metadata string is not correct.");
      }

      m_relativeStr = m_completeStr.substr(colonIdx + 1);
   }

   bool shared_content_entry::vendor_file() const noexcept
   {
      return toupper(m_metadataStr[VENDOR_METADATA_INDEX]) == 'V';
   }

   size_t shared_content_entry::size() const noexcept
   {
      return m_completeStr.size();
   }

   const wchar_t* shared_content_entry::buffer() const noexcept
   {
      return m_completeStr.c_str();
   }

   const wchar_t* shared_content_entry::relative_path() const noexcept
   {
      return m_relativeStr.c_str();
   }
}