#include "pch.h"
#include "include\Content\qgl_wtext.h"

namespace qgl::content
{
   struct wtext::impl
   {
      impl(const wchar_t* text,
           const D2D_RECT_F* layout) :
         Layout(*layout),
         Text(text)
      {

      }

      impl(const impl&) = default;

      impl(impl&&) = default;

      virtual ~impl() noexcept = default;

      std::wstring Text;
      D2D_RECT_F Layout;
   };

   wtext::wtext(const wchar_t* text, 
                const D2D_RECT_F* layout,
                const wchar_t* name, 
                const qgl::content::content_id id) :
      m_impl_p(new impl(text, layout)),
      content_item(name, id,
                   qgl::content::RESOURCE_TYPE_STRING,
                   qgl::content::CONTENT_LOADER_ID_WTEXT)
   {
   }

   wtext::wtext(const wtext& r) :
      content_item(r)
   {
      delete m_impl_p;
      m_impl_p = new impl(*r.m_impl_p);
   }

   wtext::wtext(wtext&& r) :
      content_item(std::move(r))
   {
      delete m_impl_p;
      m_impl_p = r.m_impl_p;
      r.m_impl_p = nullptr;
   }

   wtext::~wtext() noexcept
   {
      delete m_impl_p;
      m_impl_p = nullptr;
   }

   size_t wtext::length() const noexcept
   {
      return m_impl_p->Text.size();
   }

   const wchar_t* wtext::c_str() const noexcept
   {
      return m_impl_p->Text.c_str();
   }

   const D2D_RECT_F* wtext::layout() const noexcept
   {
      return &m_impl_p->Layout;
   }
}