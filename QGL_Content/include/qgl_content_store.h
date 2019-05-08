#pragma once
#include "qgl_content_include.h"
#include "qgl_content_item.h"
#include "qgl_file_helpers.h"
#include "qgl_iimporter.h"
#include "qgl_content_file.h"
#include "qgl_content_store_config.h"

namespace qgl::content
{
   /*
    Constructor checks if the content store exists on the disk and throws an exception if
    it doesn't.
    */
   template<typename ContentCharT, typename IDT>
   class content_store
   {
      public:
      using FileStringT = winrt::hstring;
      IDT INVALID_ID = static_cast<IDT>(-1);

      content_store(const content_store_config& storeConfig) :
         m_config(storeConfig)
      {

      }

      content_store(const content_store&) = delete;

      content_store(content_store&&) = default;

      virtual ~content_store() = default;

      IDT file_id(const FileStringT& relativePath)
      {
         //Check if the file already has an ID.
         if (m_fileNameIDMap.count(relativePath) != 0)
         {
            //Return the ID if the path is already in the map.
            return m_fileNameIDMap[relativePath];
         }
         else
         {
            return INVALID_ID;
         }
      }

      /*
       Loads the content from the relative path and returns an ID that can be used to lookup
       a shared_ptr to the loaded item.
       */
      template<class LoadingUnaryPredicate>
      IDT load(const FileStringT& relativePath,
               LoadingUnaryPredicate pred)
      {
         //Check if the file already has an ID.
         auto fileID = file_id(relativePath);
         if (fileID != INVALID_ID)
         {
            //Return the ID if the path is already in the map.
            return fileID;
         }

         //Get and then increment the ID for the content we are loading.
         IDT retID = m_nextID.fetch_add(1);

         //Map the content path to the content's ID.
         m_fileNameIDMap[relativePath] = retID;

         //Open the content file.
         //This loads the file's header and dictionary.
         content_file f(abs_path(relativePath));

         //Use the loader predicate to load the file.
         //Store the shared_ptr in the map of ID->shared_ptr
         m_IDContentMap[retID] = pred(f);
      }

      template<typename LoadT>
      IDT load(const FileStringT& relativePath,
               std::function<std::shared_ptr<LoadT>(const content_file&)> loadFunction)
      {
         //Check if the file already has an ID.
         auto fileID = file_id(relativePath);
         if (fileID != INVALID_ID)
         {
            //Return the ID if the path is already in the map.
            return fileID;
         }

         //Get and then increment the ID for the content we are loading.
         IDT retID = m_nextID.fetch_add(1);

         //Map the content path to the content's ID.
         m_fileNameIDMap[relativePath] = retID;

         //Open the content file.
         //This loads the file's header and dictionary.
         content_file f(abs_path(relativePath));

         //Use the loader predicate to load the file.
         //Store the shared_ptr in the map of ID->shared_ptr
         m_IDContentMap[retID] = loadFunction(f);
      }

      template<class T>
      std::shared_ptr<T> shared_get(IDT contentID)
      {
         return std::shared_ptr<T>(static_cast<T*>(m_IDContentMap[contentID].get()));
      }

      template<class T>
      const T* get(IDT contentID)
      {
         return static_cast<T*>(m_IDContentMap[contentID].get());
      }

      FileStringT abs_path(const FileStringT& relativePath) const
      {
         return m_config.path() + relativePath;
      }

      private:
      std::atomic<IDT> m_nextID;

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