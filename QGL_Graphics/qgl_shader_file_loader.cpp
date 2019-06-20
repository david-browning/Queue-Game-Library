#include "pch.h"
#include "include/Content/qgl_shader.h"
#include "include/Content/File-Loaders/qgl_shader_file_loader.h"
#include "include/Content/Content-Loaders/qgl_shader_importer.h"

namespace qgl::content::loaders
{
   std::unique_ptr<shader> shader_file_loader::operator()(
      const icontent_file* f,
      const id_t newID) const
   {
      //Make sure the file has the correct loader.
      auto headerInfo = f->const_header()->metadata();
      check_loader_and_resource<RESOURCE_TYPE_SHADER,
         CONTENT_LOADER_ID_UNKNOWN>(headerInfo);

      auto entry = f->const_at(0);
      check_resource_type<RESOURCE_TYPE_SHADER>(entry->metadata());
      if (entry->metadata()->loader_id() == CONTENT_LOADER_ID_SHADER_SOURCE)
      {
         throw std::invalid_argument(
            "No support for compiling shaders at run time.");
      }
      else if (entry->metadata()->loader_id() ==
               CONTENT_LOADER_ID_SHADER_COMPILED)
      {
         static shader_importer importer;
         return std::make_unique<shader>(importer.load(*f->handle(),
                                                       *entry));
      }
      else
      {
         throw std::runtime_error("The entry's loader ID is not correct.");
      }

      return nullptr;
   }
}