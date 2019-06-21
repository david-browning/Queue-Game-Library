#include "pch.h"
#include "include/Content/File-Loaders/qgl_pso_file_loader.h"
#include "include/Content/Content-Loaders/qgl_shader_importer.h"

namespace qgl::content::loaders
{
   pso_file_loader::pso_file_loader(graphics::d3d_device* dev_p,
                                    icontent_store* store_p) :
      m_store_p(store_p),
      m_dev_p(dev_p)
   {
   }

   std::unique_ptr<ipso> pso_file_loader::operator()(const icontent_file* f,
                                                     id_t newID) const
   {
      //Make sure the file has the correct loader.
      auto headerInfo = f->const_header()->metadata();
      check_loader_and_resource<RESOURCE_TYPE_PSO,
         CONTENT_LOADER_ID_PSO>(headerInfo);

      //Make sure it has between the minimum and maximum entries.
      const auto dictCount = f->size();
      if (dictCount < 6 || dictCount > 9)
      {
         throw std::runtime_error("Too few or too many dictionary entries.");
      }

      //These are the fields we need to load.
      std::unique_ptr<blender> blender_p = nullptr;
      std::unique_ptr<rasterizer> rasterizer_p = nullptr;
      std::unique_ptr<multisample_desc> samplerDesc_p = nullptr;
      std::unique_ptr<vertex_description> vertDesc_p = nullptr;

      //Load each entry and put it in the correct spot in the creation params.
      for (size_t i = 0; i < dictCount; i++)
      {
         auto entry = f->const_at(i);
         auto eMeta = entry->metadata();
         switch (eMeta->resource_type())
         {
            case RESOURCE_TYPE_DESCRIPTION:
            {
               switch (eMeta->loader_id())
               {
                  case CONTENT_LOADER_ID_VERTEX_DESCRIPTION:
                  {
                     //Load the vertex description.



                     break;
                  }
                  case CONTENT_LOADER_ID_MULTISAMPLE_DESCRIPTION:
                  {
                     //Load the multi-sampling description.

                     break;
                  }
                  default:
                  {
                     throw std::invalid_argument(
                        "An unsupported loader was found in the dictionary.");
                  }
               }
               break;
            }
            case RESOURCE_TYPE_RASTERIZER:
            {
               check_loader_id<CONTENT_LOADER_ID_RASTERIZER>(eMeta);
               //Load the rasterizer.


               break;
            }
            case RESOURCE_TYPE_BLENDER:
            {
               check_loader_id<CONTENT_LOADER_ID_BLENDER>(eMeta);
               //Load the blender



               break;
            }
            case RESOURCE_TYPE_SHADER:
            {
               //Load the shader.



               break;
            }
            default:
            {
               throw std::invalid_argument(
                  "An unsupported resource was found in the dictionary.");
            }
         }
      }

      //Check that we loaded everything we need.



      //Construct the PSO and return it.

      return nullptr;
   }
}