#include "pch.h"
#include "include/Content-Store/qgl_content_store_config.h"
#include "include/qgl_file_helpers.h"

namespace qgl::content
{
   content_store_config::content_store_config(const winrt::hstring& storePath) :
      content_store_config(storePath.c_str(),
                           storePath.size())
   {
   }

   content_store_config::content_store_config(const wchar_t* storePath, 
                                              size_t len)
   {
      m_storePath = new wchar_t[len + 1];
      MemoryCopy(m_storePath, storePath, len);
      m_storePath[len] = L'\0';
   }

   content_store_config::content_store_config(const content_store_config& c)
   {
      auto len = MemoryLength(c.m_storePath);
      m_storePath = new wchar_t[len + 1];
      MemoryCopy(m_storePath, c.m_storePath, len);
      m_storePath[len] = L'\0';
   }

   content_store_config::content_store_config(content_store_config&& m)
   {
      m_storePath = m.m_storePath;
      m.m_storePath = nullptr;
   }

   content_store_config::~content_store_config() noexcept
   {
      delete[] m_storePath;

      #ifdef DEBUG
      m_storePath = nullptr;
      #endif
   }

   const wchar_t* content_store_config::path() const
   {
      return m_storePath;
   }
}
