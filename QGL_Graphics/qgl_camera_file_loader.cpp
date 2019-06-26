#include "pch.h"
#include "include/Content/File-Loaders/qgl_camera_file_loader.h"
#include "include/Content/qgl_camera.h"
#include "include/Content/Content-Loaders/qgl_camera_importer.h"
#include "include/Content/Content-Buffers/qgl_camera_buffer.h"
#include "include/qgl_window.h"

namespace qgl::content::loaders
{
   camera_file_loader::camera_file_loader(const graphics::window* wnd_p,
                                          graphics::d3d_device* dev_p) :
      m_wnd_p(wnd_p),
      m_dev_p(dev_p)
   {
   }

   std::unique_ptr<camera> camera_file_loader::operator()(
      const icontent_file* f, 
      const id_t newID) const
   {
      //Make sure the file has the correct loader.
      auto headerInfo = f->const_header()->metadata();
      check_loader_and_resource<RESOURCE_TYPES::RESOURCE_TYPE_CAMERA,
         CONTENT_LOADER_IDS::CONTENT_LOADER_ID_CAMERA>(headerInfo);

      //Cameras only have 1 entry: The description
      check_dictionary_count<1>(f);

      auto entry = f->const_at(0);
      check_loader_and_resource<RESOURCE_TYPE_CAMERA,
         CONTENT_LOADER_ID_STRUCT>(entry->metadata());

      static camera_buffer_importer importer;

      auto buffer = importer.load(*f->handle(), *entry);
      return std::make_unique<camera>(&buffer,
                                      m_wnd_p->ratio(),
                                      m_dev_p,
                                      headerInfo->name(),
                                      newID);
   }
}