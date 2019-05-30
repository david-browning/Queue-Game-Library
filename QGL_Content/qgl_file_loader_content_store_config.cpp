#include "pch.h"
#include "include/File-Loaders/qgl_file_loader_content_store_config.h"
#include "include/Content-Importers/qgl_import_helpers.h"
#include "include/Content-Importers/qgl_importer_wstring.h"

std::shared_ptr<qgl::content::content_store_config>
qgl::content::content_store_config_loader::operator()(const content_file& f)
{
   auto headerInfo = f.header()->metadata();
   check_loader_and_resource<RESOURCE_TYPES::RESOURCE_TYPE_DESCRIPTION,
      CONTENT_LOADER_IDS::CONTENT_LOADER_ID_STORE_CONFIG>(headerInfo);

   for (size_t i = 0; i < f.size(); i++)
   {
      const auto& entry = f[i];
      auto info = entry.metadata();
      switch (info->resource_type())
      {
         case RESOURCE_TYPES::RESOURCE_TYPE_STRING:
         {
            wstring_importer wsImporter;
            check_loader_id<CONTENT_LOADER_IDS::CONTENT_LOADER_ID_WSTRING>(
               info);
            return std::make_shared<content_store_config>(
               wsImporter.load(f.handle(), entry).c_str());
            break;
         }
         default:
         {
            check_resource_type<RESOURCE_TYPES::RESOURCE_TYPE_UNKNOWN>(info);
            break;
         }
      }
   }

   return nullptr;
}
