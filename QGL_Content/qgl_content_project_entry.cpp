#include "pch.h"
#include "include/Content-Project/qgl_content_project_entry.h"

namespace qgl::content::helpers
{
   content_project_entry::content_project_entry(
      const CONTENT_METADATA_BUFFER* meta_p, 
      const wchar_t* absPath) :
      m_meta(*meta_p)
   {
      m_pathLen = wcslen(absPath);
      m_absPath = new wchar_t[m_pathLen + 1];
      memcpy(m_absPath, absPath, sizeof(wchar_t) * m_pathLen);
      m_absPath[m_pathLen] = L'\0';
   }

   content_project_entry::content_project_entry(
      const content_project_entry& r) :
      m_meta(r.m_meta),
      m_pathLen(r.m_pathLen)
   {
      delete[] m_absPath;
      m_absPath = new wchar_t[m_pathLen + 1];
      memcpy(m_absPath, r.m_absPath, sizeof(wchar_t) * m_pathLen);
      m_absPath[m_pathLen] = L'\0';
   }

   content_project_entry::content_project_entry(content_project_entry&& r) :
      m_meta(std::move(r.m_meta)),
      m_pathLen(std::move(r.m_pathLen))
   {
      delete[] m_absPath;
      m_absPath = r.m_absPath;

      r.m_absPath = nullptr;
   }

   content_project_entry::~content_project_entry() noexcept
   {
      delete[] m_absPath;
      m_absPath = nullptr;
   }

   const CONTENT_METADATA_BUFFER* content_project_entry::const_metadata() 
      const noexcept
   {
      return &m_meta;
   }

   const wchar_t* content_project_entry::const_path() const noexcept
   {
      return m_absPath;
   }

   size_t content_project_entry::path_size() const noexcept
   {
      return m_pathLen;
   }
}