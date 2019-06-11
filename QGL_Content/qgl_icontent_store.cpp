#include "pch.h"
#include "include/Interfaces/qgl_icontent_store.h"
#include <execution>

namespace qgl::content
{
   struct content_store_1_0 : public icontent_store
   {
      using resource_loader_hash_t = uint32_t;
      resource_loader_hash_t hash(RESOURCE_TYPES r,
                                  CONTENT_LOADER_IDS c)
      {
         return (static_cast<uint32_t>(r) << (CHAR_BIT * sizeof(r))) |
            static_cast<uint32_t>(c);
      }

      content_store_1_0(const wchar_t* storePath,
                        qgl_version_t v) :
         m_rootPath(storePath),
         m_version(v)
      {
         m_nextID.store(0);
      }

      content_store_1_0(const content_store_1_0&) = default;

      content_store_1_0(content_store_1_0&&) = default;

      virtual ~content_store_1_0() noexcept
      {

      }

      virtual void release()
      {
         //Calls the destructor.
         delete this;
      }

      virtual id_t reserve(const wchar_t* relative)
      {
         //Check if the file already has an ID.
         id_t retID = 0;
         if (m_fileIDMap.count(relative) > 0)
         {
            retID = m_fileIDMap.at(relative);
         }
         else
         {
            //Lock the mutex until the function finishes.
            std::lock_guard<std::mutex>lock(m_mappingMutex);

            //File not mapped. Map it to a new ID.
            retID = m_nextID.fetch_add(1);
            m_fileIDMap[relative] = retID;
            m_fileIDMapReverse[retID] = relative;
         }

         return retID;
      }

      virtual bool loaded(id_t id) const
      {
         return m_IDContentMap.count(id) > 0 &&
            m_IDContentMap.at(id).has_value();
      }

      virtual void map(RESOURCE_TYPES rID,
                       CONTENT_LOADER_IDS lID,
                       load_function fn)
      {
         auto h = hash(rID, lID);

         //Lock the mutex until done mapping
         std::lock_guard<std::mutex>lock(m_mappingMutex);

         m_loadFunctionMap[h] = fn;
      }

      virtual id_t load(const wchar_t* relative)
      {
         //Get an ID for the file.
         auto retID = reserve(relative);

         //The id may have just been reserved or it was previously unloaded.
         //Check if the ID maps to loaded content. The content is loaded if the
         //optional has a value. When unloaded, the optional value is destroyed.

         //If the content is already loaded:
         if (m_IDContentMap.count(retID) > 0 &&
             m_IDContentMap.at(retID).has_value())
         {
            //Return the ID for the already loaded content.
            return retID;
         }
         //Else, continue to load the content.

         //Open the content file.
         //This loads the file's header and dictionary.
         auto absPath = abs_path(relative);
         icontent_file* filePtr = nullptr;
         winrt::check_hresult(qgl_open_content_file(relative, 
                                                    m_version,
                                                    &filePtr));
         auto fileSafe = qgl::make_unique<icontent_file>(filePtr);
         
         const auto meta = fileSafe->header()->metadata();

         //Look up the loader using the content file's header.
         auto loaderHash = hash(meta->resource_type(), meta->loader_id());
         if (m_loadFunctionMap.count(loaderHash) <= 0)
         {
            throw std::logic_error("No valid loader is mapped.");
         }
         auto loaderFn = m_loadFunctionMap.at(loaderHash);

         //Load the content using the file loader.
         auto contentPtr = loaderFn(fileSafe.get(), retID);

         //Lock the mutex until done mapping
         std::lock_guard<std::mutex>lock(m_mappingMutex);

         //Save the unique pointer in the map.
         m_IDContentMap[retID] = std::make_optional(std::move(contentPtr));

         //Return the loaded content's ID.
         return retID;
      }

      virtual void queue_load(const wchar_t* relative)
      {
         std::lock_guard lock(m_queueMutex);
         m_pendingLoads.push_back(relative);
      }

