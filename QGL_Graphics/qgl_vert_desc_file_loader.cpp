#include "pch.h"
#include "include/Content/FIle-Loaders/qgl_vert_desc_file_loader.h"
#include "include/Content/qgl_vert_description.h"
#include "include/Content/Content-Loaders/qgl_vert_elem_importer.h"

namespace qgl::content::loaders
{
   std::unique_ptr<vertex_description> vert_desc_file_loader::operator()(
      const icontent_file* f,
      const id_t newID) const
   {
      //Make sure the file has the correct loader.
      auto headerInfo = f->const_header()->metadata();
      check_loader_and_resource<RESOURCE_TYPE_DESCRIPTION,
         CONTENT_LOADER_ID_VERTEX_DESCRIPTION>(headerInfo);

      static const vert_elem_importer elemImporter;

      auto numElements = f->size();
      std::vector<buffers::VERTEX_ELEMENT_BUFFER> elements;

      for (size_t i = 0; i < numElements; i++)
      {
         auto entry = f->const_at(i);
         check_loader_and_resource<RESOURCE_TYPE_DESCRIPTION,
            CONTENT_LOADER_ID_VERTEX_ELEMENT>(entry->metadata());

         elements.push_back(elemImporter.load(*f->handle(),
                                              *entry));

      }

      return std::make_unique<vertex_description>(elements.data(),
                                                  elements.size(),
                                                  headerInfo->name(),
                                                  newID);
   }
}