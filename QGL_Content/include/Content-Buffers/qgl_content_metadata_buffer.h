#pragma once
#include "include/qgl_content_include.h"
#include "include/qgl_content_types.h"

namespace qgl::content
{
   static constexpr size_t CONTENT_MAX_NAME_LEN = 40;
   static constexpr size_t QGL_GUID_STR_LEN = 36;

   /*
    Stores metadata about content. Content could be a texture, geometry, or
    other data used in the engine.
    */
   struct LIB_EXPORT CONTENT_METADATA_BUFFER
   {
      public:
      /*
       Default constructor.
       Sets the flags to default, resource and loader to unknown, the name to
       the empty string, and the compiler version to latest. The content is
       visible and does not obey physics.
       Creates a new GUID.
       Throws an exception if a GUID cannot be created.
       */
      CONTENT_METADATA_BUFFER();

      /*
        The content is visible and does not obey physics.
        Sets the compiler version to latest.
       */
      CONTENT_METADATA_BUFFER(RESOURCE_TYPES type,
                              CONTENT_LOADER_IDS loaderID,
                              const wchar_t* name,
                              size_t nameLength);

      /*
       The content is visible and does not obey physics.
       Sets the compiler version to latest.
       Creates a new GUID.
       Throws an exception if a GUID cannot be created.
       Throws if the name is too long.
       */
      CONTENT_METADATA_BUFFER(RESOURCE_TYPES type,
                              CONTENT_LOADER_IDS loaderID,
                              const winrt::hstring& name);

      /*
       Copy constructor.
       */
      CONTENT_METADATA_BUFFER(const CONTENT_METADATA_BUFFER& r);

      /*
       Move constructor.
       */
      CONTENT_METADATA_BUFFER(CONTENT_METADATA_BUFFER&& r);

      /*
       Destructor. Do not mark as virtual.
       */
      ~CONTENT_METADATA_BUFFER() = default;

      /*
       Returns true if the content is supposed to be visible.
       */
      inline bool content_visible() const
      {
         return (m_flags1 & 0x08) != 0;
      }

      /*
       Changes the visibility state.
       */
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

      /*
       Returns true if the content should obey physics.
       */
      inline bool obey_physics() const
      {
         return (m_flags1 & 0x04) != 0;
      }

      /*
       Changes whether the content should obey physics.
       */
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

      /*
       Returns the content's name.
       */
      winrt::hstring name() const
      {
         return winrt::hstring(m_name);
      }

      /*
       Sets the content's name.
       Throws if the name is too long.
       */
      void name(const winrt::hstring& n);

      /*
       Returns the type of content this is.
       */
      inline RESOURCE_TYPES resource_type() const
      {
         return static_cast<RESOURCE_TYPES>(m_type);
      }

      /*
       Returns a reference to the resource type.
       */
      auto& resource_type()
      {
         return m_type;
      }

      /*
       Returns the loader ID this content uses.
       */
      inline CONTENT_LOADER_IDS loader_id() const
      {
         return static_cast<CONTENT_LOADER_IDS>(m_loaderID);
      }

      /*
       Returns a reference to the loader ID.
       */
      auto& loader_id()
      {
         return m_loaderID;
      }

      /*
       Returns the compiler version this content was compiled for.
       */
      inline qgl::qgl_version_t version() const
      {
         return m_compilerVersion;
      }

      /*
       Returns a const reference to the content's GUID.
       */
      inline const GUID& guid() const
      {
         return m_guid;
      }

      /*
       Returns the content's GUID as a string.
       Throws if the GUID cannot be converted to a string.
       */
      winrt::hstring guid_str() const;

      friend void swap(CONTENT_METADATA_BUFFER& first,
                       CONTENT_METADATA_BUFFER& second) noexcept
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

      /*
       Overwrites this with r and returns a reference to this.
       */
      CONTENT_METADATA_BUFFER& operator=(CONTENT_METADATA_BUFFER r)  noexcept
      {
         swap(*this, r);
         return *this;
      }

      friend bool operator==(const CONTENT_METADATA_BUFFER& r,
                             const CONTENT_METADATA_BUFFER& l) noexcept
      {
         return r.m_compilerVersion == l.m_compilerVersion &&
            r.m_flags1 == l.m_flags1 &&
            r.m_guid == l.m_guid &&
            r.m_loaderID == r.m_loaderID &&
            wcscmp(r.m_name, l.m_name) == 0 &&
            r.m_reserved1 == l.m_reserved1 &&
            r.m_reserved2 == l.m_reserved2 &&
            r.m_type == l.m_type;
      }

      private:
      /*
       Version the content was compiled.
       */
      uint32_t m_compilerVersion;

      /*
       Unique ID for this content.
       */
      GUID m_guid = { 0 };

      /*
       Content type. See qgl_content_types.h for options.
       */
      uint16_t m_type;

      /*
       Content loader ID. See qgl_content_types.h for options.
       */
      uint16_t m_loaderID;

      /*
       { 1 Is Shared }{ 3 }{ 1 IsVisible }{ 1 IsPhysical }{ 2 Flags }
       */
      uint8_t m_flags1;

      uint8_t m_reserved1;

      uint16_t m_reserved2;

      /*
       Name of this content object.
       */
      wchar_t m_name[CONTENT_MAX_NAME_LEN] = { 0 };

      /*
       Content is visible.
       */
      static constexpr uint8_t DEFAULT_FLAGS = 0b00001000;

   };
}