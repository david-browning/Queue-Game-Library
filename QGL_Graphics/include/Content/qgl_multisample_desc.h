#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_multisample_desc_buffer.h"

namespace qgl::content
{
   class QGL_GRAPHICS_API multisample_desc : public content_item
   {
      public:
      multisample_desc(const buffers::MULTISAMPLE_DESC_BUFFER* buff_p,
                       const wchar_t* name,
                       id_t id);

      multisample_desc(const multisample_desc&) = default;

      multisample_desc(multisample_desc&&) = default;

      virtual ~multisample_desc() noexcept = default;

      UINT count() const noexcept;

      UINT quality() const noexcept;

      private:
      buffers::MULTISAMPLE_DESC_BUFFER m_buffer;
   };
}