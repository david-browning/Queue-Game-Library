#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Items/qgl_content_item.h"

namespace qgl::content
{
   class QGL_CONTENT_API string_item : public content_item
   {
      public:
      /*
       Makes a copy of str. It can go out of scope after this is constructed.
       */
      string_item(const char* str,
                  const wchar_t* name,
                  content_id id);

      /*
       Copy constructor.
       */
      string_item(const string_item& r);

      /*
       Move constructor.
       */
      string_item(string_item&& r);

      /*
       Destructor
       */
      virtual ~string_item() noexcept;

      /*
       Returns a const pointer to the string.
       */
      const char* c_str() const noexcept;

      /*
       Returns a pointer to the string.
       */
      char* data() noexcept;

      /*
       Swaps the contents of l and r.
       */
      friend void swap(string_item& l, string_item& r) noexcept
      {
         using std::swap;
         swap(l.m_impl_p, r.m_impl_p);
      }

      /*
       Assignment operator.
       */
      string_item& operator=(string_item r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Equality operator.
       */
      friend bool operator==(const string_item& l,
                             const string_item& r) noexcept
      {
         return impl_equal(l.m_impl_p, r.m_impl_p, &l, &r);
      }

      private:
      struct impl;
      impl* m_impl_p = nullptr;

      /*
       Returns true if the contents of l and r are equal.
       */
      friend QGL_CONTENT_API bool impl_equal(const impl* l,
                                             const impl* r,
                                             const content_item* lm,
                                             const content_item* rm) noexcept;
   };
}