#include "pch.h"
#include "include/Content-Store/qgl_content_store.h"

namespace qgl::content
{
   /*
    Unary predicate used to check if an iterator reference contains the given
    resource type.
    */
   struct FIND_WITH_RESOURCE_TYPE_UNARY_PREDICATE
   {
      FIND_WITH_RESOURCE_TYPE_UNARY_PREDICATE(RESOURCE_TYPES criterea) :
         m_criterea(criterea)
      {

      }

      bool operator()(const qgl::content::content_store::iterator_ref t) const
      {
         return t.metadata().resource_type() == m_criterea;
      }

      private:
      RESOURCE_TYPES m_criterea;
   };

   content_store::content_store(const content_store_config& storeConfig) :
      m_config(storeConfig)
   {

   }

   content_store::id_t content_store::file_id(
      const FileStringT& relativePath) const
   {
      if (m_fileNameIDMap.count(relativePath) > 0)
      {
         return m_fileNameIDMap.at(relativePath);
      }

      return content_store::INVALID_ID;
   }

   content_store::FileStringT content_store::abs_path(
      const FileStringT& relativePath) const
   {
      return m_config.path() + relativePath;
   }

   content_store::iterator content_store::begin()
   {
      return m_contentList.begin();
   }

   content_store::iterator content_store::end()
   {
      return m_contentList.end();
   }

   content_store::const_iterator content_store::cbegin() const
   {
      return m_contentList.cbegin();
   }

   content_store::const_iterator content_store::cend() const
   {
      return m_contentList.cend();
   }

   content_store::iterator content_store::find_with(iterator first, 
                                                    iterator last, 
                                                    RESOURCE_TYPES type) const
   {
      return std::find_if(first, 
                          last, 
                          FIND_WITH_RESOURCE_TYPE_UNARY_PREDICATE(type));
   }
}