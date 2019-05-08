#include "pch.h"
#include "include/qgl_content_info_buffer.h"
#include <objbase.h>

qgl::content::CONTENT_INFO_BUFFER::CONTENT_INFO_BUFFER()
{
   MemorySet(m_name, L'\0', CONTENT_MAX_NAME_LEN);
   m_flags1 = 0b00011000;
   m_reserved1 = 0;
   m_reserved2 = 0;
   m_type = RESOURCE_TYPES::RESOURCE_TYPE_UNKNOWN;
   m_loaderID = CONTENT_LOADER_IDS::CONTENT_LOADER_ID_UNKNOWN;
   m_compilerVersion = qgl::QGL_VERSION_0_1_WIN;
   winrt::check_hresult(CoCreateGuid(&m_guid));
}

qgl::content::CONTENT_INFO_BUFFER::CONTENT_INFO_BUFFER(RESOURCE_TYPES type, 
                                                       CONTENT_LOADER_IDS loaderID,
                                                       const wchar_t* name,
                                                       size_t nameLength) :
   m_flags1(0b00011000), //content_item is visible and has 1 byte char width
   m_reserved1(0),
   m_reserved2(0),
   m_type(type),
   m_loaderID(loaderID)
{
   winrt::check_hresult(CoCreateGuid(&m_guid));
   assert(nameLength < CONTENT_MAX_NAME_LEN);
   MemoryCopy(m_name, name, nameLength);
   m_name[nameLength] = L'\0';
}

qgl::content::CONTENT_INFO_BUFFER::CONTENT_INFO_BUFFER(RESOURCE_TYPES type,
                                                       CONTENT_LOADER_IDS loaderID,
                                                       const winrt::hstring& name) :
   CONTENT_INFO_BUFFER(type, loaderID, name.c_str(), name.size())
{

}

qgl::content::CONTENT_INFO_BUFFER::CONTENT_INFO_BUFFER(const CONTENT_INFO_BUFFER& r) :
   m_flags1(r.m_flags1),
   m_reserved1(r.m_reserved1),
   m_reserved2(r.m_reserved2),
   m_type(r.m_type),
   m_loaderID(r.m_loaderID),
   m_compilerVersion(r.version())
{
   MemoryCopy<wchar_t>(m_name, r.m_name, CONTENT_MAX_NAME_LEN);
   MemoryCopy<GUID>(&m_guid, &r.m_guid, 1);

}

qgl::content::CONTENT_INFO_BUFFER::CONTENT_INFO_BUFFER(CONTENT_INFO_BUFFER&& r) :
   m_flags1(r.m_flags1),
   m_reserved1(r.m_reserved1),
   m_reserved2(r.m_reserved2),
   m_type(r.m_type),
   m_loaderID(r.m_loaderID),
   m_compilerVersion(r.version())
{
   MemoryCopy<wchar_t>(m_name, r.m_name, CONTENT_MAX_NAME_LEN);
   MemoryCopy<GUID>(&m_guid, &r.m_guid, 1);

}

winrt::hstring qgl::content::CONTENT_INFO_BUFFER::name() const
{
   return winrt::hstring(m_name);
}

void qgl::content::CONTENT_INFO_BUFFER::name(const winrt::hstring& n)
{
   auto len = n.size();
   if (len + 1 <= CONTENT_MAX_NAME_LEN)
   {
      MemoryCopy<wchar_t>(m_name, n.c_str(), len);
      m_name[len] = L'\0';
   }
   else
   {
      throw std::domain_error("The name is too long.");
   }
}

winrt::hstring qgl::content::CONTENT_INFO_BUFFER::guid_str() const
{
   OLECHAR* guidStr = nullptr;
   auto hr = StringFromCLSID(m_guid, &guidStr);
   if (SUCCEEDED(hr))
   {
      winrt::hstring ret{ guidStr };
      CoTaskMemFree(guidStr);
      return ret;
   }
   else
   {
      CoTaskMemFree(guidStr);
      winrt::throw_hresult(hr);
   }
}

void qgl::content::CONTENT_INFO_BUFFER::guid_str(const winrt::hstring& g)
{
  winrt::check_hresult(CLSIDFromString(g.c_str(), &m_guid));
}