      virtual thread_handle_t flush_loads()
      {
         std::thread backgroundThread(
            &content_store_1_0::flush_loads_thread,
            this);

         return backgroundThread.native_handle();
      }

      virtual void unload(id_t id)
      {
         //Do nothing if the id is not in the content map.
         if (m_IDContentMap.count(id) <= 0)
         {
            return;
         }

         //Lock the mutex until done unloading
         std::lock_guard<std::mutex>lock(m_mappingMutex);

         //Content pointers are wrapped in an optional so they can be easily
         //destructed without changing the container.

         //Destroy the optional's data. The load function checks if the 
         //optional has a value to determine if the content is already loaded.
         m_IDContentMap[id].reset();

         //Do not remove the file mapping, in case we load the same file 
         //again. 
         //file_string path = m_fileIDMapReverse.at(id);
         //m_fileIDMap.erase(path);
         //m_fileIDMapReverse.erase(id);
      }

      virtual const content_item* get(id_t id) const
      {
         if (m_IDContentMap.count(id) > 0)
         {
            auto& opt = m_IDContentMap.at(id);
            if (opt.has_value())
            {
               return opt.value().get();
            }
            else
            {
               throw std::logic_error("The content is not loaded.");
            }
         }
         else
         {
            throw std::runtime_error("The ID is not mapped.");
         }
      }

      virtual content_item* get(id_t id)
      {
         if (m_IDContentMap.count(id) > 0)
         {
            auto& opt = m_IDContentMap.at(id);
            if (opt.has_value())
            {
               return opt.value().get();
            }
            else
            {
               throw std::logic_error("The content is not loaded.");
            }
         }
         else
         {
            throw std::runtime_error("The ID is not mapped.");
         }
      }

      private:
      file_string abs_path(const wchar_t* relativePath) const
      {
         return m_rootPath + file_string(relativePath);
      }

      void flush_loads_thread()
      {
         //Do not allow any queuing while this is running.
         std::lock_guard lock(m_queueMutex);

         //Call load on each string in the pending list.
         //Do it in parallel.
         std::for_each(std::execution::par,
                       m_pendingLoads.begin(),
                       m_pendingLoads.end(),
                       [&](const file_string& p)
         {
            load(p.c_str());
         });

         m_pendingLoads.clear();
      }

      /*
       This ends with a slash.
       */
      file_string m_rootPath;

      /*
       Next ID to give to content.
       */
      std::atomic<id_t> m_nextID;

      /*
       Maps a content ID to content.
       */
      std::unordered_map<id_t, std::optional<content_ptr_t>> m_IDContentMap;

      /*
       Maps a loader ID to a loader.
       */
      std::unordered_map<resource_loader_hash_t,
         load_function> m_loadFunctionMap;

      std::unordered_map<file_string, id_t> m_fileIDMap;

      std::unordered_map<id_t, file_string> m_fileIDMapReverse;

      std::mutex m_mappingMutex;

      std::mutex m_queueMutex;

      qgl_version_t m_version;

      std::vector<file_string> m_pendingLoads;
   };
   
   HRESULT qgl_create_content_store(const wchar_t* storePath, 
                                            qgl_version_t v, 
                                            icontent_store** out_p)
   {
      if (out_p == nullptr)
      {
         return E_INVALIDARG;
      }

      icontent_store* ret = nullptr;

      switch (std::hash<qgl_version_t>{}(v))
      {
         case qgl::hashes::VERSION_0_1_HASH:
         case qgl::hashes::VERSION_0_2_HASH:
         {
            ret = new(std::nothrow) content_store_1_0(storePath, v);
            break;
         }
         default:
         {
            return E_NOINTERFACE;
         }
      }

      if (ret == nullptr)
      {
         return E_OUTOFMEMORY;
      }

      *out_p = ret;
      return S_OK;
   }
}