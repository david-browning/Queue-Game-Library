#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Importers/qgl_iimporter.h"
#include "include/qgl_file_helpers.h"

namespace qgl::content::loaders
{
   template<typename T>
   class integral_load_entry_fn
   {
      static_assert(std::is_integral<T>::value, "T must be integral.");

      public:
      /*
       Throws exception on read error.
       */
      T operator()(
         const file_handle& fileHandle,
         const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup) const
      {
         if (sizeof(T) != lookup.size())
         {
            throw std::invalid_argument("T is not the same size as the "
                                        "dictionary entry excepts.");
         }

         T ret = 0;
         winrt::check_hresult(read_file_sync(&fileHandle,
                                             lookup.size(),
                                             lookup.offset(),
                                             &ret));

         return ret;
      }
   };

   template<typename T, 
   CONTENT_LOADER_IDS LoaderID, 
   RESOURCE_TYPES ResourceType = RESOURCE_TYPE_INTEGER>
   class integral_dict_export_fn
   {
      static_assert(std::is_integral<T>::value, "T must be integral.");

      public:
      CONTENT_DICTIONARY_ENTRY_BUFFER operator()(const T& integer,
                                                 const wchar_t* name,
                                                 size_t offset)
      {
         CONTENT_METADATA_BUFFER info(ResourceType,
                                      LoaderID,
                                      name);

         return CONTENT_DICTIONARY_ENTRY_BUFFER(sizeof(T), &info, offset);
      }
   };
}