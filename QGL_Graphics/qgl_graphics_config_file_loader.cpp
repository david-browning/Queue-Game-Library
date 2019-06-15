#include "pch.h"
#include "include/Content/FIle-Loaders/qgl_graphics_config_file_loader.h"
#include "include/Content/qgl_graphics_config.h"
#include "include/Content/Content-Buffers/qgl_graphic_config_buffer.h"
#include "include/Content/Content-Loaders/qgl_graphics_config_importer.h"

namespace qgl::content::loaders
{
   std::unique_ptr<graphics_config> qgl_load_graphics_config_file(
      const icontent_file* f,
      const id_t newID)
   {
      //Make sure the file has the correct info.
      auto headerInfo = f->const_header()->metadata();
      check_loader_and_resource<RESOURCE_TYPE_GRAPHICS_CONFIG,
         CONTENT_LOADER_ID_GRAPHICS_CONFIG>(headerInfo);

      //Graphics configs only have 1 entry: The description.
      const auto dictCount = f->size();
      if (dictCount != 1)
      {
         throw std::runtime_error("The content file should only have 1 entry.");
      }

      auto entry = f->const_at(0);
      check_loader_and_resource<RESOURCE_TYPE_GRAPHICS_CONFIG,
         CONTENT_LOADER_ID_STRUCT>(entry->metadata());

      static graphics_config_buffer_importer importer;

      auto buffer = importer.load(*f->handle(), *entry);
      return std::make_unique<graphics_config>(&buffer,
                                               headerInfo->name(),
                                               newID);
   }
}