#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/qgl_blender.h"

namespace qgl::content::loaders
{
   class blender_entry_load_functor
   {
      public:
      blender operator()(
         const file_handle& fileHandle,
         const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup) const
      {
         if (lookup.size() != sizeof(buffers::BLENDER_BUFFER))
         {
            throw std::invalid_argument("The lookup size is not expected.");
         }

         buffers::BLENDER_BUFFER buff;
         winrt::check_hresult(read_file_sync(&fileHandle,
                                             sizeof(buffers::BLENDER_BUFFER),
                                             lookup.offset(),
                                             &buff));

         return blender(&buff,
                        lookup.metadata()->name(),
                        INVALID_CONTENT_ID);
      }
   };

   class blender_dict_export_functor
   {
      public:
      CONTENT_DICTIONARY_ENTRY_BUFFER operator()(
         [[maybe_unused]] const blender& blndr,
         const wchar_t* name,
         size_t offset) const
      {
         CONTENT_METADATA_BUFFER info(RESOURCE_TYPE_BLENDER,
                                      CONTENT_LOADER_ID_BLENDER,
                                      name);

         return CONTENT_DICTIONARY_ENTRY_BUFFER(
            sizeof(buffers::BLENDER_BUFFER),
            &info,
            offset);
      }
   };

   using blender_importer = entry_importer<
      blender,
      blender_entry_load_functor,
      blender_dict_export_functor>;
}