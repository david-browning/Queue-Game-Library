#include "pch.h"
#include "include/File-Loaders/qgl_file_loader_wstring.h"
#include "include/Content-Importers/qgl_import_helpers.h"
#include "include/Content-Importers/qgl_importer_wstring.h"

std::shared_ptr<std::wstring> qgl::content::wstring_file_loader::operator()(
   const content_file& f)
{
     //Make sure the wstring file has the correct loader.
   auto headerInfo = f.header()->metadata();
   check_loader_and_resource<RESOURCE_TYPES::RESOURCE_TYPE_STRING,
      CONTENT_LOADER_IDS::CONTENT_LOADER_ID_WSTRING>(headerInfo);

   //WString files should only have 1 entry: the string's content.
   const auto dictCount = f.size();
   if (dictCount != 1)
   {
      throw std::runtime_error("The content file should only have 1 entry.");
   }

   static wstring_importer wsImporter;

   auto& entry = f[0];
   check_loader_and_resource<RESOURCE_TYPES::RESOURCE_TYPE_STRING,
      CONTENT_LOADER_IDS::CONTENT_LOADER_ID_WSTRING>(entry.metadata());

   return std::make_shared<std::wstring>(wsImporter.load(f.handle(), entry));
}
