#include "pch.h"
#include "include/Content/File-Loaders/qgl_multisample_desc_file_loader.h"
#include "include/Content/Content-Loaders/qgl_multisample_desc_importer.h"

namespace qgl::content::loaders
{
   std::unique_ptr<multisample_desc> multisample_desc_file_loader::operator()(
      const icontent_file* f, 
      id_t newID) const
   {
      //Make sure the file has the correct loader.
      auto headerInfo = f->const_header()->metadata();
      check_loader_and_resource<RESOURCE_TYPE_DESCRIPTION,
         CONTENT_LOADER_ID_MULTISAMPLE_DESCRIPTION>(headerInfo);

      //Only 1 dictionary entry.
      const auto dictCount = f->size();
      if (dictCount != 1)
      {
         throw std::runtime_error("The content file should only have 1 entry.");
      }

      auto entry = f->const_at(0);
      check_loader_and_resource<RESOURCE_TYPE_DESCRIPTION,
         CONTENT_LOADER_ID_MULTISAMPLE_DESCRIPTION>(entry->metadata());

      static struct_importer<buffers::MULTISAMPLE_DESC_BUFFER,
         RESOURCE_TYPE_DESCRIPTION,
         CONTENT_LOADER_ID_MULTISAMPLE_DESCRIPTION> importer;

      auto buffer = importer.load(*f->handle(), *entry);
      return std::make_unique<multisample_desc>(&buffer,
                                                headerInfo->name(),
                                                newID);
   }
}