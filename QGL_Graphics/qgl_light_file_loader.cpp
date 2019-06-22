#include "pch.h"
#include "include/Content/File-Loaders/qgl_light_file_loader.h"
#include "include/Content/Content-Loaders/qgl_light_importer.h"
#include "include/Content/qgl_light.h"

namespace qgl::content::loaders
{
   light_file_loader::light_file_loader(graphics::d3d_device* dev_p) :
      m_dev_p(dev_p)
   {
   }

   std::unique_ptr<light> light_file_loader::operator()(const icontent_file* f,
                                                        const id_t newID)
   {
      //Make sure the file has the correct loader.
      auto headerInfo = f->const_header()->metadata();
      check_loader_and_resource<RESOURCE_TYPE_LIGHT,
         CONTENT_LOADER_ID_LIGHT>(headerInfo);

      //Only 1 dictionary entry.
      const auto dictCount = f->size();
      if (dictCount != 1)
      {
         throw std::runtime_error("The content file should only have 1 entry.");
      }

      auto entry = f->const_at(0);
      check_loader_and_resource<RESOURCE_TYPE_LIGHT,
         CONTENT_LOADER_ID_STRUCT>(entry->metadata());

      static light_buffer_importer importer;
      auto buff = importer.load(*f->handle(),
                                *entry);

      return std::make_unique<light>(&buff,
                                     m_dev_p,
                                     headerInfo->name(),
                                     newID);
   }
}