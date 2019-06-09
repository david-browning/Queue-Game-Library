#include "pch.h"
#include "include/Interfaces/qgl_icontent_file.h"
#include "include/Content-Buffers/qgl_content_dictionary_metadata_buffer.h"
#include "include/qgl_file_helpers.h"
#include "include/Content-Files/qgl_content_file_helpers.h"

namespace qgl::content
{
   using namespace content_file_helpers;

   struct content_file : public icontent_file
   {
      content_file(const wchar_t* filePath)
      {
         bool existingFile = file_exists(filePath);
         open_file_readwrite(filePath, &m_handle);

         if (existingFile)
         {
            check_and_throw_file_size();
            read_in();
         }
      }

      content_file(const winrt::Windows::Storage::StorageFile& f)
      {
         bool existingFile = file_size(f) > 0;
         open_file_readwrite(f, &m_handle);

         if (existingFile)
         {
            check_and_throw_file_size();
            read_in();
         }
      }

      /*
       Cannot copy a file.
       */
      content_file(const content_file&) = delete;

      /*
       Move constructor.
       */
      content_file(content_file&&) = default;

      /*
       The destructor does not flush the file but does close the file handle.
       If the file is not flushed and this goes out of scope, any changes to
       the file will be lost.
       */
      virtual ~content_file() noexcept = default;

      virtual void release()
      {
         //Calls the destructor.
         delete this;
      }

      virtual void flush()
      {
          //Write the header.
         content_file_helpers::write_header(&m_handle, m_header);

         //Create a dictionary metadata buffer;
         CONTENT_DICTIONARY_METADATA_BUFFER dictMeta(
            m_entryDataToWrite.size(),
            sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER));

         //Write the dictionary metadata
         size_t dictMetaOffset = m_header.dictionary_offset();
         write_dictionary_metadata(&m_handle, dictMeta, dictMetaOffset);

         //Offset to where to put the next dictionary entry.
         size_t dictEntryOffset = dictMetaOffset +
            sizeof(CONTENT_DICTIONARY_METADATA_BUFFER);

         //Offset to where to put the content data.
         auto contentDataOffset = dictionary_data_offset(m_header, dictMeta);

         if (m_dict.size() != m_entryDataToWrite.size())
         {
            throw std::domain_error("There must be the same number of entries "
                                    "and data.");
         }

         auto entryIt = m_dict.begin();
         auto contentIt = m_entryDataToWrite.begin();
         for (; entryIt != m_dict.end() &&
              contentIt != m_entryDataToWrite.end();
              ++entryIt, ++contentIt)
         {
            entryIt->m_offset = contentDataOffset;
            write_dictionary_entry(&m_handle, *entryIt, dictEntryOffset);

            if (entryIt->shared())
            {
               write_shared_data_path(&m_handle,
                                      *entryIt,
                                      contentIt->shared_buffer());
            }
            else
            {
               write_content_data(&m_handle,
                                  *entryIt,
                                  contentIt->buffer());
            }

            contentDataOffset += entryIt->size();
            dictEntryOffset += dictMeta.entry_size();
         }
      }

      virtual void push_back(const CONTENT_METADATA_BUFFER* meta,
                             const DATA_CONTENT_ENTRY* buff)
      {
         content_variant_entry cont(buff);
         CONTENT_DICTIONARY_ENTRY_BUFFER entry(buff->size(), meta);
         entry.shared(false);

         m_entryDataToWrite.push_back(std::move(cont));
         m_dict.push_back(std::move(entry));
      }

      virtual void push_back(const CONTENT_METADATA_BUFFER* meta,
                             const SHARED_CONTENT_ENTRY* buff)
      {
         content_variant_entry cont(buff);
         CONTENT_DICTIONARY_ENTRY_BUFFER entry(shared_entry_data_size(buff),
                                               meta);
         entry.shared(true);

         m_entryDataToWrite.push_back(std::move(cont));
         m_dict.push_back(std::move(entry));
      }

      virtual const CONTENT_FILE_HEADER_BUFFER* header() const noexcept
      {
         return &m_header;
      }

      virtual CONTENT_FILE_HEADER_BUFFER* header() noexcept
      {
         return &m_header;
      }

      virtual size_t size() const noexcept
      {
         return m_dict.size();
      }

      virtual const file_handle* handle() const noexcept
      {
         return &m_handle;
      }

      virtual CONTENT_DICTIONARY_ENTRY_BUFFER* at(
         size_t idx) noexcept
      {
         return &m_dict[idx];
      }

      virtual const CONTENT_DICTIONARY_ENTRY_BUFFER* at(
         size_t idx) const noexcept
      {
         return &m_dict[idx];
      }

      private:
      void read_in()
      {
         //Read the header
         m_header = load_header(&m_handle);

         //Read the dictionary metadata
         auto dictMeta = load_dictionary_metadata(&m_handle,
                                                  m_header.dictionary_offset());

         size_t dictEntryOffset = m_header.dictionary_offset() +
            sizeof(CONTENT_DICTIONARY_METADATA_BUFFER);

         //For each entry in the dictionary:
         for (size_t i = 0; i < dictMeta.count(); i++)
         {
            auto dictEntry = load_dictionary_entry(&m_handle, dictEntryOffset);
            m_dict.push_back(dictEntry);

            if (dictEntry.shared())
            {
               auto sharedContent = load_shared_data_path(&m_handle, dictEntry);
               content_variant_entry cbt(&sharedContent);
               m_entryDataToWrite.push_back(cbt);
            }
            else
            {
               auto content = load_content_data(&m_handle, dictEntry);
               content_variant_entry cbt(&content);
               m_entryDataToWrite.push_back(cbt);
            }

            dictEntryOffset += sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER);
         }
      }

      void check_and_throw_file_size()
      {
         if (!valid_content_file_size(&m_handle))
         {
            throw std::domain_error("The file is too small to be valid.");
         }
      }

      /*
       File header.
       */
      CONTENT_FILE_HEADER_BUFFER m_header;

      std::vector<CONTENT_DICTIONARY_ENTRY_BUFFER> m_dict;

      /*
       An array of pointers to data that is pending writes to the content file.
       This is only populated when the content file is in write mode.
       */
      std::vector<content_variant_entry> m_entryDataToWrite;

      /*
       File handle.
       */
      file_handle m_handle;
   };

   icontent_file* qgl_open_content_file(const wchar_t* filePath,
                                        qgl_version_t v)
   {
      switch (v)
      {
         case QGL_VERSION_0_1_WIN:
         case QGL_VERSION_0_2_WIN:
         {
            return new content_file(filePath);
         }
      }

      return nullptr;
   }

   icontent_file* qgl_open_content_file(
      const winrt::Windows::Storage::StorageFile& f,
      qgl_version_t v)
   {
      switch (v)
      {
         case QGL_VERSION_0_1_WIN:
         case QGL_VERSION_0_2_WIN:
         {
            return new content_file(f);
         }
      }

      return nullptr;
   }
}