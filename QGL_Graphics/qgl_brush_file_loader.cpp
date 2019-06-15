#include "pch.h"
#include "include\Content\FIle-Loaders\qgl_brush_file_loader.h"
#include "include/Content/Content-Buffers/qgl_brush_buffer.h"
#include "include/Content/qgl_brush.h"
#include "include/Content/Content-Loaders/qgl_brush_importer.h"

namespace qgl::content::loaders
{
   brush_file_loader::brush_file_loader(graphics::d2d_context* devContext_p) :
      m_ptr(devContext_p)
   {
   }

   std::unique_ptr<brush> brush_file_loader::operator()(
      const icontent_file* f,
      const id_t newID)

   {
      //Make sure the file has the correct loader.
      auto headerInfo = f->const_header()->metadata();
      check_loader_and_resource<RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
         CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH>(headerInfo);

      //Brushes only have 1 entry: The description.
      const auto dictCount = f->size();
      if (dictCount != 1)
      {
         throw std::runtime_error("The content file should only have 1 entry.");
      }

      auto entry = f->const_at(0);
      check_loader_and_resource<RESOURCE_TYPE_BRUSH,
         CONTENT_LOADER_ID_STRUCT>(entry->metadata());

      static brush_buffer_importer importer;

      auto buffer = importer.load(*f->handle(), *entry);
      return std::make_unique<brush>(&buffer,
                                     m_ptr,
                                     headerInfo->name(),
                                     newID);

   }
}