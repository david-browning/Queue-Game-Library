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

      //Only 1 dictionary entry.
      check_dictionary_count<1>(f);

      auto entry = f->const_at(0);
      check_resource_type<RESOURCE_TYPE_SHADER>(entry->metadata());

      //Read the shader header.
      buffers::SHADER_HEADER_BUFFER hdr;
      winrt::check_hresult(read_file_sync(f->handle(),
                                          sizeof(hdr),
                                          entry->offset(),
                                          &hdr));

      //Allocate memory to store the shader data.
      std::vector<std::byte> shaderData;
      shaderData.resize(entry->size() - sizeof(hdr));

      //Read the shader data.
      winrt::check_hresult(read_file_sync(f->handle(),
                                          shaderData.size(),
                                          entry->offset() + sizeof(hdr),
                                          shaderData.data()));

      if (entry->metadata()->loader_id() == CONTENT_LOADER_ID_SHADER_SOURCE)
      {
         throw std::invalid_argument(
            "No support for compiling shaders at run time.");
      }
      else if (entry->metadata()->loader_id() ==
               CONTENT_LOADER_ID_SHADER_COMPILED)
      {
         return std::make_unique<shader>(&hdr,
                                         shaderData.data(),
                                         shaderData.size(),
                                         headerInfo->name(),
                                         newID);
      }
      else
      {
         throw std::runtime_error("The entry's loader ID is not correct.");
      }
   }
}