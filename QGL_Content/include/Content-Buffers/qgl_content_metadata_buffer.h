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
       the empty string, and the compiler version to latest.
       Creates a new GUID.
       Throws an exception if a GUID cannot be created.
       */
      CONTENT_METADATA_BUFFER();

      CONTENT_METADATA_BUFFER(RESOURCE_TYPES type,
                          CONTENT_LOADER_IDS loaderID,
                           const wchar_t* name,
                          size_t nameLength);

      /*
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
       Returns the number of bytes a character uses.
       */
      inline uint8_t content_char_width() const
      {
         return (m_flags1 & 0x70) >> 4;
      }

      /*
       Sets the number of characters 
       */
      inline void content_char_width(uint8_t charWidthBytes)
      {
         //Clear the width flags first.
         m_flags1 &= 0x8F;

         m_flags1 |= ((charWidthBytes & 0x07) << 4);
      }

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
       Returns the type of content this is.
       */
      inline RESOURCE_TYPES resource_type() const
      {
         return static_cast<RESOURCE_TYPES>(m_type);
      }

      /*
       Returns the loader ID this content uses.
       */
      inline CONTENT_LOADER_IDS loader_id() const
      {
         return static_cast<CONTENT_LOADER_IDS>(m_loaderID);
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

      private:

      /*
       Returns a reference to the GUID.
       */
      inline GUID& guid()
      {
          return m_guid;
      }

      /*
       Converts the string to a GUID and sets the content's GUID.
       Throws if the string cannot be converted to a GUID.
       */
      void guid_str(const winrt::hstring& g);

      /*
       Sets the content's name.
       Throws if the name is too long.
       */
      void name(const winrt::hstring& n);


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
       { 1 Is Shared }{ 3 Char Width }{ 1 IsVisible }{ 1 IsPhysical }{ 2 Flags }
       */
      uint8_t m_flags1;

      uint8_t m_reserved1;

      uint16_t m_reserved2;
                  
      /*
       Name of this content object.
       */
      wchar_t m_name[CONTENT_MAX_NAME_LEN] = { 0 };

      /*
       Content is visible and has 1 byte char width
       */
      static constexpr uint8_t DEFAULT_FLAGS = 0b00011000;

   };
}