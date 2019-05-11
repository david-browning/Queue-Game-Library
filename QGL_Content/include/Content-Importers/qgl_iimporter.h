#pragma once
#include "include/qgl_content_include.h"
#include "include/qgl_content_item.h"
#include "include/qgl_content_types.h"
#include "include/Content-Buffers/qgl_content_dict_entry_buffer.h"

namespace qgl::content
{
    /*
     Entry importers load data stored in a content file.
     Overload load and dict_entry when implementing this class.
     LoadT: object that this importer returns.
     */
    template<class LoadT>
    class ientry_importer
    {
        public:
        
        /*
         */
        constexpr ientry_importer(CONTENT_LOADER_IDS contentImporterID) :
            m_contentImporterID(contentImporterID)
        {
        }

        /*
         Copy constructor.
         */
        ientry_importer(const ientry_importer& r) = default;

        /*
         Move constructor.
         */
        ientry_importer(ientry_importer&& r) = default;

        /*
         Destructor.
         */
        virtual ~ientry_importer() noexcept
        {

        }

        /*
         Loads content from a file using the lookup data.
         The file must be opened with read permissions.
         Returns the data loaded.
         */
        virtual LoadT load(
            const winrt::file_handle& fileHandle,
            const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup) const = 0;

        /*
         Creates a dictionary entry for the given content.
         The offset is the offset into the file where the content's data
         is stored.
         */
        virtual CONTENT_DICTIONARY_ENTRY_BUFFER dict_entry(
            const LoadT& data,
            const winrt::hstring& objName,
            size_t offset = -1) const = 0;

        /*
         Returns this loader's ID.
         */
        inline virtual CONTENT_LOADER_IDS id() const final
        {
            return m_contentImporterID;
        }

        /*
         Returns true if an entry importer is the same as this.
         */
        virtual bool operator==(const ientry_importer& other) const final
        {
            return m_contentImporterID == other.m_contentImporterID;
        }

        /*
         Returns true if an entry importer is not the same as this.
         */
        virtual bool operator!=(const ientry_importer& other) const final
        {
            return m_contentImporterID != other.m_contentImporterID;
        }

        private:
        /*
         LoaderID.
         */
        CONTENT_LOADER_IDS m_contentImporterID;
    };
}