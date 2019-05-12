#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Store/qgl_content_store_config.h"

namespace qgl::content
{
   /*
    Provides the mechanism to import content from a repository. Tracks imports
    so content is not loaded from disk more than once.
    After loading, content pointers can be accessed by using a content ID.
    The content ID is assigned when a content file is loaded.
    */
   LIB_EXPORT class content_store
   {
      public:
      using FileStringT = winrt::hstring;
      using IDT = uint64_t;
      IDT INVALID_ID = static_cast<IDT>(-1);

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
      content_store(content_store&&) = default;

      /*
       Destructor.
       */
      virtual ~content_store() = default;

      /*
       Returns an ID for a content file. Use this to lookup a content's ID when
       you have the content's relative file path.
       If the file has not been loaded into the store, this returns
       content_store::INVALID_ID.
       */
      IDT file_id(const FileStringT& relativePath);

      /*
       Loads the content from the relative path and returns an ID that can be
       used to lookup a shared_ptr to the loaded item.
       LoadingUnaryPredicate: Unary predicate takes a const content_file_read& 
       and returns a shared_ptr to content. See string_file_loader for an 
       example.
       */
      template<class LoadingUnaryPredicate>
      IDT load(const FileStringT& relativePath, LoadingUnaryPredicate pred)
      {
         //Check if the file already has an ID.
         auto fileID = file_id(relativePath);
         if (fileID != INVALID_ID)
         {
            //Return the ID if the path is already in the store.
            return fileID;
         }

         //Get and then increment the ID for the content we are loading.
         IDT retID = m_nextID.fetch_add(1);

         //Map the content path to the content's ID.
         m_fileNameIDMap[relativePath] = retID;

         //Open the content file.
         //This loads the file's header and dictionary.
         content_file_read f(abs_path(relativePath));

         //Use the loader predicate to load the file.
         //Store the shared_ptr in the map of ID->shared_ptr
         m_IDContentMap[retID] = pred(f);
      }

      /*
       Returns a shared_ptr to content. Throws an exception if content ID is 
       not mapped in the store.
       */
      template<class T>
      std::shared_ptr<T> shared_get(IDT contentID)
      {
         return std::reinterpret_pointer_cast<T, void>(
            m_IDContentMap.at(contentID));
      }

      /*
       Returns a const pointer to to content. Throws an exception if the content
       ID is not mapped in the store.
       */
      template<class T>
      const T* get(IDT contentID)
      {
         return static_cast<T*>(m_IDContentMap[contentID].get());
      }

      /*
       Returns the absolute file path for a given path relative to the content
       store.
       */
      FileStringT abs_path(const FileStringT& relativePath) const;

      private:
      /*
       Next ID to give to content.
       */
      std::atomic<IDT> m_nextID;

      /*
       Content store configuration.
       */
      content_store_config m_config;

      /*
       Maps a file name to a content ID.
       */
      std::unordered_map<FileStringT, IDT> m_fileNameIDMap;

      /*
       Maps a content ID to the content.
       */
      std::unordered_map<IDT, std::shared_ptr<void>> m_IDContentMap;
   };
}