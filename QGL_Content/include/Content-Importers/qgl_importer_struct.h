#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Importers/qgl_iimporter.h"
#include "include/qgl_file_helpers.h"

namespace qgl::content
{
   /*
    StructType must be a POD.
    */
   template<typename StructType, CONTENT_LOADER_IDS lid, RESOURCE_TYPES rid>
   class struct_importer : public ientry_importer<StructType>
   {
      static_assert(std::is_pod<StructType>::value,
                    "Parameter 'StructType' must be plain old data.");

      public:
      using LoadT = StructType;
      constexpr struct_importer() :
         ientry_importer(lid)
      {

      }

      virtual ~struct_importer() noexcept
      {

      }

      virtual LoadT load(const file_handle* fileHandle,
                         const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup) const
      {
         LoadT ret;
         read_file_sync(fileHandle, lookup.size(), lookup.offset(), &ret);
         return ret;
      }

      virtual CONTENT_DICTIONARY_ENTRY_BUFFER dict_entry(
         const LoadT& data,
         const winrt::hstring& objName,
         size_t offset = -1) const
      {
         static CONTENT_METADATA_BUFFER info(rid, lid, objName);

         return CONTENT_DICTIONARY_ENTRY_BUFFER(sizeof(LoadT),
                                                info,
                                                offset);
      }
   };

   #pragma region Signed Integer Importers

   #pragma endregion

   #pragma region Unsigned Integer Importer

   using uint8_importer = struct_importer<uint8_t,
      CONTENT_LOADER_IDS::CONTENT_LOADER_ID_UINT8,
      RESOURCE_TYPES::RESOURCE_TYPE_INTEGER>;

   #pragma endregion

}