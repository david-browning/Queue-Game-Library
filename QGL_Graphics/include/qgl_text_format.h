#pragma once
#include "qgl_graphics_include.h"
#include "qgl_text_format_buffer.h"

namespace qgl::graphics
{
   class LIB_EXPORT text_format : public qgl::content::wcontent_item
   {
      public:
      text_format(const TEXT_FORMAT_BUFFER& format,
                  const winrt::com_ptr<IDWriteFactory>& factory_p,
                  const content::wcontent_item::str_t& name,
                  const content::wcontent_item::id_t& id);

      text_format(const text_format& r);

      text_format(text_format&& r);

      virtual ~text_format() noexcept = default;

      inline const IDWriteTextFormat* get() const
      {
         return m_format_p.get();
      }

      inline IDWriteTextFormat* get()
      {
         return m_format_p.get();
      }

      private:
      void m_create_text_format();

      TEXT_FORMAT_BUFFER m_buffer;
      #pragma warning(push)
      #pragma warning(disable: 4251)
      winrt::com_ptr<IDWriteFactory> m_factory_p;
      winrt::com_ptr<IDWriteTextFormat> m_format_p;
      #pragma warning(pop)
   };
}