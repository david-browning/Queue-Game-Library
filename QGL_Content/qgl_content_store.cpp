#include "pch.h"
#include "include/Content-Store/qgl_content_store.h"

namespace qgl::content
{
   content_store::content_store(const content_store_config& storeConfig) :
      m_config(storeConfig)
   {

   }

   content_store::IDT content_store::file_id(const FileStringT& relativePath)
   {

   }

   content_store::FileStringT content_store::abs_path(
      const FileStringT& relativePath) const
   {
      return FileStringT();
   }
}