#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Importers/qgl_iimporter.h"
#include "include/qgl_file_helpers.h"

namespace qgl::content
{
   class wstring_importer : public ientry_importer<std::wstring>
   {
      public:
      using LoadT = std::wstring;
      constexpr wstring_importer() :
         ientry_importer(CONTENT_LOADER_IDS::CONTENT_LOADER_ID_WSTRING)
      {

      }

      virtual ~wstring_importer() noexcept
      {

      }

      virtual LoadT load(const winrt::file_handle& fileHandle,
                         const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup) const
      {
         const size_t numChars = lookup.size() / sizeof(wchar_t);
         wchar_t* newStr = new wchar_t[numChars + 1];
         newStr[numChars] = L'\0';

         read_file_sync<wchar_t>(fileHandle,
                                 lookup.size(),
                                 lookup.offset(),
                                 newStr);

         LoadT ret(newStr);
         delete[] newStr;
         return ret;
      }

      virtual CONTENT_DICTIONARY_ENTRY_BUFFER dict_entry(
         const LoadT& data,
         const winrt::hstring& objName,
         size_t offset = -1) const
      {
         static constexpr RESOURCE_TYPES rType =
            RESOURCE_TYPES::RESOURCE_TYPE_STRING;

         static CONTENT_METADATA_BUFFER info(rType, this->id(), objName);

         return
            CONTENT_DICTIONARY_ENTRY_BUFFER(data.size() * sizeof(wchar_t),
                                            info,
                                            offset);
      }
   };
}