#pragma once
#include "include/qgl_content_include.h"
#include "include/Interfaces/qgl_icontent_file.h"
#include "include/Content-Store/qgl_content_store_config.h"

namespace qgl::content
{
   struct QGL_CONTENT_API content_store_config_loader
   {
      std::shared_ptr<content_store_config> operator()(
         const icontent_file* f);
   };
}