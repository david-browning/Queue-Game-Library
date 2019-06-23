#include "pch.h"
#include "include/File-Loaders/qgl_file_loader_wstring.h"
#include "include/Content-Importers/qgl_import_helpers.h"
#include "include/Content-Importers/qgl_importer_wstring.h"
#include "include/Content-Items/qgl_wstring_item.h"

namespace qgl::content::loaders
{
   std::unique_ptr<wstring_item> qgl_load_wstring_file(
      const icontent_file* f,
      const id_t newID)
   {
      //Make sure the wstring file has the correct loader.
      auto headerInfo = f->const_header()->metadata();
      check_loader_and_resource<RESOURCE_TYPES::RESOURCE_TYPE_STRING,
         CONTENT_LOADER_IDS::CONTENT_LOADER_ID_WSTRING>(headerInfo);

       //WString files should only have 1 entry: the string's content.
      check_dictionary_count<1>(f);

      static wstring_importer wsImporter;

      auto entry = f->const_at(0);
      check_loader_and_resource<RESOURCE_TYPES::RESOURCE_TYPE_STRING,
         CONTENT_LOADER_IDS::CONTENT_LOADER_ID_WSTRING>(entry->metadata());

      auto str = wsImporter.load(*f->handle(), *entry);
      return std::make_unique<wstring_item>(str.c_str(),
                                            headerInfo->name(),
                                            newID);
   }
}