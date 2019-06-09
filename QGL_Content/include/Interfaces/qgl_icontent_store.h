#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Store/qgl_content_accessor.h"
#include "include/Content-Items/qgl_content_item.h"
#include "include/Interfaces/qgl_icontent_file.h"

namespace qgl::content
{
   /*
    Provides the mechanism to import content from a repository. Tracks imports
    so content is not loaded from disk more than once.
    After loading, content pointers can be accessed by using a content ID.
    The content ID is assigned when a content file is loaded.
    */
   struct icontent_store : public iqgl
   {
      using content_ptr_t = std::unique_ptr<content_item>;

      using load_function =
         std::function<content_ptr_t(const icontent_file* f)>;

      /*
       Maps a resource type and loader ID to a loader function. 
       The content store looks up the loader function when loading a file
       into memory. The loader function must be mapped or the file cannot be
       loaded.
       */
      virtual void map(RESOURCE_TYPES rID,
                       CONTENT_LOADER_IDS lID,
                       load_function fn) = 0;

      /*
       Loads the content from the relative path and returns an ID that can be
       used to lookup a pointer to the loaded item. If the file is already 
       loaded, this returns the existing content's ID.
       Throws std::logic_error if the loader is not mapped.
       */
      virtual id_t load(const wchar_t* relative) = 0;

      /*
       Unloads the memory associated with the given content ID. After unloaded,
       the content ID is not valid, meaning you cannot lookup the content item
       pointer using the ID. 
       The content can be loaded again, but it will most likely have a new
       ID and a new pointer.
       Does nothing if there is no loaded content with the given ID.
       */
      virtual void unload(id_t id) = 0;

      /*
       Returns a const pointer to content. Throws std::runtime_error if the 
       content ID is not mapped in the store. Throws std::logic_error if the 
       content is valid, but not loaded.
       The content store manages the pointer's memory. Do not free or 
       reallocate the memory.
       */
      virtual const content_item* get(id_t id) const = 0;

      /*
       Returns a pointer to content. Throws std::runtime_error if the content
       ID is not mapped in the store. Throws std::logic_error if the 
       content is valid, but not loaded.
       The content store manages the pointer's memory. Do not free or
       reallocate the memory.
       */
      virtual content_item* get(id_t id) = 0;
   };

   template<typename T>
   inline T* content_cast(content_item* p)
   {
      return dynamic_cast<T*>(p);
   }

   template<typename T>
   inline const T* content_cast(const content_item* p)
   {
      return dynamic_cast<const T*>(p);
   }

   extern QGL_CONTENT_API icontent_store* qgl_create_content_store(
      const wchar_t* storePath, qgl_version_t v);
}