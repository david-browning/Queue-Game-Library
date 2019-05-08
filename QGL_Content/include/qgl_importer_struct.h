#pragma once
#include "include/qgl_content_include.h"
#include "qgl_iimporter.h"
#include "qgl_file_helpers.h"

namespace qgl::content
{
   /*
    StructType must be a POD.
    */
   template<typename StructType,
      CONTENT_LOADER_IDS lid,
      RESOURCE_TYPES rid,
      typename ContentCharT = char,
      typename IDT = content_id>
      class struct_importer : public ientry_importer<ContentCharT, IDT, StructType>
   {
      static_assert(std::is_pod<StructType>::value, "Template parameter 'StructType' must be plain old data.");

      public:
      using LoadT = StructType;
      constexpr struct_importer() :
         ientry_importer(lid)
      {

      }

      virtual ~struct_importer() noexcept
      {

      }

      virtual LoadT load(_IN_HANDLE_ const winrt::file_handle& fileHandle,
                           _IN_ const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup,
                           IDT newID = -1) const
      {
         LoadT ret;
         read_file_sync(fileHandle, lookup.size(), lookup.offset(), &ret);
         return ret;
      }

      virtual const CONTENT_DICTIONARY_ENTRY_BUFFER dict_entry(const LoadT& data,
                                                               const winrt::hstring& objName,
                                                               size_t offset = -1) const
      {
         static content_info info(rid, lid);
         return content_dictionary_entry(sizeof(LoadT), info, objName, offset);
      }
   };

   #pragma region Signed Integer Importers

   #pragma endregion

   #pragma region Unsigned Integer Importer

   template<typename ContentCharT = char, typename IDT = content_id>
   using uint8_importer = struct_importer<uint8_t,
      CONTENT_LOADER_IDS::CONTENT_LOADER_ID_UINT8,
      RESOURCE_TYPES::RESOURCE_TYPE_INTEGER,
      ContentCharT, IDT>;

   #pragma endregion

}