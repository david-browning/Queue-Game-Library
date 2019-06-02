#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_text_format_buffer.h"

namespace qgl::content
{
   /*
    Describes how to render text.
    */
   class QGL_GRAPHICS_API text_format : public qgl::content::content_item
   {
      public:
      /*
       Constructor.
       Get the IDWriteFactory from a graphics_device.
       */
      text_format(const content::buffers::TEXT_FORMAT_BUFFER* format,
                  IDWriteFactory* factory_p,
                  const wchar_t* name,
                  qgl::content::content_id id);

      /*
       Copy constructor.
       */
      text_format(const text_format& r);

      /*
       Move constructor.
       */
      text_format(text_format&& r);

      /*
       Destructor
       */
      virtual ~text_format() noexcept;

      /*
       Returns a const pointer to the D2D text format.
       */
      const IDWriteTextFormat* get() const noexcept;

      /*
       Returns a pointer to the D2D text format.
       */
      IDWriteTextFormat* get() noexcept;

      private:
      struct impl;
      impl* m_impl_p = nullptr;
   };
}