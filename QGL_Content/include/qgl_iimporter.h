#pragma once
#include "include/qgl_content_include.h"
#include "include/qgl_content_item.h"
#include "include/qgl_content_types.h"

namespace qgl::content
{
   /*
    LoadT is the object that this importer returns.
    ContentCharT is the type of character that the loaded item's name is encoded using.
    IDT is the type of the content_item's ID.
    */
   template<typename ContentCharT, typename IDT, typename LoadT>
   class ientry_importer
   {
      public:
      constexpr ientry_importer(_IN_ CONTENT_LOADER_IDS contentImporterID) :
         m_contentImporterID(contentImporterID)
      {
      }

      ientry_importer(const ientry_importer& r) :
         m_contentImporterID(r.m_contentImporterID)
      {

      }

      ientry_importer(ientry_importer&& r) :
         m_contentImporterID(r.m_contentImporterID)
      {

      }

      virtual ~ientry_importer() noexcept
      {

      }

      virtual LoadT load(_IN_HANDLE_ const winrt::file_handle& fileHandle,
                           _IN_ const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup,
                           IDT id = -1) const = 0;

        //virtual void expt(_IN_HANDLE_ const HANDLE fileHandle,
        //                  _IN_ const content_dictionary_entry& entry,
        //                  const LoadT& toExport) const = 0;

      virtual const CONTENT_DICTIONARY_ENTRY_BUFFER dict_entry(const LoadT& data,
                                                               const winrt::hstring& objName,
                                                               size_t offset = -1) const = 0;

      inline virtual CONTENT_LOADER_IDS id() const final
      {
         return m_contentImporterID;
      }

      inline virtual bool operator==(const ientry_importer& other) const final
      {
         return m_contentImporterID == other.m_contentImporterID;
      }

      inline virtual bool operator!=(const ientry_importer& other) const final
      {
         return m_contentImporterID != other.m_contentImporterID;
      }

      private:
      CONTENT_LOADER_IDS m_contentImporterID;
   };
}