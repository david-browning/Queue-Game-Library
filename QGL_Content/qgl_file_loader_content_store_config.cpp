#include "pch.h"
#include "include/Content-Importers/qgl_file_loader_content_store_config.h"
#include "include/Content-Importers/qgl_importer_testers.h"
#include "include/Content-Importers/qgl_importer_wstring.h"

std::shared_ptr<qgl::content::content_store_config>
qgl::content::content_store_config_loader::operator()(const content_file & f)
{
   auto& headerInfo = f.header().info();
   check_loader_and_resource<RESOURCE_TYPES::RESOURCE_TYPE_DESCRIPTION,
      CONTENT_LOADER_IDS::CONTENT_LOADER_ID_STORE_CONFIG>(headerInfo);

   for (size_t i = 0; i < f.count(); i++)
   {
      auto& entry = f.at(i);
      auto& info = entry.metadata();
      switch (info.resource_type())
      {
         case RESOURCE_TYPES::RESOURCE_TYPE_STRING:
         {
            wstring_importer wsImporter;
            check_loader_id<CONTENT_LOADER_IDS::CONTENT_LOADER_ID_WSTRING>(
               info);
            return std::make_shared<content_store_config>(
               winrt::hstring(wsImporter.load(f.handle(), entry)));
            break;
         }
         default:
         {
            check_resource_type<RESOURCE_TYPES::RESOURCE_TYPE_UNKNOWN>(
               info);
            break;
         }
      }
   }

   return nullptr;
}
