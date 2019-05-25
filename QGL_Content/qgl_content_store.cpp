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
      using iterator_ref = content_store::iterator::reference;

      FIND_WITH_RESOURCE_TYPE_UNARY_PREDICATE(RESOURCE_TYPES criterea) :
         m_criterea(criterea)
      {

      }

      bool operator()(const iterator_ref t) const
      {
         return t.metadata().resource_type() == m_criterea;
      }

      private:
      RESOURCE_TYPES m_criterea;
   };

   struct content_store::impl
   {
      public:
      impl(const content_store_config& storeConfig) :
         m_config(storeConfig)
      {

      }

      impl(const impl&) = delete;

      impl(impl&& m) = default;

      virtual ~impl() noexcept = default;

      id_t file_id(const file_string& relativePath) const
      {
         if (m_fileNameIDMap.count(relativePath) > 0)
         {
            return m_fileNameIDMap.at(relativePath);
         }

         return INVALID_CONTENT_ID;
      }

      file_string abs_path(const file_string& relativePath) const
      {
         return m_config.path() + relativePath;
      }

      #pragma region pointer getters
      const void* get(id_t contentID)
      {
         return shared_get(contentID).get();
      }

      std::shared_ptr<void> shared_get(id_t contentID)
      {
         auto idx = m_IDContentMap.at(contentID);
         return m_contentList[idx].shared_get();
      }
      #pragma endregion

      void put(id_t contentID,
               const file_string& relativePath,
               const content_accessor& accessor)
      {
         m_fileNameIDMap[relativePath] = contentID;
         m_contentList.push_back(accessor);
         auto idx = m_contentList.size() - 1;
         m_IDContentMap[contentID] = idx;
      }

      id_t next_id()
      {
         return m_nextID.fetch_add(1);
      }

      #pragma region iterators
      iterator begin()
      {
         return m_contentList.begin();
      }

      iterator end()
      {
         return m_contentList.end();
      }

      const_iterator cbegin() const
      {
         return m_contentList.cbegin();
      }

      const_iterator cend() const
      {
         return m_contentList.cend();
      }
      #pragma endregion

      #pragma region content searching
      iterator find_with(iterator first,
                         iterator last,
                         RESOURCE_TYPES type) const
      {
         return std::find_if(first,
                             last,
                             FIND_WITH_RESOURCE_TYPE_UNARY_PREDICATE(type));
      }
      #pragma endregion

      private:
      /*
       Content store configuration.
       */
      content_store_config m_config;

      /*
       Next ID to give to content.
       */
      std::atomic<id_t> m_nextID;

      /*
       Maps a relative file path to a content ID.
       */
      std::unordered_map<file_string, id_t> m_fileNameIDMap;

      /*
       Maps a content ID an index. The index is used to lookup the
       content_accessor from the content list.
       */
      std::unordered_map<id_t, size_t> m_IDContentMap;

      content_list_t m_contentList;
   };

   content_store::content_store(const content_store_config& storeConfig) :
      m_impl_p(new impl(storeConfig))
   {
   }

   content_store::content_store(content_store&& m) :
      m_impl_p(m.m_impl_p)
   {
      m.m_impl_p = nullptr;
   }

   content_store::~content_store()
   {
      delete m_impl_p;
   }

   id_t content_store::file_id(
      const file_string& relativePath) const
   {
      return m_impl_p->file_id(relativePath);
   }

   file_string content_store::abs_path(const file_string& relativePath) const
   {
      return m_impl_p->abs_path(relativePath);
   }

   content_store::iterator content_store::begin()
   {
      return m_impl_p->begin();
   }

   content_store::iterator content_store::end()
   {
      return m_impl_p->end();
   }

   content_store::const_iterator content_store::cbegin() const
   {
      return m_impl_p->cbegin();
   }

   content_store::const_iterator content_store::cend() const
   {
      return m_impl_p->cend();
   }

   content_store::iterator content_store::find_with(iterator first, 
                                                    iterator last, 
                                                    RESOURCE_TYPES type) const
   {
      return m_impl_p->find_with(first, last, type);
   }
   
   const void* content_store::p_get(id_t contentID)
   {
      return m_impl_p->get(contentID);
   }

   std::shared_ptr<void> content_store::p_shared_get(id_t contentID)
   {
      return m_impl_p->shared_get(contentID);
   }

   void content_store::p_put(id_t contentID, 
                             const file_string& relativePath,
                             const content_accessor& accessor)
   {
      m_impl_p->put(contentID, relativePath, accessor);
   }

   id_t content_store::p_nextID()
   {
      return m_impl_p->next_id();
   }
}