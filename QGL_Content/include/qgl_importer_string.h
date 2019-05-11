#pragma once
#include "qgl_content_include.h"
#include "qgl_iimporter.h"
#include "qgl_file_helpers.h"

namespace qgl::content
{
   template<typename ContentCharT = char, typename IDT = content_id>
   class string_importer : public ientry_importer<ContentCharT, IDT, std::string>
   {
      public:
      using LoadT = std::string;
      constexpr string_importer() :
         ientry_importer(CONTENT_LOADER_IDS::CONTENT_LOADER_ID_STRING)
      {

      }

      virtual ~string_importer() noexcept
      {

      }

      virtual LoadT load(_IN_HANDLE_ const winrt::file_handle& fileHandle,
                           _IN_ const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup,
                           IDT newID = -1) const
      {
         //Add an extra byte for the NULL.
         char* newStr = new char[lookup.size() + 1];
         newStr[lookup.size()] = '\0';

         read_file_sync<char>(fileHandle, lookup.size(), lookup.offset(), newStr);
         std::string ret(newStr);
         delete[] newStr;

         return ret;
      }

      virtual const CONTENT_DICTIONARY_ENTRY_BUFFER dict_entry(const LoadT& data,
                                                               const winrt::hstring& objName,
                                                               size_t offset = -1) const
      {
         static constexpr RESOURCE_TYPES rType = RESOURCE_TYPES::RESOURCE_TYPE_STRING;
         static CONTENT_METADATA_BUFFER info(rType, this->id(), objName);
         return CONTENT_DICTIONARY_ENTRY_BUFFER(data.size(), info, offset);
      }
   };
}