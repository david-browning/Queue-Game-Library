#include "pch.h"
#include "include/Content/File-Loaders/qgl_text_format_file_loader.h"
#include "include/Content/Content-Loaders/qgl_text_format_importer.h"

namespace qgl::content::loaders
{
   text_format_file_loader::text_format_file_loader(IDWriteFactory* factory_p) :
      m_factory_p(factory_p)
   {
   }

   std::unique_ptr<text_format> text_format_file_loader::operator()(
      const icontent_file* f, 
      const id_t newID)
   {
       //Make sure the file has the correct loader.
      auto headerInfo = f->const_header()->metadata();
      check_loader_and_resource<RESOURCE_TYPE_TEXT_FORMAT,
         CONTENT_LOADER_ID_TEXT_FORMAT>(headerInfo);

      //Only 1 dictionary entry.
      const auto dictCount = f->size();
      if (dictCount != 1)
      {
         throw std::runtime_error("The content file should only have 1 entry.");
      }

      auto entry = f->const_at(0);
      check_loader_and_resource<RESOURCE_TYPE_TEXT_FORMAT,
         CONTENT_LOADER_ID_STRUCT>(entry->metadata());

      static text_format_buffer_importer importer;
      auto buff = importer.load(*f->handle(),
                                *entry);

      return std::make_unique<text_format>(&buff,
                                           m_factory_p,
                                           headerInfo->name(),
                                           newID);
   }
}