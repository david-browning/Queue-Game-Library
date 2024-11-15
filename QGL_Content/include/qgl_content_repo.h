#pragma once
#include "include/qgl_content_include.h"
#include "include/Loaders/qgl_content_loader.h"
#include "include/Files/qgl_content_file_helpers.h"

namespace qgl::content
{
   struct content_repo_params
   {
      public:
      sys_str root;
      bool recurse;
      std::vector<sys_str> extensions;
      size_t pool;
   };

   struct content_load_params
   {
      public:
      uint8_t lod;
   };

   template<class FileHandle, typename TickT>
   class content_repo
   {
      public:
      content_repo(const content_repo_params& p)
      {
         load_store(p);
      }

      template<class LoaderIt>
      content_repo(const content_repo_params& p, LoaderIt first, LoaderIt last)
      {
         load_store(p);
         while (first != last)
         {
            insert_loader(*first);
            first++;
         }
      }

      content_repo(const content_repo&) = default;

      content_repo(content_repo&&) = default;

      virtual ~content_repo() noexcept = default;

      void insert_loader(const content_loader& l)
      {
         m_loaders[l.id()] = std::make_unique<content_loader>(l);
      }

      void insert_loader(iloader_metadata* meta_p)
      {
         qgl::guid g;
         meta_p->id(&g);
         m_loaders[g] = std::make_unique<content_loader>(meta_p);
      }

      /*
       Removes a loaded. If an item that references this loader is fetched,
       the fetch will fail.
       */
      void erase_loader(const qgl::guid& g)
      {
         m_loaders.erase(g);
      }

      /*
       Returns true if content with the given GUID has been fetched.
       */
      bool fetched(const qgl::guid& g) const noexcept;

      template<class T>
      T* get(const guid& g, 
             const content_load_params& p, 
             TickT elapsed);

      template<class T>
      const T* get(const guid& g,
                   const content_load_params& p, 
                   TickT elapsed) const;

      void evict(const guid& g, const content_load_params& p);

      void evict(const guid& g);
      
      /*
       Loads the content with the given GUID into the content store. If the 
       content has not been fetched or is in the process of being fetched, this
       will block until the content is loaded.
       */
      void fetch(const guid& g);

      /*
       Spawns a background thread that fetches the content with the given GUID.
       This function will return immediately. If the content is not fetched by 
       the time "get" is called, the "get" call will block.
       */
      void async_fetch(std::promise<void>&& p, const guid& g);

      private:
      struct search_handle_traits
      {
         using type = typename HANDLE;

         static void close(type value) noexcept
         {
            winrt::check_bool(FindClose(value));
         }

         static constexpr type invalid() noexcept
         {
            return INVALID_HANDLE_VALUE;
         }
      };

      struct load_traits
      {
         std::thread fetch_thread;

         /*
          True if the thread finished loading the content.
          */
         bool thead_done;

         /*
          Time the content was last accessed.
          */
         TickT last_accessed;
      };

      void load_store(content_repo_params p)
      {
         // Format the extensions.
         for (size_t i = 0; i < p.extensions.size(); i++)
         {
            p.extensions[i] = format_extention(p.extensions[i]);
         }

         load_dir(p.root, p);
      }

      void load_dir(const sys_str& dir, const content_repo_params& p)
      {
         auto searchPattern = format_dir(dir) + L"*";
         WIN32_FIND_DATA fileInfo;
         
         auto findH = FindFirstFileExFromAppW(
            searchPattern.c_str(),
            FINDEX_INFO_LEVELS::FindExInfoStandard,
            &fileInfo,
            FindExSearchNameMatch,
            nullptr,
            FIND_FIRST_EX_LARGE_FETCH);
         // Wrap the search handle so it is freed automatically.
         winrt::handle_type<search_handle_traits> searchH{findH};

         if (!searchH)
         {
            winrt::throw_last_error();
         }

         do
         {
            sys_str path{ dir + fileInfo.cFileName };
            
            // If its a directory.
            if (p.recurse &&
                fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
               if (path != L"." && path != L"..");
               {
                  load_dir(path, p);
               }

               // Skip . and .. directories.
            }
            else
            {
               // Its a file.
               // Check if the file has the desired extension
               for (const auto& extension : p.extensions)
               {
                  if(mem::ends_with(path.c_str(), path.size(),
                                    extension.c_str(), extension.size()))
                  {
                     load_file(path);
                  }
               }
            }
         }
         while (FindNextFile(searchH.get(), &fileInfo));
      }

      void load_file(const sys_str& f)
      {
         // Read the file header
         FileHandle h{ f, file_open_modes::read };
         auto header = read_file_header(h);

         // Map the id to the file path.
         const auto& id = header.metadata.id;
         if (m_files.count(id) > 0)
         {
            throw std::runtime_error {
               id.str<char>() + " is already mapped to the store."
            };
         }

         m_files[id] = f;
      }

      sys_str format_extention(const sys_str& ext)
      {
         // Make sure the extension starts with a .
         if (ext.front() != L'.')
         {
            return L'.' + ext;
         }

         return ext;
      }

      sys_str format_dir(const sys_str& dir)
      {
         // Make sure the directory ends with a slash.
         if (dir.back() != L'/')
         {
            return dir + L'/';
         }

         return dir;
      }

      std::unordered_map<qgl::guid, std::unique_ptr<content_loader>> m_loaders;
      std::unordered_map<qgl::guid, sys_str> m_files;
      std::unordered_map<qgl::guid, load_traits> m_content;
   };
}