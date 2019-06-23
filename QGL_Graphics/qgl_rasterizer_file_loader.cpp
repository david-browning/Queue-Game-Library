#include "pch.h"
#include "include/Content/qgl_rasterizer.h"
#include "include/Content/File-Loaders/qgl_rasterizer_file_loader.h"
#include "include/Content/Content-Loaders/qgl_rasterizer_importer.h"

namespace qgl::content::loaders
{
   std::unique_ptr<rasterizer> qgl_load_rasterizer_file(const icontent_file* f,
                                                        const id_t newID)
   {
      //Make sure the file has the correct loader.
      auto headerInfo = f->const_header()->metadata();
      check_loader_and_resource<RESOURCE_TYPE_RASTERIZER,
         CONTENT_LOADER_ID_RASTERIZER>(headerInfo);

      //Only 1 dictionary entry.
      check_dictionary_count<1>(f);

      auto entry = f->const_at(0);
      check_loader_and_resource<RESOURCE_TYPE_RASTERIZER,
         CONTENT_LOADER_ID_RASTERIZER>(entry->metadata());

      static rasterizer_buffer_importer importer;
      auto buff = importer.load(*f->handle(),
                                *entry);

      return std::make_unique<rasterizer>(&buff,
                                          headerInfo->name(),
                                          newID);
   }
}