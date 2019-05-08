#pragma once
#include "include/qgl_content_include.h"
#include "include/qgl_content_types.h"

namespace qgl::content
{
   static constexpr size_t CONTENT_MAX_NAME_LEN = 40;
   static constexpr size_t QGL_GUID_STR_LEN = 36;

   struct LIB_EXPORT CONTENT_INFO_BUFFER
   {
      public:
      CONTENT_INFO_BUFFER();

      CONTENT_INFO_BUFFER(RESOURCE_TYPES type,
                          CONTENT_LOADER_IDS loaderID,
                           const wchar_t* name,
                          size_t nameLength);

      CONTENT_INFO_BUFFER(RESOURCE_TYPES type,
                          CONTENT_LOADER_IDS loaderID,
                          const winrt::hstring& name);

      CONTENT_INFO_BUFFER(const CONTENT_INFO_BUFFER& r);

      CONTENT_INFO_BUFFER(CONTENT_INFO_BUFFER&& r);

      ~CONTENT_INFO_BUFFER() = default;
           
      inline uint8_t content_char_width() const
      {
         return (m_flags1 & 0x70) >> 4;
      }

      inline void content_char_width(uint8_t charWidthBytes)
      {
         //Clear the width flags first.
         m_flags1 &= 0x8F;

         m_flags1 |= ((charWidthBytes & 0x07) << 4);
      }

      /*
       Returns true if the content_item info indicates that it points to a separate file.
       */
      inline bool shared_content() const
      {
         return (m_flags1 & 0x80) != 0;
      }

      inline void shared_content(bool isShared)
      {
         if (isShared)
         {
            m_flags1 |= 0x80;
         }
         else
         {
            m_flags1 &= ~0x80;
         }
      }

      inline bool content_visible() const
      {
         return (m_flags1 & 0x08) != 0;
      }

      inline void content_visible(bool isVisible)
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

      inline bool obey_physics() const
      {
         return (m_flags1 & 0x04) != 0;
      }

      inline void obey_physics(bool obey)
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

      winrt::hstring name() const;

      void name(const winrt::hstring& n);

      inline RESOURCE_TYPES resource_type() const
      {
         return static_cast<RESOURCE_TYPES>(m_type);
      }

      inline void resource_type(RESOURCE_TYPES r)
      {
         m_type = r;
      }

      inline CONTENT_LOADER_IDS loader_id() const
      {
         return static_cast<CONTENT_LOADER_IDS>(m_loaderID);
      }

      inline void loader_id(CONTENT_LOADER_IDS id)
      {
         m_loaderID = id;
      }

      inline qgl::qgl_version_t version() const
      {
         return m_compilerVersion;
      }

      void version(qgl_version_t v)
      {
         m_compilerVersion = v;
      }

      inline const GUID& guid() const
      {
         return m_guid;
      }

      void guid(const GUID& g)
      {
         m_guid = g;
      }

      winrt::hstring guid_str() const;

      void guid_str(const winrt::hstring& g);

      friend void swap(CONTENT_INFO_BUFFER& first, 
                       CONTENT_INFO_BUFFER& second) noexcept
      {
         using std::swap;
         swap(first.m_type, second.m_type);
         swap(first.m_loaderID, second.m_loaderID);
         swap(first.m_flags1, second.m_flags1);
         swap(first.m_reserved1, second.m_reserved1);
         swap(first.m_reserved2, second.m_reserved2);
         swap(first.m_name, second.m_name);
         swap(first.m_compilerVersion, second.m_compilerVersion);
         swap(first.m_guid, second.m_guid);
      }

      CONTENT_INFO_BUFFER& operator=(CONTENT_INFO_BUFFER r)  noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:
      uint32_t m_compilerVersion;

      GUID m_guid = { 0 };

      uint16_t m_type;

      uint16_t m_loaderID;

      //{ 1 Is Shared } { 3 Char Width } { 1 IsVisible } { 1 IsPhysical } { 2 Flags }
      uint8_t m_flags1;

      uint8_t m_reserved1;

      uint16_t m_reserved2;
                  
      //Name of this content_item object.
      wchar_t m_name[CONTENT_MAX_NAME_LEN] = { 0 };

   };
}