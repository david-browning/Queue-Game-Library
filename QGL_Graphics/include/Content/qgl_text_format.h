#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_text_format_buffer.h"

namespace qgl::graphics::content
{
   class QGL_GRAPHICS_API text_format : public qgl::content::content_item
   {
      public:
      text_format(const content::buffers::TEXT_FORMAT_BUFFER* format,
                  IDWriteFactory* factory_p,
                  const wchar_t* name,
                  qgl::content::content_id id);

      text_format(const text_format& r);

      text_format(text_format&& r);

      virtual ~text_format() noexcept;

      const IDWriteTextFormat* get() const noexcept;

      IDWriteTextFormat* get() noexcept;

      private:
      struct impl;
      impl* m_impl_p;
   };
}