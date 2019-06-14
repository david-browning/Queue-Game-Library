#include "pch.h"
#include "include/Content-Items/qgl_string_item.h"

namespace qgl::content
{
   struct string_item::impl
   {
      impl(const char* s) :
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

      std::string str;
   };

   string_item::string_item(const char* str, 
                            const wchar_t* name, 
                            content_id id) :
      content_item(name, id,
                   RESOURCE_TYPES::RESOURCE_TYPE_STRING, 
                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_STRING),
      m_impl_p(new impl(str))
   {
   }

   string_item::string_item(const string_item& r) :
      content_item(r)
   {
      delete m_impl_p;
      m_impl_p = new impl(*r.m_impl_p);
   }

   string_item::string_item(string_item&& r) :
      content_item(std::move(r))
   {
      delete m_impl_p;
      m_impl_p = r.m_impl_p;
      r.m_impl_p = nullptr;
   }

   string_item::~string_item() noexcept
   {
      delete m_impl_p;
      m_impl_p = nullptr;
   }

   const char* string_item::c_str() const noexcept
   {
      return m_impl_p->str.c_str();
   }

   char* string_item::data() noexcept
   {
      return m_impl_p->str.data();
   }

   bool impl_equal(const string_item::impl* l,
                   const string_item::impl* r,
                   const content_item* lm,
                   const content_item* rm) noexcept
   {
      return *l == *r && *lm == *rm;
   }
}