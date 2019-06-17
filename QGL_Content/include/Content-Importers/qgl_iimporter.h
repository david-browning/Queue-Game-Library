#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Items/qgl_content_item.h"
#include "include/qgl_content_types.h"
#include "include/Content-Buffers/qgl_content_dict_entry_buffer.h"
#include <type_traits>

namespace qgl::content::loaders
{
   #ifdef DEBUG
   //These structs only exist to instantiate a template at design time.
   class test_entry_loader
   {
      public:
      int operator()(const file_handle&,
                     const CONTENT_DICTIONARY_ENTRY_BUFFER&) const
      {
         return 0;
      }
   };

   class test_dict_export
   {
      public:
      CONTENT_DICTIONARY_ENTRY_BUFFER operator()(const int&,
                                                 const wchar_t*,
                                                 size_t) const
      {
         return CONTENT_DICTIONARY_ENTRY_BUFFER();
      }
   };

   #endif

   /*
    Entry importers load data stored in a content file.
    Overload load and dict_entry when implementing this class.
    LoadT: object that this importer returns.
    */
   template<class LoadT, typename LoadFunctor, typename DictMakeFunctor>
   class ientry_importer
   {
      public:
      constexpr ientry_importer(LoadFunctor lf = LoadFunctor(),
                                DictMakeFunctor df = DictMakeFunctor()) :
         m_loadFunctor(lf),
         m_dictMakeFunctor(df)
      {

      }

      /*
       Loads content from a file using the lookup data.
       The file must be opened with read permissions.
       Returns the data loaded.
       */
      LoadT load(const file_handle& fileHandle,
                 const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup) const
      {
         return m_loadFunctor(fileHandle, lookup);
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
         return m_dictMakeFunctor(data, objName, offset);
      }

      private:
      LoadFunctor m_loadFunctor;
      DictMakeFunctor m_dictMakeFunctor;
   };
}