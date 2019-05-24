#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Files/qgl_content_file.h"
#include "include/Content-Store/qgl_content_store_config.h"

namespace qgl::content
{
   struct QGL_CONTENT_API content_store_config_loader
   {
      std::shared_ptr<content_store_config> operator()(
         const content_file& f);
   };
}