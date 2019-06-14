#include "pch.h"
#include "include/File-Loaders/qgl_file_loader_string.h"
#include "include/Content-Importers/qgl_import_helpers.h"
#include "include/Content-Importers/qgl_importer_string.h"
#include "include/Content-Items/qgl_string_item.h"


namespace qgl::content::loaders
{
   std::unique_ptr<string_item> qgl_load_string_file(const icontent_file* f,
                                                     const id_t newID)
   {
      //Make sure the string file has the correct loader.
      auto headerInfo = f->const_header()->metadata();
      check_loader_and_resource<RESOURCE_TYPES::RESOURCE_TYPE_STRING,
         CONTENT_LOADER_IDS::CONTENT_LOADER_ID_STRING>(headerInfo);

      //String files should only have 1 entry: the string's content.
      const auto dictCount = f->size();
      if (dictCount != 1)
      {
         throw std::runtime_error("The content file should only have 1 entry.");
      }

      static string_importer sImporter;
      auto entry = f->const_at(0);
      check_loader_and_resource<RESOURCE_TYPES::RESOURCE_TYPE_STRING,
         CONTENT_LOADER_IDS::CONTENT_LOADER_ID_STRING>(entry->metadata());

      auto str = sImporter.load(*f->handle(), *entry);
      return std::make_unique<string_item>(str.c_str(),
                                           headerInfo->name(),
                                           newID);
   }
}