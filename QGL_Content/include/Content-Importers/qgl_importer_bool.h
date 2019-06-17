#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Importers/qgl_iimporter.h"
#include "include/qgl_file_helpers.h"

namespace qgl::content::loaders
{
   using bool_data_t = uint64_t;

   class bool_load_entry_fn
   {
      public:
      bool operator()(const file_handle& fileHandle,
                      const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup) const
      {
         bool_data_t ret = false;

         if (sizeof(ret) != lookup.size())
         {
            throw std::invalid_argument("The boolean is not the same size as "
                                        "the dictionary entry excepts.");
         }

         winrt::check_hresult(read_file_sync(&fileHandle,
                                             lookup.size(),
                                             lookup.offset(),
                                             &ret));

         return ret != 0;
      }
   };

   class bool_dict_export_fn
   {
      public:
      CONTENT_DICTIONARY_ENTRY_BUFFER operator()(const bool& integer,
                                                 const wchar_t* name,
                                                 size_t offset)
      {
         CONTENT_METADATA_BUFFER info(RESOURCE_TYPE_BOOL,
                                      CONTENT_LOADER_ID_BOOL,
                                      name);

         return CONTENT_DICTIONARY_ENTRY_BUFFER(sizeof(bool_data_t),
                                                &info, offset);
      }
   };

   using bool_importer = ientry_importer<
      bool,
      bool_load_entry_fn,
      bool_dict_export_fn>;
}