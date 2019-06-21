#include "pch.h"
#include "include/Content/qgl_multisample_desc.h"

namespace qgl::content
{
   multisample_desc::multisample_desc(
      const buffers::MULTISAMPLE_DESC_BUFFER* buff_p, 
      const wchar_t* name, 
      id_t id) :
      m_buffer(*buff_p),
      content_item(name, id,
                   RESOURCE_TYPE_DESCRIPTION,
                   CONTENT_LOADER_ID_MULTISAMPLE_DESCRIPTION)
   {

   }

   UINT multisample_desc::count() const noexcept
   {
      return m_buffer.Count;
   }

   UINT multisample_desc::quality() const noexcept
   {
      return m_buffer.Quality;
   }
}