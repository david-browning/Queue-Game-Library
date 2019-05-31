#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Items/qgl_content_item.h"

namespace qgl::content
{
   class QGL_CONTENT_API wstring_item : public content_item
   {
      public:
      /*
       Makes a copy of str. It can go out of scope after this is constructed.
       */
      wstring_item(const wchar_t* str,
                   const wchar_t* name,
                   content_id id,
                   RESOURCE_TYPES rType,
                   CONTENT_LOADER_IDS loaderID);

      /*
       Copy constructor.
       */
      wstring_item(const wstring_item& r);

      /*
       Move constructor.
       */
      wstring_item(wstring_item&& r);

      /*
       Destructor.
       */
      virtual ~wstring_item() noexcept;

      /*
       Returns a const pointer to the string.
       */
      const wchar_t* c_str() const noexcept;

      /*
       Returns a pointer to the string.
       */
      wchar_t* data() noexcept;

      /*
       Swaps the contents of l and r.
       */
      friend void swap(wstring_item& l, wstring_item& r) noexcept
      {
         using std::swap;
         swap(l.m_impl_p, r.m_impl_p);
      }

      /*
       Assignment operator.
       */
      wstring_item& operator=(wstring_item r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Equality operator.
       */
      friend bool operator==(const wstring_item& l,
                             const wstring_item& r) noexcept
      {
         return impl_equal(l.m_impl_p, r.m_impl_p, &l, &r);
      }

      private:
      struct impl;
      impl* m_impl_p;

      /*
       Returns true if the contents of l and r are equal.
       */
      friend QGL_CONTENT_API bool impl_equal(const impl* l, const impl* r,
                                             const content_item* lm,
                                             const content_item* rm) noexcept;
   };
}