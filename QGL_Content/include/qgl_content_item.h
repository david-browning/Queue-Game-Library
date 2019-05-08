#pragma once
#include "include/qgl_content_include.h"
#include "include/qgl_content_types.h"

namespace qgl::content
{
   using content_id = uint64_t;

   template<typename ContentCharT, typename IDT = content_id>
   class LIB_EXPORT content_item
   {
      public:
      using id_t = IDT;
      using str_t = std::basic_string<ContentCharT>;
      content_item(const str_t& name, const IDT& id,
                   RESOURCE_TYPES rType, CONTENT_LOADER_IDS loaderID) :
         m_id(id),
         m_name(name),
         m_loaderID(loaderID),
         m_rType(rType)
      {

      }

      content_item(const content_item& r) = default;

      content_item(content_item&& r) = default;

      virtual ~content_item() = default;

      inline const IDT& id() const
      {
         return  m_id;
      }

      inline const str_t& name() const
      {
         return m_name;
      }

      inline const RESOURCE_TYPES resource_type() const
      {
         return m_rType;
      }

      inline const CONTENT_LOADER_IDS loader_id() const
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
      #pragma warning(push)
      #pragma warning(disable: 4251)
      str_t m_name;
      #pragma warning(pop)

      IDT m_id;
      RESOURCE_TYPES m_rType;
      CONTENT_LOADER_IDS m_loaderID;
   };

   using wcontent_item = content_item<wchar_t, content_id>;
}