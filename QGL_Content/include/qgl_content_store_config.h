#pragma once
#include "qgl_content_include.h"

namespace qgl::content
{
   class LIB_EXPORT content_store_config
   {
      public:
      content_store_config(const winrt::hstring& storePath);

      content_store_config(const content_store_config&) = default;

      content_store_config(content_store_config&&) = default;

      virtual ~content_store_config() noexcept = default;

      inline const winrt::hstring& path() const
      {
         return m_storePath;
      }

      friend void swap(content_store_config& first, content_store_config& second) noexcept
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
      #pragma warning(push)
      #pragma warning(disable: 4251)
      winrt::hstring m_storePath;
      #pragma warning(pop)
   };
}