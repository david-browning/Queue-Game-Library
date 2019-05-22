#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content
{
   /*
    Shared content is a path. This path is an encoded relative path to where
    the shared content file is stored. The path is encoded as metadata and a
    relative path, separated by a colon.
    The shared path must be converted to a content file using a content store.
    */
   struct LIB_EXPORT SHARED_CONTENT_ENTRY
   {
      public:
      /*
       Parses the string's metadata and copies the relative path to internal
       storage. 
       Throws std::invalid_argument is the string is not the correct form.
       */
      SHARED_CONTENT_ENTRY(const std::wstring& str);

      /*
       Copy Constructor.
       */
      SHARED_CONTENT_ENTRY(const SHARED_CONTENT_ENTRY&) = default;

      /*
       Move Constructor.
       */
      SHARED_CONTENT_ENTRY(SHARED_CONTENT_ENTRY&&) = default;

      /*
       Destructor.
       */
      virtual ~SHARED_CONTENT_ENTRY() noexcept = default;

      /*
       Returns true if the shared file is a vendor specific file.
       Returns false if the shared file is a QGL content file.
       This is case insensitive. 
       */
      bool vendor_file() const noexcept;

      /*
       Returns a const reference to the relative file path. This gets 
       processed by a content store.
       */
      const std::wstring& path() const noexcept;

      friend void swap(SHARED_CONTENT_ENTRY& first,
                       SHARED_CONTENT_ENTRY& second) noexcept
      {
         using std::swap;
         swap(first.m_metadataStr, second.m_metadataStr);
         swap(first.m_relativePath, second.m_relativePath);
      }

      /*
       Assignment operator.
       */
      SHARED_CONTENT_ENTRY& operator=(SHARED_CONTENT_ENTRY r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Equality operator.
       */
      friend bool operator==(const SHARED_CONTENT_ENTRY& r,
                             const SHARED_CONTENT_ENTRY& l) noexcept
      {
         return r.m_metadataStr == l.m_metadataStr &&
            r.m_relativePath == l.m_relativePath;
      }

      private:

      /*
       Use wide string because that is what Windows uses.
       */
      std::wstring m_relativePath;

      /*
       The first character in the metadata is either a 'Q' or 'V'. 'Q'
       indicates the shared file is a QGL content file. 'V' indicates the
       shared file is vendor specified.When a vendor specified file is loaded
       by QGL, the file's bytes are passed to an appropiate content importer.
       */
      std::wstring m_metadataStr;
   };
}