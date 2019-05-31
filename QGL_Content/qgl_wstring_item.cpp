#include "pch.h"
#include "include\Content-Items\qgl_wstring_item.h"

namespace qgl::content
{
   struct wstring_item::impl
   {
      impl(const wchar_t* s) :
         str(s)
      {

      }

      impl(const impl&) = default;

      impl(impl&&) = default;

      virtual ~impl() noexcept = default;

      friend bool operator==(const impl& l, const impl& r) noexcept
      {
         return l.str == r.str;
      }

      std::wstring str;
   };

   wstring_item::wstring_item(const wchar_t* str,
                              const wchar_t* name, 
                              content_id id, 
                              RESOURCE_TYPES rType,
                              CONTENT_LOADER_IDS loaderID) :
      content_item(name, id, rType, loaderID),
      m_impl_p(new impl(str))
   {
   }

   wstring_item::wstring_item(const wstring_item& r) :
      content_item(r),
      m_impl_p(new impl(r.c_str()))
   {
   }

   wstring_item::wstring_item(wstring_item&& r) :
      content_item(std::move(r))
   {
      m_impl_p = r.m_impl_p;
      r.m_impl_p = nullptr;
   }

   wstring_item::~wstring_item() noexcept
   {
      delete m_impl_p;
      m_impl_p = nullptr;
   }

   const wchar_t* qgl::content::wstring_item::c_str() const noexcept
   {
      return m_impl_p->str.c_str();
   }

   wchar_t* qgl::content::wstring_item::data() noexcept
   {
      return m_impl_p->str.data();
   }

   bool impl_equal(const wstring_item::impl* l, 
                   const wstring_item::impl* r,
                   const content_item* lm,
                   const content_item* rm) noexcept
   {
      return *l == *r && *lm == *rm;
   }
}