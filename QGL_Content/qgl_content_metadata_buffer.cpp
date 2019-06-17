#include "pch.h"
#include "include/Content-Buffers/qgl_content_metadata_buffer.h"
#include <objbase.h>

namespace qgl::content
{

   /*
    Content is visible.
    */
   static constexpr uint8_t DEFAULT_FLAGS = 0b00001000;

   static constexpr uint8_t IS_SHARED_FLAG = 0x80;

   CONTENT_METADATA_BUFFER::CONTENT_METADATA_BUFFER() :
      m_flags1(DEFAULT_FLAGS),
      m_reserved1(0),
      m_reserved2(0),
      m_reserved3(0),
      m_reserved4(0),
      m_type(RESOURCE_TYPES::RESOURCE_TYPE_UNKNOWN),
      m_loaderID(CONTENT_LOADER_IDS::CONTENT_LOADER_ID_UNKNOWN),
      m_compilerVersion(qgl::QGL_VERSION_LATEST)
   {
      set_memory(m_name, L'\0', CONTENT_MAX_NAME_LEN);
      winrt::check_hresult(CoCreateGuid(&m_guid));
   }

   CONTENT_METADATA_BUFFER::CONTENT_METADATA_BUFFER(
      RESOURCE_TYPES type,
      CONTENT_LOADER_IDS loaderID,
      const wchar_t* name) :
      m_flags1(DEFAULT_FLAGS),
      m_reserved1(0),
      m_reserved2(0),
      m_reserved3(0),
      m_reserved4(0),
      m_type(type),
      m_loaderID(loaderID),
      m_compilerVersion(qgl::QGL_VERSION_LATEST)
   {
      auto nameLength = mem_length(name, L'\0');
      if (nameLength <= CONTENT_MAX_NAME_LEN)
      {
         copy_elements(m_name, name, nameLength);
         m_name[nameLength] = L'\0';
      }
      else
      {
         throw std::domain_error("The name is too long.");
      }

      winrt::check_hresult(CoCreateGuid(&m_guid));
   }

   CONTENT_METADATA_BUFFER::CONTENT_METADATA_BUFFER(
      const CONTENT_METADATA_BUFFER& r) :
      m_flags1(r.m_flags1),
      m_reserved1(r.m_reserved1),
      m_reserved2(r.m_reserved2),
      m_reserved3(r.m_reserved3),
      m_reserved4(r.m_reserved4),
      m_type(r.m_type),
      m_loaderID(r.m_loaderID),
      m_compilerVersion(r.version())
   {
      copy_elements<wchar_t>(m_name, r.m_name, CONTENT_MAX_NAME_LEN);
      copy_elements<GUID>(&m_guid, &r.m_guid, 1);
   }

   CONTENT_METADATA_BUFFER::CONTENT_METADATA_BUFFER(
      CONTENT_METADATA_BUFFER&& r) :
      m_flags1(r.m_flags1),
      m_reserved1(r.m_reserved1),
      m_reserved2(r.m_reserved2),
      m_reserved3(r.m_reserved3),
      m_reserved4(r.m_reserved4),
      m_type(r.m_type),
      m_loaderID(r.m_loaderID),
      m_compilerVersion(r.version())
   {
      copy_elements<wchar_t>(m_name, r.m_name, CONTENT_MAX_NAME_LEN);
      copy_elements<GUID>(&m_guid, &r.m_guid, 1);
   }

   bool CONTENT_METADATA_BUFFER::content_visible() const noexcept
   {
      return (m_flags1 & 0x08) != 0;
   }

   void CONTENT_METADATA_BUFFER::content_visible(bool isVisible) noexcept
   {
      if (isVisible)
      {
         m_flags1 |= 0x08;
      }
      else
      {
         m_flags1 &= ~0x08;
      }
   }

   bool CONTENT_METADATA_BUFFER::obey_physics() const noexcept
   {
      return (m_flags1 & 0x04) != 0;
   }

   void CONTENT_METADATA_BUFFER::obey_physics(bool obey) noexcept
   {
      if (obey)
      {
         m_flags1 |= 0x04;
      }
      else
      {
         m_flags1 &= ~0x04;
      }
   }

   const wchar_t* CONTENT_METADATA_BUFFER::name() const noexcept
   {
      return m_name;
   }

   void CONTENT_METADATA_BUFFER::name(const wchar_t* const n)
   {
      auto len = wcslen(n);
      if (len <= CONTENT_MAX_NAME_LEN)
      {
         copy_elements<wchar_t>(m_name, n, len);
         m_name[len] = L'\0';
      }
      else
      {
         throw std::domain_error("The name is too long.");
      }
   }

   RESOURCE_TYPES CONTENT_METADATA_BUFFER::resource_type() const noexcept
   {
      return static_cast<RESOURCE_TYPES>(m_type);
   }

   void CONTENT_METADATA_BUFFER::resource_type(RESOURCE_TYPES t) noexcept
   {
      m_type = t;
   }

   CONTENT_LOADER_IDS CONTENT_METADATA_BUFFER::loader_id() const noexcept
   {
      return static_cast<CONTENT_LOADER_IDS>(m_loaderID);
   }

   void CONTENT_METADATA_BUFFER::loader_id(CONTENT_LOADER_IDS l) noexcept
   {
      m_loaderID = l;
   }

   qgl::qgl_version_t CONTENT_METADATA_BUFFER::version() const noexcept
   {
      return m_compilerVersion;
   }

   void CONTENT_METADATA_BUFFER::version(qgl::qgl_version_t v) noexcept
   {
      m_compilerVersion = v;
   }

   const GUID* CONTENT_METADATA_BUFFER::guid() const noexcept
   {
      return &m_guid;
   }

   bool CONTENT_METADATA_BUFFER::shared() const noexcept
   {
      return (m_flags1 & IS_SHARED_FLAG) != 0;
   }

   void CONTENT_METADATA_BUFFER::shared(bool v) noexcept
   {
      auto bit = static_cast<uint8_t>(v ? 1 : 0);
      uint8_t oldFlags = m_flags1 & ~IS_SHARED_FLAG;
      m_flags1 = oldFlags | (bit << 7);
   }

   winrt::hstring CONTENT_METADATA_BUFFER::guid_str() const
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
}