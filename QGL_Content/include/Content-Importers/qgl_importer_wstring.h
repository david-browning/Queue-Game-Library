#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Importers/qgl_iimporter.h"
#include "include/qgl_file_helpers.h"

namespace qgl::content
{
   struct wstring_load_entry_fn
   {
      std::wstring operator()(
         const file_handle& fileHandle,
                              const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup) const
      {
         auto numChars = lookup.size() / sizeof(wchar_t);
         std::wstring ret(numChars, L'\0');
         read_file_sync<wchar_t>(&fileHandle,
                                 lookup.size(),
                                 lookup.offset(),
                                 ret.data());
         return ret;
      }
   };

   struct wstring_dict_export_fn
   {
      CONTENT_DICTIONARY_ENTRY_BUFFER operator()(const std::wstring& data,
                                                 const wchar_t* objName,
                                                 size_t offset) const
      {
         static constexpr RESOURCE_TYPES rType =
            RESOURCE_TYPES::RESOURCE_TYPE_STRING;
         static CONTENT_METADATA_BUFFER info(rType,
                                             CONTENT_LOADER_ID_WSTRING,
                                             objName);

         return CONTENT_DICTIONARY_ENTRY_BUFFER(data.size(), &info, offset);
      }
   };

   using wstring_importer = ientry_importer<
      std::wstring,
      CONTENT_LOADER_ID_WSTRING,
      wstring_load_entry_fn,
      wstring_dict_export_fn>;
}