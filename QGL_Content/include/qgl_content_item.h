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
   class LIB_EXPORT content_item
   {
      public:

      /*
       Constructor.
       */
      content_item(const std::wstring& name,
                   const content_id& id,
                   RESOURCE_TYPES rType,
                   CONTENT_LOADER_IDS loaderID);

      content_item(const wchar_t* name,
                   size_t len,
                   const content_id& id,
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
      content_id id() const
      {
         return  m_id;
      }

      /*
       Returns a const pointer to this content's name.
       */
      const wchar_t* name() const
      {
         return m_name;
      }

      /*
       Returns this content's type of resource.
       */
      RESOURCE_TYPES resource_type() const
      {
         return m_rType;
      }

      /*
       Returns the loader ID this content uses.
       */
      CONTENT_LOADER_IDS loader_id() const
      {
         return m_loaderID;
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