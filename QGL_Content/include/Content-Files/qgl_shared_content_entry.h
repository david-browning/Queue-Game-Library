#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content::entries
{
   /*
    Shared content is a path. This path is an encoded relative path to where
    the shared content file is stored. The path is encoded as metadata and a
    relative path, separated by a colon.
    The shared path must be converted to a content file using a content store.
    */
   class shared_content_entry
   {
      public:
      shared_content_entry();

      /*
       Parses the string's metadata and copies the relative path to internal
       storage. 
       Throws std::invalid_argument is the string is not the correct form.
       */
      shared_content_entry(const wchar_t* str);

      /*
       Copy Constructor.
       */
      shared_content_entry(const shared_content_entry&) = default;

      /*
       Move Constructor.
       */
      shared_content_entry(shared_content_entry&&) = default;

      /*
       Destructor.
       */
      ~shared_content_entry() noexcept = default;

      /*
       Returns true if the shared file is a vendor specific file.
       Returns false if the shared file is a QGL content file.
       This is case insensitive. 
       */
      bool vendor_file() const noexcept;

      /*
       Returns the number of characters in the buffer.
       */
      size_t size() const noexcept;

      const wchar_t* buffer() const noexcept;

      const wchar_t* relative_path() const noexcept;

      friend void swap(shared_content_entry& first,
                       shared_content_entry& second) noexcept
      {
         using std::swap;
         swap(first.m_metadataStr, second.m_metadataStr);
         swap(first.m_relativeStr, second.m_relativeStr);
         swap(first.m_completeStr, second.m_completeStr);
      }

      /*
       Assignment operator.
       */
      shared_content_entry& operator=(shared_content_entry r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Equality operator.
       */
      friend bool operator==(const shared_content_entry& r,
                             const shared_content_entry& l) noexcept
      {
         return r.m_completeStr == l.m_completeStr;
      }

      private:
      std::wstring m_completeStr;

      /*
       The first character in the metadata is either a 'Q' or 'V'. 'Q'
       indicates the shared file is a QGL content file. 'V' indicates the
       shared file is vendor specified.When a vendor specified file is loaded
       by QGL, the file's bytes are passed to an appropriate content importer.
       */
      std::wstring m_metadataStr;

      /*
       Use wide string because that is what Windows uses.
       */
      std::wstring m_relativeStr;
   };
}