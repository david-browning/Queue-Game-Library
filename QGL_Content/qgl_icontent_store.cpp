#include "pch.h"
#include "include/Interfaces/qgl_icontent_store.h"

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

      virtual void map(RESOURCE_TYPES rID,
                       CONTENT_LOADER_IDS lID,
                       load_function fn)
      {
         auto h = hash(rID, lID);
         m_loadFunctionMap[h] = fn;
      }

      virtual id_t load(const wchar_t* relative)
      {
         //Check if the file already has an ID.
         if (m_fileIDMap.count(relative) > 0)
         {
            //Return the ID if the path is already in the store.
            return m_fileIDMap[relative];
         }

         //Get and then increment the ID for the content we are loading.
         id_t retID = m_nextID.fetch_add(1);

         //Open the content file.
         //This loads the file's header and dictionary.
         auto absPath = abs_path(relative);
         auto f = qgl::make_shared(qgl_open_content_file(
            relative,
            m_version))->as<icontent_file>();

         const auto meta = f->header()->metadata();

         //Look up the loader using the content file's header.
         auto loaderHash = hash(meta->resource_type(), meta->loader_id());
         if (m_loadFunctionMap.count(loaderHash) <= 0)
         {
            throw std::logic_error("No valid loader is mapped.");
         }
         auto loaderFn = m_loadFunctionMap.at(loaderHash);

         //Load the content using the file loader.
         auto contentPtr = loaderFn(f, retID);

         //Save the unique pointer in the map.
         //Map the file to the id.
         m_IDContentMap[retID] = std::make_optional(std::move(contentPtr));
         m_fileIDMap[relative] = retID;
         m_fileIDMapReverse[retID] = relative;

         //Return the loaded content's ID.
         return retID;
      }

      virtual void unload(id_t id)
      {
         if (m_IDContentMap.count(id) <= 0)
         {
            return;
         }

         //Content pointers are wrapped in an optional so they can be easily
         //destructed without changing the container.

         //Destroy the optional's data.
         m_IDContentMap[id].reset();

         //Remove the file path mapping.
         file_string path = m_fileIDMapReverse.at(id);
         m_fileIDMap.erase(path);
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

      qgl_version_t m_version;
   };

   icontent_store* qgl_create_content_store(const wchar_t* storePath,
                                            qgl_version_t v)
   {
      switch (v)
      {
         case QGL_VERSION_0_1_WIN:
         case QGL_VERSION_0_2_WIN:
         {
            return new content_store_1_0(storePath, v);
         }
      }

      return nullptr;
   }
}