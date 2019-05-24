#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Store/qgl_content_store_config.h"
#include "include/Content-Store/qgl_content_store_accessor.h"
#include "include/File-Loaders/qgl_file_loader_content_store_config.h"

namespace qgl::content
{
   QGL_CONTENT_TEMPLATE template class QGL_CONTENT_API 
      std::vector<content_accessor<uint64_t>, 
      std::allocator<content_accessor<uint64_t>>>;

   /*
    Provides the mechanism to import content from a repository. Tracks imports
    so content is not loaded from disk more than once.
    After loading, content pointers can be accessed by using a content ID.
    The content ID is assigned when a content file is loaded.
    */
   class QGL_CONTENT_API content_store
   {
      public:
      using id_t = uint64_t;
      static constexpr id_t INVALID_ID = static_cast<id_t>(-1);
      using content_list_t = std::vector<content_accessor<id_t>>;
      using iterator = content_list_t::iterator;
      using const_iterator = content_list_t::const_iterator;

      /*
       Creates a content store using the configuration.
       */
      content_store(const content_store_config& storeConfig);

      /*
       Cannot copy a content store.
       */
      content_store(const content_store&) = delete;

      /*
       Move constructor.
       */
      content_store(content_store&&);

      /*
       Destructor.
       */
      virtual ~content_store();

      /*
       Returns an ID for a content file. Use this to lookup a content's ID when
       you have the content's relative file path.
       If the file has not been loaded into the store, this returns
       content_store::INVALID_ID.
       */
      id_t file_id(const file_string& relativePath) const;

      /*
       Returns the absolute file path for a given path relative to the content
       store.
       */
      file_string abs_path(const file_string& relativePath) const;

      /*
       Loads the content from the relative path and returns an ID that can be
       used to lookup a shared_ptr to the loaded item.
       LoadingUnaryPredicate: Unary predicate takes a const content_file_read&
       and returns a shared_ptr to content. See string_file_loader for an
       example.
       */
      template<class LoadingUnaryPredicate>
      id_t load(const file_string& relativePath, LoadingUnaryPredicate pred)
      {
         //Check if the file already has an ID.
         auto fileID = file_id(relativePath);
         if (fileID != INVALID_ID)
         {
            //Return the ID if the path is already in the store.
            return fileID;
         }

         //Get and then increment the ID for the content we are loading.
         id_t retID = p_nextID();
         
         //Open the content file.
         //This loads the file's header and dictionary.
         content_file f(abs_path(relativePath));

         //Use the loader predicate to load the file.
         //Store the shared_ptr in the map of ID->shared_ptr
         auto data = content_accessor<id_t>(retID,
                                            f.header().metadata(),
                                            pred(f));
         p_put(retID, relativePath, data);

         return retID;
      }

      /*
       Returns a shared_ptr to content. Throws an exception if content ID is
       not mapped in the store.
       */
      template<class T>
      std::shared_ptr<T> shared_get(id_t contentID)
      {
         return std::reinterpret_pointer_cast<T, void>(p_shared_get(contentID));
      }

      /*
       Returns a const pointer to content. Throws an exception if the content
       ID is not mapped in the store.
       */
      template<class T>
      const T* get(id_t contentID)
      {
         return static_cast<const T*>(p_get(contentID));
      }

      /*
       Returns an iterator to the begining of all loaded content.
       */
      iterator begin();
           
      /*
       Returns an iterator to the end of all loaded content.
       */
      iterator end();

      /*
       Returns a const iterator to the begining of all loaded content.
       */
      const_iterator cbegin() const;

      /*
       Returns a const iterator to the end of all loaded content.
       */
      const_iterator cend() const;

      /*
       Searches through all loaded content until it finds content who's
       resource type matches type.
       Returns an iterator to that content.
       To keep searching forward, call this again using the return value as
       the first iterator.
       */
      iterator find_with(iterator first,
                         iterator last,
                         RESOURCE_TYPES type) const;

      private:
      struct impl;
      impl* m_impl_p;

      const void* p_get(id_t contentID);

      std::shared_ptr<void> p_shared_get(id_t contentID);

      /*
       Maps a content ID to the content path.
       Maps a content ID to a content_accessor
       */
      void p_put(id_t contentID,
                 const file_string& relativePath,
                 const content_accessor<id_t>& accessor);

      id_t p_nextID();
   };
}