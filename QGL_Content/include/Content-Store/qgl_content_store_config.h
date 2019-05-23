#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content
{
   /*
    Contains configuration data for a content store.
    */
   class LIB_EXPORT content_store_config
   {
      public:
      
      /*
       Constructor.
       */
      content_store_config(const winrt::hstring& storePath);

      /*
       Copy constructor.
       */
      content_store_config(const content_store_config&) = default;

      /*
       Move constructor.
       */
      content_store_config(content_store_config&&) = default;

      /*
       Destructor.
       */
      virtual ~content_store_config() noexcept = default;

      /*
       Returns a const reference to the store's root path.
       */
      inline const winrt::hstring& path() const
      {
         return m_storePath;
      }

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
       The store's root path.
       */
      winrt::hstring m_storePath;
   };
}