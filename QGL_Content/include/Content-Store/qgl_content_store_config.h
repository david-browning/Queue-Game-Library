#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content
{
   /*
    Contains configuration data for a content store.
    */
   class QGL_CONTENT_API content_store_config
   {
      public:
      /*
       Constructor.
       */
      content_store_config(const wchar_t* storePath);

      /*
       Copy constructor.
       */
      content_store_config(const content_store_config&);

      /*
       Move constructor.
       */
      content_store_config(content_store_config&&);

      /*
       Destructor.
       */
      virtual ~content_store_config() noexcept;

      /*
       Returns a const pointer to the store's root path.
       */
      const wchar_t* path() const;

      friend void swap(content_store_config& first, 
                       content_store_config& second) noexcept
      {
         using std::swap;
         swap(first.m_storePath, second.m_storePath);
      }

      content_store_config& operator=(content_store_config r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:
      /*
       The store's root path. This always ends with a backslash.
       */
      wchar_t* m_storePath;
   };
}