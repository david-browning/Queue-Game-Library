#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/qgl_camera.h"
#include "include/Content/Content-Buffers/qgl_camera_buffer.h"
#include "include/Content/Content-Loaders/qgl_camera_importer.h"

namespace qgl::content::loaders
{
   /*
    Functor for loading a camera from a camera content file.
    */
   class QGL_GRAPHICS_API camera_file_loader
   {
      public:
      camera_file_loader(float viewWidth,
                         float viewHeight,
                         graphics::d3d_device* dev_p) :
         m_viewHeight(viewHeight),
         m_viewWidth(viewWidth),
         m_dev_p(dev_p)
      {

      }

      template<bool RightHandMode = true>
      std::unique_ptr<camera<RightHandMode>> operator()(const icontent_file* f,
                                                        const id_t newID)
      {
         //Make sure the file has the correct loader.
         auto headerInfo = f->const_header()->metadata();
         check_loader_and_resource<RESOURCE_TYPES::RESOURCE_TYPE_CAMERA,
            CONTENT_LOADER_IDS::CONTENT_LOADER_ID_CAMERA>(headerInfo)

         //Cameras only have 1 entry: The description
         const auto dictCount = f->size();
         if (dictCount != 1)
         {
            throw std::runtime_error(
               "The content file should only have 1 entry.");
         }

         auto entry = f->const_at(0);
         check_loader_and_resource<RESOURCE_TYPE_DESCRIPTION,
            CONTENT_LOADER_ID_CAMERA>(entry->metadata());

         static camera_buffer_importer importer;

         auto buffer = importer.load(*f->handle(), *entry);
         return std::make_unique<camera<RightHandMode>>(&buffer,
                                                        m_viewWidth,
                                                        m_viewHeight,
                                                        m_dev_p,
                                                        headerInfo->name(),
                                                        newID);
      }

      private:
      float m_viewWidth;
      float m_viewHeight;
      graphics::d3d_device* m_dev_p;
   };
}