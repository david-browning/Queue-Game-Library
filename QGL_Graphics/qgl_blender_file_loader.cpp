#include "pch.h"
#include "include/Content/qgl_blender.h"
#include "include/Content/Content-Loaders/qgl_blender_importer.h"
#include "include/Content/FIle-Loaders/qgl_blender_file_loader.h"

namespace qgl::content::loaders
{
   std::unique_ptr<blender> blender_file_loader::operator()(
      const icontent_file* f, 
      id_t newID) const
   {
      //Make sure the file has the correct loader.
      auto headerInfo = f->const_header()->metadata();
      check_loader_and_resource<RESOURCE_TYPE_BLENDER,
         CONTENT_LOADER_ID_BLENDER>(headerInfo);

      //Only 1 dictionary entry.
      const auto dictCount = f->size();
      if (dictCount != 1)
      {
         throw std::runtime_error("The content file should only have 1 entry.");
      }

      auto entry = f->const_at(0);
      check_loader_and_resource<RESOURCE_TYPE_BLENDER,
         CONTENT_LOADER_ID_STRUCT>(entry->metadata());

      
      static struct_importer<buffers::BLENDER_BUFFER,
         RESOURCE_TYPE_BLENDER,
         CONTENT_LOADER_ID_STRING> importer;

      auto buffer = importer.load(*f->handle(), *entry);
      return std::make_unique<blender>(&buffer,
                                       headerInfo->name(),
                                       newID);
   }
}