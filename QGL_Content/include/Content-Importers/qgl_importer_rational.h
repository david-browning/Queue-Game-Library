#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Importers/qgl_iimporter.h"
#include "include/qgl_file_helpers.h"

namespace qgl::content::loaders
{
   template<typename NumberT>
   constexpr size_t rational_encoded_size()
   {
      return 1 + 2 * sizeof(NumberT);
   }

   template<typename T>
   class rational_load_entry_fn
   {
      static_assert(std::is_integral<T>::value,
                    "NumberT must be integral.");

      static_assert(std::is_signed<T>::value,
                    "NumberT must be signed.");

      public:
      qgl::math::rational<T> operator()(
         const file_handle& fileHandle,
         const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup) const
      {
         if (lookup.size() != rational_encoded_size<T>())
         {
            throw std::invalid_argument("The lookup size is not expected.");
         }

         uint8_t numBytes = 0;
         winrt::check_hresult(read_file_sync(&fileHandle,
                                             sizeof(numBytes),
                                             lookup.offset(),
                                             &numBytes));

         if (numBytes != sizeof(T))
         {
            throw std::invalid_argument("The number of bytes in the file does "
                                        "not match the sizeof(T).");
         }

         T numerator = 0;
         T denominator = 0;
         winrt::check_hresult(read_file_sync(&fileHandle,
                                             sizeof(T),
                                             lookup.offset() + 1,
                                             &numerator));

         winrt::check_hresult(read_file_sync(&fileHandle,
                                             sizeof(T),
                                             lookup.offset() + 1 + sizeof(T),
                                             &denominator));

         return qgl::math::rational<T>(numerator, denominator);
      }
   };

   template<typename T>
   class rational_dict_export_fn
   {
      static_assert(std::is_integral<T>::value,
                    "NumberT must be integral.");

      static_assert(std::is_signed<T>::value,
                    "NumberT must be signed.");

      public:
      CONTENT_DICTIONARY_ENTRY_BUFFER operator()(
         const math::rational<T>& data,
         const wchar_t* name,
         size_t offset) const
      {
         static constexpr auto RATIONAL_SIZE = rational_encoded_size<T>();
         CONTENT_METADATA_BUFFER info(RESOURCE_TYPE_FLOAT,
                                      CONTENT_LOADER_ID_RATIONAL,
                                      name);

         return CONTENT_DICTIONARY_ENTRY_BUFFER(RATIONAL_SIZE,
                                                &info,
                                                offset);
      }
   };

   template<typename NumberT>
   using rational_importer = entry_importer<
      math::rational<NumberT>,
      rational_load_entry_fn<NumberT>,
      rational_dict_export_fn<NumberT>>;

   using rational_importer8 = rational_importer<int8_t>;
   using rational_importer16 = rational_importer<int16_t>;
   using rational_importer32 = rational_importer<int32_t>;
   using rational_importer64 = rational_importer<int64_t>;
}