#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Items/qgl_content_item.h"
#include "include/qgl_content_types.h"
#include "include/Content-Buffers/qgl_content_dict_entry_buffer.h"

namespace qgl::content
{
   /*
    Entry importers load data stored in a content file.
    Overload load and dict_entry when implementing this class.
    LoadT: object that this importer returns.
    */
   template<class LoadT,
      CONTENT_LOADER_IDS LoaderID,
      typename LoadFunction,
      typename DictMakeFunction>
   struct ientry_importer
   {
      public:
      /*
       Loads content from a file using the lookup data.
       The file must be opened with read permissions.
       Returns the data loaded.
       */
      LoadT load(const file_handle& fileHandle,
                 const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup) const
      {
         return LoadFunction(fileHandle, lookup);
      }

     /*
      Creates a dictionary entry for the given content.
      The offset is the offset into the file where the content's data
      is stored.
      */
      CONTENT_DICTIONARY_ENTRY_BUFFER dict_entry(const LoadT& data,
                                                 const wchar_t* objName,
                                                 size_t offset = -1) const
      {
         return DictMakeFunction(data, objName, offset);
      }
   };
}