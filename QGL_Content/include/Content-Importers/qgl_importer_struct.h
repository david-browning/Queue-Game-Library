#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Importers/qgl_iimporter.h"
#include "include/qgl_file_helpers.h"

namespace qgl::content
{
   template<typename StructT>
   struct struct_load_entry_fn
   {
      static_assert(std::is_pod<StructT>::value,
                    "Parameter 'StructType' must be plain old data.");

      StructT operator()(const file_handle& fileHandle,
                         const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup) const
      {
         StructT ret;
         read_file_sync<StructT>(&fileHandle,
                                 lookup.size(),
                                 lookup.offset(),
                                 &ret);
         return ret;
      }
   };

   template<
      typename StructT,
      RESOURCE_TYPES ResourceType,
      CONTENT_LOADER_IDS LoaderID>
   struct struct_dict_export_fn
   {
      static_assert(std::is_pod<StructT>::value,
                    "Parameter 'StructType' must be plain old data.");

      CONTENT_DICTIONARY_ENTRY_BUFFER operator()(const StructT& data,
                                                 const wchar_t* objName,
                                                 size_t offset) const
      {
         static CONTENT_METADATA_BUFFER info(ResourceType, LoaderID, objName);

         return CONTENT_DICTIONARY_ENTRY_BUFFER(sizeof(StructT),
                                                &info,
                                                offset);
      }
   };
}