#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/qgl_rasterizer.h"

namespace qgl::content::loaders
{
   class rasterizer_entry_load_functor
   {
      public:
      rasterizer operator()(
         const file_handle& fileHandle,
         const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup) const
      {
         if (lookup.size() != sizeof(buffers::RASTERIZER_BUFFER))
         {
            throw std::invalid_argument("The lookup size is not expected.");
         }

         buffers::RASTERIZER_BUFFER buff;
         winrt::check_hresult(read_file_sync(&fileHandle,
                                             sizeof(buffers::RASTERIZER_BUFFER),
                                             lookup.offset(),
                                             &buff));

         return rasterizer(&buff,
                           lookup.metadata()->name(),
                           INVALID_CONTENT_ID);
      }
   };

   class rasterizer_dict_export_functor
   {
      public:
      CONTENT_DICTIONARY_ENTRY_BUFFER operator()(
         const rasterizer& rstr,
         const wchar_t* name,
         size_t offset) const
      {
         CONTENT_METADATA_BUFFER info(RESOURCE_TYPE_RASTERIZER,
                                      CONTENT_LOADER_ID_RASTERIZER,
                                      name);

         return CONTENT_DICTIONARY_ENTRY_BUFFER(
            sizeof(buffers::RASTERIZER_BUFFER),
            &info,
            offset);
      }
   };

   using rasterizer_importer = entry_importer<
      rasterizer,
      rasterizer_entry_load_functor,
      rasterizer_dict_export_functor>;
}