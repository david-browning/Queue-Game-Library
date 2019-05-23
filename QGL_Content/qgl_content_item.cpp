#include "pch.h"
#include "include/qgl_content_item.h"

namespace qgl::content
{
   content_item::content_item(const std::wstring& name,
                              const content_id& id, 
                              RESOURCE_TYPES rType, 
                              CONTENT_LOADER_IDS loaderID) :
      content_item(name.c_str(), name.size(), 
                   id, rType, loaderID)
   {
   }

   content_item::content_item(const wchar_t* name, 
                              size_t len, 
                              const content_id& id,
                              RESOURCE_TYPES rType, 
                              CONTENT_LOADER_IDS loaderID) :
      m_id(id),
      m_rType(rType),
      m_loaderID(loaderID)
   {
      m_name = new wchar_t[len + 1];
      MemoryCopy(m_name, name, len);
      m_name[len] = L'\0';
   }

   content_item::content_item(const content_item& r) :
      m_id(r.m_id),
      m_loaderID(r.m_loaderID),
      m_rType(r.m_rType)
   {
      auto nameLen = MemoryLength(r.m_name, L'\0');
      m_name = new wchar_t[nameLen + 1];
      MemoryCopy(m_name, r.m_name, nameLen);
      m_name[nameLen] = L'\0';
   }

   content_item::content_item(content_item&& r)
   {
      m_name = r.m_name;
      m_id = r.m_id;
      m_rType = r.m_rType;
      r.m_loaderID = r.m_loaderID;

      r.m_name = nullptr;
   }

   content_item::~content_item() noexcept
   {
      delete[] m_name;
   }

}