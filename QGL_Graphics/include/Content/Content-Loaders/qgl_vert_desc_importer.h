#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/qgl_vert_description.h"

namespace qgl::content::loaders
{
   class vert_desc_load_entry_functor
   {
      public:
      vertex_description operator()(
         const file_handle& fileHandle,
         const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup) const
      {
         //Read the first byte which is the number of vertex elements.
         uint8_t numElements = 0;
         winrt::check_hresult(read_file_sync(&fileHandle,
                                             sizeof(numElements),
                                             lookup.offset(),
                                             &numElements));

         //Allocate a vector to store the elements.
         std::vector<buffers::VERTEX_ELEMENT_BUFFER> elements;
         elements.resize(numElements);

         //Read the elements into the vector.
         winrt::check_hresult(
            read_file_sync(&fileHandle,
                           sizeof(buffers::VERTEX_ELEMENT_BUFFER) * numElements,
                           lookup.offset() + 8,
                           elements.data()));

         //This won't be stored in the content store, so it does not need to be
         //addressable by a content ID. Its ok to give it an invalid ID.
         return vertex_description(elements.data(),
                                   elements.size(),
                                   lookup.metadata()->name(),
                                   INVALID_CONTENT_ID); 
      }
   };

   class vert_desc_dict_export_functor
   {
      public:
      CONTENT_DICTIONARY_ENTRY_BUFFER operator()(
         const vertex_description& vertDesc,
         const wchar_t* name,
         size_t offset) const
      {
         CONTENT_METADATA_BUFFER info(RESOURCE_TYPE_DESCRIPTION,
                                      CONTENT_LOADER_ID_VERTEX_DESCRIPTION,
                                      name);

         const size_t vertDescSize = 8 +
            (vertDesc.size() * sizeof(buffers::VERTEX_ELEMENT_BUFFER));

         return CONTENT_DICTIONARY_ENTRY_BUFFER(vertDescSize,
                                                &info,
                                                offset);
      }
   };

   using vert_desc_importer = entry_importer<
      vertex_description,
      vert_desc_load_entry_functor,
      vert_desc_dict_export_functor>;
}