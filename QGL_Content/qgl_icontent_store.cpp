#include "pch.h"
#include "include/Interfaces/qgl_icontent_store.h"
#include <execution>

namespace qgl::content
{
   class content_store_1_0 : public icontent_store
   {
      public:
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

      virtual id_t reserve(const wchar_t* relative) noexcept
      {
         //Check if the file already has an ID.
         id_t retID = INVALID_CONTENT_ID;
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
         }

         return retID;
      }

      virtual bool loaded(id_t id) const noexcept
      {
         return m_IDContentMap.count(id) > 0 &&
            m_IDContentMap.at(id).has_value();
      }

      virtual void map(RESOURCE_TYPES rID,
                       CONTENT_LOADER_IDS lID,
                       load_function fn) noexcept
      {
         auto h = hash(rID, lID);

         //Lock the mutex until done mapping
         std::lock_guard<std::mutex>lock(m_mappingMutex);

         m_loadFunctionMap[h] = fn;
      }

      virtual HRESULT load(const wchar_t* relative,
                           id_t* id_p) noexcept
      {
         //Get an ID for the file.
         *id_p = reserve(relative);

         //The id may have just been reserved or it was previously unloaded.
         //Check if the ID maps to loaded content. The content is loaded if the
         //optional has a value. When unloaded, the optional value is destroyed.

         //If the content is already loaded:
         if (m_IDContentMap.count(*id_p) > 0 &&
             m_IDContentMap.at(*id_p).has_value())
         {
            //Return the ID for the already loaded content.
            #ifdef DEBUG
            OutputDebugString(L"This content ID is already mapped.");
            #endif
            return S_ALREADYMAPPED;
         }
         //Else, continue to load the content.

         //Open the content file.
         //This loads the file's header and dictionary.
         auto absPath = abs_path(relative);
         icontent_file* filePtr = nullptr;
         auto hr = qgl_open_content_file(absPath.c_str(),
                                         m_version,
                                         &filePtr);
         auto fileSafe = qgl::make_unique<icontent_file>(filePtr);
         if (FAILED(hr))
         {
            return hr;
         }

         const auto meta = fileSafe->header()->metadata();

         //Look up the loader using the content file's header.
         auto loaderHash = hash(meta->resource_type(), meta->loader_id());
         if (m_loadFunctionMap.count(loaderHash) <= 0)
         {
            return E_NOLOADER;
         }
         auto loaderFn = m_loadFunctionMap.at(loaderHash);

         //Load the content using the file loader.
         content_ptr_t contentPtr = nullptr;
         try
         {
            contentPtr = loaderFn(fileSafe.get(), *id_p);
         }
         catch (std::invalid_argument&)
         {
            //Most-likely, the file data was unexpected for the loader.
            return E_UNEXPECTED;
         }
         catch (...)
         {
            //Probably a file reading error.
            return E_HANDLE;
         }

         if (contentPtr == nullptr)
         {
            #ifdef DEBUG
            OutputDebugString(L"The content loader failed.");
            #endif
            return E_UNEXPECTED;
         }

         //Lock the mutex until done mapping
         std::lock_guard<std::mutex>lock(m_mappingMutex);

         //Save the unique pointer in the map.
         m_IDContentMap[*id_p] = std::make_optional(std::move(contentPtr));

         //Return the loaded content's ID.
         return S_OK;
      }

      virtual void queue_load(const wchar_t* relative) noexcept
      {
         std::lock_guard lock(m_queueMutex);
         m_pendingLoads.push_back(relative);
      }

      virtual thread_handle_t flush_loads() noexcept
      {
         std::thread backgroundThread(
            &content_store_1_0::flush_loads_thread,
            this);

         return backgroundThread.native_handle();
      }

      virtual void unload(id_t id) noexcept
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

      virtual const content_item* const_get(id_t id) const noexcept
      {
         if (m_IDContentMap.count(id) > 0)
         {
            auto& opt = m_IDContentMap.at(id);
            if (opt.has_value())
            {
               return opt.value().get();
            }
         }
       
         return nullptr;
      }

      virtual content_item* get(id_t id) noexcept
      {
         if (m_IDContentMap.count(id) > 0)
         {
            auto& opt = m_IDContentMap.at(id);
            if (opt.has_value())
            {
               return opt.value().get();
            }
         }
       
         return nullptr;
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
            //Allow this to get lost. The caller can get the ID later by using
            //reserve().
            id_t id; 
            auto hr = load(p.c_str(), &id);
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

      std::mutex m_mappingMutex;

      std::mutex m_queueMutex;

      qgl_version_t m_version;

      std::vector<file_string> m_pendingLoads;
   };

   HRESULT qgl_create_content_store(const wchar_t* storePath,
                                    qgl_version_t v,
                                    icontent_store** out_p) noexcept
   {
      if (out_p == nullptr || storePath == nullptr)
      {
         #ifdef DEBUG
         OutputDebugString(L"out_p cannot be nullptr.");
         #endif
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
            #ifdef DEBUG
            OutputDebugString(L"This QGL version is not supported.");
            #endif
            return E_NOINTERFACE;
         }
      }

      if (ret == nullptr)
      {
         #ifdef DEBUG
         OutputDebugString(L"Out of memory!");
         #endif
         return E_OUTOFMEMORY;
      }

      *out_p = ret;
      return S_OK;
   }
}