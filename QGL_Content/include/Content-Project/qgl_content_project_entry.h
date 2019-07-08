#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Buffers/qgl_content_metadata_buffer.h"

namespace qgl::content::helpers
{
   class QGL_CONTENT_API content_project_entry
   {
      public:
      /*
       Constructs a project entry with the given metadata and absolute file 
       path. The contents of the pointers are copied and can go out of scope.
       */
      content_project_entry(const CONTENT_METADATA_BUFFER* meta_p,
                            const wchar_t* absPath);

      /*
       Copy constructor
       */
      content_project_entry(const content_project_entry& r);

      /*
       Move constructor
       */
      content_project_entry(content_project_entry&& r);

      /*
       Destructor
       */
      ~content_project_entry() noexcept;

      /*
       Returns a pointer to the metadata for this entry.
       */
      const CONTENT_METADATA_BUFFER* const_metadata() const noexcept;

      /*
       Returns a pointer to this entry's absolute file path.
       */
      const wchar_t* const_path() const noexcept;

      /*
       Returns the number of characters, not including the null terminator, in 
       the path.
       */
      size_t path_size() const noexcept;

      /*
       Swaps the contents of l and r.
       */
      inline friend void swap(content_project_entry& l,
                              content_project_entry& r) noexcept
      {
         using std::swap;
         swap(l.m_meta, r.m_meta);
         swap(l.m_absPath, r.m_absPath);
         swap(l.m_pathLen, r.m_pathLen);
      }

      /*
       Copy assign.
       */
      inline content_project_entry& operator=(content_project_entry r)
      {
         swap(*this, r);
         return *this;
      }

      private:
      CONTENT_METADATA_BUFFER m_meta;
      wchar_t* m_absPath = nullptr;
      size_t m_pathLen;
   };
}