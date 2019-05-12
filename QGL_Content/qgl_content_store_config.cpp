#include "pch.h"
#include "include/Content-Store/qgl_content_store_config.h"
#include "include/qgl_file_helpers.h"

qgl::content::content_store_config::content_store_config(
   const winrt::hstring& storePath) :
   m_storePath(storePath)
{
   //Check that the location exists.
   if (!dir_exists(m_storePath))
   {
      throw std::runtime_error("The store path does not exist.");
   }

   //The store path needs to end with a backslash.
   if (m_storePath.back() != L'\\')
   {
      std::wstring newPath(m_storePath.c_str());
      newPath.push_back(L'\\');
      m_storePath = newPath;
   }
}
