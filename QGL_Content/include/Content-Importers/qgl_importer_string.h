#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Importers/qgl_iimporter.h"
#include "include/qgl_file_helpers.h"

namespace qgl::content::loaders
{
   class string_load_entry_fn
   {
      public:
      std::string operator()(
         const file_handle& fileHandle,
         const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup) const
      {
         std::string ret(lookup.size(), '\0');
         read_file_sync<char>(&fileHandle,
                              lookup.size(),
                              lookup.offset(),
                              ret.data());
         return ret;
      }
   };

   class string_dict_export_fn
   {
      public:
      CONTENT_DICTIONARY_ENTRY_BUFFER operator()(const std::string& data,
                                                 const wchar_t* objName,
                                                 size_t offset) const
      {
         CONTENT_METADATA_BUFFER info(RESOURCE_TYPE_STRING,
                                      CONTENT_LOADER_ID_STRING,
                                      objName);

         return CONTENT_DICTIONARY_ENTRY_BUFFER(data.size(), &info, offset);
      }
   };


   using string_importer = ientry_importer<
      std::string,
      CONTENT_LOADER_ID_STRING,
      string_load_entry_fn,
      string_dict_export_fn>;
}