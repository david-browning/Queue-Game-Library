#include "pch.h"
#include "include/Content/File-Loaders/qgl_sampler_file_loader.h"
#include "include/Content/Content-Loaders/qgl_sampler_importer.h"
#include "include/Content/qgl_sampler.h"

namespace qgl::content::loaders
{
   std::unique_ptr<sampler> qgl_load_sampler_file(const icontent_file* f,
                                                  const id_t newID)
   {
      //Make sure the file has the correct loader.
      auto headerInfo = f->const_header()->metadata();
      check_loader_and_resource<RESOURCE_TYPE_SAMPLER,
         CONTENT_LOADER_ID_SAMPLER>(headerInfo);

      //Only 1 dictionary entry.
      check_dictionary_count<1>(f);

      auto entry = f->const_at(0);
      check_loader_and_resource<RESOURCE_TYPE_SAMPLER,
         CONTENT_LOADER_ID_STRUCT>(entry->metadata());

      static sampler_buffer_importer importer;
      auto buff = importer.load(*f->handle(),
                                *entry);

      return std::make_unique<sampler>(&buff,
                                       headerInfo->name(),
                                       newID);
   }
}

