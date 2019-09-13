#include "pch.h"
#include "include/Content-Items/qgl_content_item.h"
using namespace qgl::mem;

namespace qgl::content
{
   content_item::content_item(const wchar_t* name, 
                              content_id id,
                              RESOURCE_TYPES rType, 
                              CONTENT_LOADER_IDS loaderID) :
      m_id(id),
      m_rType(rType),
      m_loaderID(loaderID)
   {
      auto len = wcslen(name);
      m_name = new wchar_t[len + 1];
      copy_elements(m_name, name, len);
      m_name[len] = L'\0';
   }

   content_item::content_item(const content_item& r) :
      m_id(r.m_id),
      m_loaderID(r.m_loaderID),
      m_rType(r.m_rType)
   {
      auto nameLen = mem_length(r.m_name, L'\0');
      m_name = new wchar_t[nameLen + 1];
      copy_elements(m_name, r.m_name, nameLen);
      m_name[nameLen] = L'\0';
   }

   content_item::content_item(content_item&& r)
   {
      m_name = r.m_name;
      m_id = r.m_id;
      m_rType = r.m_rType;
      m_loaderID = r.m_loaderID;

      r.m_name = nullptr;
   }

   content_item::~content_item() noexcept
   {
      delete[] m_name;
   }

   content_id content_item::id() const
   {
      return m_id;
   }

   const wchar_t* content_item::name() const
   {
      return m_name;
   }

   RESOURCE_TYPES content_item::resource_type() const
   {
      return m_rType;
   }

   CONTENT_LOADER_IDS content_item::loader_id() const
   {
      return m_loaderID;
   }
}