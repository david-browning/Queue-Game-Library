#include "pch.h"
#include "include/Content-Files/qgl_content_file.h"
#include "include/Content-Files/qgl_content_file_helpers.h"
#include "include/qgl_file_helpers.h"

namespace qgl::content
{
   using namespace content_file_helpers;

   #pragma region impl
   class content_file::impl
   {
      public:
      using dictionary_container_t = 
         std::vector<CONTENT_DICTIONARY_ENTRY_BUFFER>;
      using content_container_t = std::vector<content_variant_entry>;

      impl(const wchar_t* filePath)
      {
         bool existingFile = file_exists(filePath);
         open_file_readwrite(filePath, &m_handle);

         if (existingFile)
         {
            check_and_throw_file_size();
            read_in();
         }
      }

      impl(const winrt::Windows::Storage::StorageFile& f)
      {
         bool existingFile = file_size(f) > 0;
         open_file_readwrite(f, &m_handle);

         if (existingFile)
         {
            check_and_throw_file_size();
            read_in();
         }
      }

      impl(const impl&) = delete;

      impl(impl&& m) = default;

      ~impl() noexcept = default;

      void push_back(const CONTENT_METADATA_BUFFER& meta,
                     const DATA_CONTENT_ENTRY& buff)
      {
         content_variant_entry cont(buff);
         CONTENT_DICTIONARY_ENTRY_BUFFER entry(buff.size(), meta);
         entry.shared(false);

         m_entryDataToWrite.push_back(std::move(cont));
         m_dict.push_back(std::move(entry));
      }

      void push_back(const CONTENT_METADATA_BUFFER& meta,
                     const SHARED_CONTENT_ENTRY& buff)
      {
         content_variant_entry cont(buff);
         CONTENT_DICTIONARY_ENTRY_BUFFER entry(shared_entry_data_size(buff),
                                               meta);
         entry.shared(true);

         m_entryDataToWrite.push_back(std::move(cont));
         m_dict.push_back(std::move(entry));
      }

      void pop_back()
      {
         m_dict.pop_back();
         m_entryDataToWrite.pop_back();
      }

      const CONTENT_FILE_HEADER_BUFFER& header() const noexcept
      {
         return m_header;
      }

      CONTENT_FILE_HEADER_BUFFER& header()
      {
         return m_header;
      }

      size_t size() const noexcept
      {
         return m_dict.size();
      }

      const file_handle& handle() const noexcept
      {
         return m_handle;
      }

      void flush()
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

      CONTENT_DICTIONARY_ENTRY_BUFFER& operator[](size_t idx) noexcept
      {
         return m_dict[idx];
      }

      const CONTENT_DICTIONARY_ENTRY_BUFFER& operator[](size_t idx) 
         const noexcept
      {
         return m_dict[idx];
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
               content_variant_entry cbt(sharedContent);
               m_entryDataToWrite.push_back(cbt);
            }
            else
            {
               auto content = load_content_data(&m_handle, dictEntry);
               content_variant_entry cbt(content);
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
   #pragma endregion

   content_file::content_file(const wchar_t* filePath) :
      m_impl_p(new impl(filePath))
   {

   }

   content_file::content_file(const winrt::Windows::Storage::StorageFile& f) :
      m_impl_p(new impl(f))
   {

   }

   content_file::content_file(content_file&& m) :
      m_impl_p(m.m_impl_p)
   {
      m.m_impl_p = nullptr;
   }

   content_file::~content_file() noexcept
   {
      delete m_impl_p;
   }

   void content_file::flush()
   {
      m_impl_p->flush();
   }

   void content_file::push_back(const CONTENT_METADATA_BUFFER& meta,
                                const DATA_CONTENT_ENTRY& buff)
   {
      m_impl_p->push_back(meta, buff);
   }

   void content_file::push_back(const CONTENT_METADATA_BUFFER& meta,
                                const SHARED_CONTENT_ENTRY& buff)
   {
      m_impl_p->push_back(meta, buff);
   }

   const CONTENT_FILE_HEADER_BUFFER& content_file::header() const noexcept
   {
      return m_impl_p->header();
   }

   CONTENT_FILE_HEADER_BUFFER& content_file::header()
   {
      return m_impl_p->header();
   }

   size_t content_file::size() const noexcept
   {
      return m_impl_p->size();
   }

   const file_handle* content_file::handle() const noexcept
   {
      return &m_impl_p->handle();
   }

   CONTENT_DICTIONARY_ENTRY_BUFFER& content_file::operator[](
      size_t idx) noexcept
   {
      return (*m_impl_p)[idx];
   }

   const CONTENT_DICTIONARY_ENTRY_BUFFER& content_file::operator[](
      size_t idx) const noexcept
   {
      return (*m_impl_p)[idx];
   }
}