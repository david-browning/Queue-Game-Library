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

   template<typename T, CONTENT_LOADER_IDS LoaderID>
   class integral_dict_export_fn
   {
      static_assert(std::is_integral<T>::value, "T must be integral.");

      public:
      CONTENT_DICTIONARY_ENTRY_BUFFER operator()(const T& integer,
                                                 const wchar_t* name,
                                                 size_t offset)
      {
         CONTENT_METADATA_BUFFER info(RESOURCE_TYPE_INTEGER,
                                      LoaderID,
                                      name);

         return CONTENT_DICTIONARY_ENTRY_BUFFER(sizeof(T), &info, offset);
      }
   };

   template<typename T, CONTENT_LOADER_IDS LoaderID>
   using integral_importer = entry_importer<
      T,
      CONTENT_LOADER_ID_INT8,
      integral_load_entry_fn<T>,
      integral_dict_export_fn<T, LoaderID>>;

   using int8_importer = integral_importer<int8_t, CONTENT_LOADER_ID_INT8>;
   using int16_importer = integral_importer<int16_t, CONTENT_LOADER_ID_INT16>;
   using int32_importer = integral_importer<int32_t, CONTENT_LOADER_ID_INT32>;
   using int64_importer = integral_importer<int64_t, CONTENT_LOADER_ID_INT64>;

   using uint8_importer = integral_importer<uint8_t,
      CONTENT_LOADER_ID_UINT8>;
   using uint16_importer = integral_importer<uint16_t,
      CONTENT_LOADER_ID_UINT16>;
   using uint32_importer = integral_importer<uint32_t,
      CONTENT_LOADER_ID_UINT32>;
   using uint64_importer = integral_importer<uint64_t,
      CONTENT_LOADER_ID_UINT64>;
}