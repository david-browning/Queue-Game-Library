#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/qgl_shader.h"

namespace qgl::content::loaders
{
   class shader_load_entry_functor
   {
      public:
      shader operator()(
         const file_handle& fileHandle,
         const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup) const
      {
         //Read the header.
         buffers::SHADER_HEADER_BUFFER hdr;
         winrt::check_hresult(read_file_sync(&fileHandle,
                                             sizeof(hdr),
                                             lookup.offset(),
                                             &hdr));

         //Allocate memory to store the shader data.
         std::vector<std::byte> shaderData;
         shaderData.resize(lookup.size() - sizeof(hdr));
         winrt::check_hresult(read_file_sync(&fileHandle,
                                             shaderData.size(),
                                             lookup.offset() + sizeof(hdr),
                                             shaderData.data()));

         return shader(&hdr, shaderData.data(),
                       shaderData.size(),
                       lookup.metadata()->name(),
                       INVALID_CONTENT_ID);
      }
   };

   class shader_dict_export_functor
   {
      public:
      CONTENT_DICTIONARY_ENTRY_BUFFER operator()(
         const shader& shdr,
         const wchar_t* name,
         size_t offset) const
      {
         CONTENT_METADATA_BUFFER info(RESOURCE_TYPE_SHADER,
                                      CONTENT_LOADER_ID_SHADER_COMPILED,
                                      name);

         size_t entrySize = sizeof(buffers::SHADER_HEADER_BUFFER) +
            shdr.byte_code()->BytecodeLength;

         return CONTENT_DICTIONARY_ENTRY_BUFFER(entrySize,
                                                &info,
                                                offset);
      }
   };

   using shader_importer = entry_importer<
      shader,
      shader_load_entry_functor,
      shader_dict_export_functor>;
}