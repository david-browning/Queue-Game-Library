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
                   CONTENT_LOADER_IDS loaderID) :
         m_id(id),
         m_name(name),
         m_loaderID(loaderID),
         m_rType(rType)
      {

      }

      /*
       Copy constructor.
       */
      content_item(const content_item& r) = default;

      /*
       Move constructor.
       */
      content_item(content_item&& r) = default;

      /*
       Destructor.
       */
      virtual ~content_item() noexcept = default;

      /*
       Returns this content's ID.
       */
      content_id id() const
      {
         return  m_id;
      }

      /*
       Returns a const reference to this content's name.
       */
      const std::wstring& name() const
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

      content_item operator=(const content_item& r)
      {
         if (this != &r)
         {
            m_id = r.m_id;
            m_name = r.m_name;
            m_rType = r.m_rType;
            m_loaderID = r.m_loaderID;
         }

         return *this;
      }

      private:
      /*
       This content's name.
       */
      std::wstring m_name;

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