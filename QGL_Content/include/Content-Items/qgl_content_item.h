#pragma once
#include "include/qgl_content_include.h"
#include "include/qgl_content_types.h"

namespace qgl::content
{
   using content_id = uint64_t;

   /*
    In-engine objects that need to retain content metadata should inherit this.
    Examples include a texture or geometry loaded from a file.
    */
   class QGL_CONTENT_API content_item
   {
      public:

      /*
       Constructor.
       */
      content_item(const wchar_t* name,
                   content_id id,
                   RESOURCE_TYPES rType,
                   CONTENT_LOADER_IDS loaderID);

      /*
       Copy constructor.
       */
      content_item(const content_item& r);

      /*
       Move constructor.
       */
      content_item(content_item&& r);

      /*
       Destructor.
       */
      virtual ~content_item() noexcept;

      /*
       Returns this content's ID.
       */
      content_id id() const;

      /*
       Returns a const pointer to this content's name.
       */
      const wchar_t* name() const;

      /*
       Returns this content's type of resource.
       */
      RESOURCE_TYPES resource_type() const;

      /*
       Returns the loader ID this content uses.
       */
      CONTENT_LOADER_IDS loader_id() const;

      friend bool operator==(const content_item& l,
                             const content_item& r) noexcept
      {
         return l.m_id == r.m_id &&
            l.m_loaderID == r.m_loaderID &&
            l.m_rType == r.m_rType &&
            (wcscmp(r.name(), l.name()) == 0);
      }

      private:
      /*
       This content's name.
       */
      wchar_t* m_name;

      /*
       This content's ID.
       */
      content_id m_id;

      /*
       This content's resource type.
       */
      RESOURCE_TYPES m_rType;

      /*
       This content's loader ID.
       */
      CONTENT_LOADER_IDS m_loaderID;
   };
}