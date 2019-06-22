#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/qgl_multisample_desc.h"

namespace qgl::content::loaders
{
   using multisample_desc_buffer_importer = struct_importer<
      buffers::MULTISAMPLE_DESC_BUFFER,
      RESOURCE_TYPE_DESCRIPTION,
      CONTENT_LOADER_ID_MULTISAMPLE_DESCRIPTION>;

   class multisample_desc_load_functor
   {
      public:
      multisample_desc operator()(
         const file_handle& fileHandle,
         const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup) const
      {
         if (lookup.size() != sizeof(buffers::MULTISAMPLE_DESC_BUFFER))
         {
            throw std::invalid_argument("The lookup size is not expected.");
         }

         static multisample_desc_buffer_importer importer;
         auto buff = importer.load(fileHandle,
                                   lookup);

         return multisample_desc(&buff,
                                 lookup.metadata()->name(),
                                 INVALID_CONTENT_ID);
      }
   };

   class multisample_desc_dict_export_functor
   {
      public:
      CONTENT_DICTIONARY_ENTRY_BUFFER operator()(
         [[maybe_unused]] const multisample_desc& desc,
         const wchar_t* name,
         size_t offset)
      {
         CONTENT_METADATA_BUFFER info(RESOURCE_TYPE_DESCRIPTION,
                                      CONTENT_LOADER_ID_MULTISAMPLE_DESCRIPTION,
                                      name);

         return CONTENT_DICTIONARY_ENTRY_BUFFER(
            sizeof(buffers::MULTISAMPLE_DESC_BUFFER),
            &info,
            offset);
      }
   };

   using multisample_desc_importer = entry_importer<
      multisample_desc,
      multisample_desc_load_functor,
      multisample_desc_dict_export_functor>;
}