#include "pch.h"
#include "include/Interfaces/qgl_icontent_file.h"
#include "include/Content-Buffers/qgl_content_dictionary_metadata_buffer.h"
#include "include/qgl_file_helpers.h"
#include "include/Content-Files/qgl_content_file_helpers.h"

namespace qgl::content
{
   using namespace content_file_helpers;

   class content_file : public icontent_file
   {
      public:
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
         size_t sz = 0;
         auto hr = file_size_sf(f, &sz);
         winrt::check_hresult(hr);

         bool existingFile = sz > 0;
         open_file_readwrite_sf(f, &m_handle);

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

      virtual HRESULT flush() noexcept
      {
          //Write the header.
         auto hr = content_file_helpers::write_header(m_handle, m_header);
         if (FAILED(hr))
         {
            return hr;
         }

         //Create a dictionary metadata buffer;
         CONTENT_DICTIONARY_METADATA_BUFFER dictMeta(
            m_entryDataToWrite.size(),
            sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER));

         //Write the dictionary metadata
         size_t dictMetaOffset = m_header.dictionary_offset();
         hr = write_dictionary_metadata(m_handle, dictMeta, dictMetaOffset);
         if (FAILED(hr))
         {
            return hr;
         }

         //Offset to where to put the next dictionary entry.
         size_t dictEntryOffset = dictMetaOffset +
            sizeof(CONTENT_DICTIONARY_METADATA_BUFFER);

         //Offset to where to put the content data.
         auto contentDataOffset = dictionary_data_offset(m_header, dictMeta);

         auto entryIt = m_dict.begin();
         auto contentIt = m_entryDataToWrite.begin();
         for (; entryIt != m_dict.end() &&
              contentIt != m_entryDataToWrite.end();
              ++entryIt, ++contentIt)
         {
            entryIt->m_offset = contentDataOffset;
            hr = write_dictionary_entry(m_handle, *entryIt, dictEntryOffset);

            if (entryIt->metadata()->shared())
            {
               hr = write_shared_data_path(m_handle,
                                           *entryIt,
                                           contentIt->shared_buffer());
            }
            else
            {
               hr = write_content_data(m_handle,
                                       *entryIt,
                                       contentIt->buffer());
            }

            if (FAILED(hr))
            {
               return hr;
            }

            contentDataOffset += entryIt->size();
            dictEntryOffset += dictMeta.entry_size();
         }


         return S_OK;
      }

      virtual void push_data_entry(
         const CONTENT_METADATA_BUFFER* meta,
         const void* buff,
         size_t buffBytes) noexcept
      {
         entries::content_variant_entry cont(buff, buffBytes);
         CONTENT_DICTIONARY_ENTRY_BUFFER entry(buffBytes, meta);
         entry.metadata()->shared(false);

         m_entryDataToWrite.push_back(std::move(cont));
         m_dict.push_back(std::move(entry));
      }

      virtual void push_shared_entry(
         const CONTENT_METADATA_BUFFER* meta,
         const wchar_t* str) noexcept
      {
         entries::content_variant_entry cont(str);
         CONTENT_DICTIONARY_ENTRY_BUFFER entry(shared_entry_data_size(&cont),
                                               meta);
         entry.metadata()->shared(true);

         m_entryDataToWrite.push_back(std::move(cont));
         m_dict.push_back(std::move(entry));
      }

      virtual void erase(size_t idx) noexcept
      {
         if (idx >= m_dict.size())
         {
            return;
         }

         m_dict.erase(m_dict.begin() + idx);
      }

      virtual const CONTENT_FILE_HEADER_BUFFER* const_header() const noexcept
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

      virtual const CONTENT_DICTIONARY_ENTRY_BUFFER* const_at(
         size_t idx) const noexcept
      {
         return &m_dict[idx];
      }

      private:
      HRESULT read_in() noexcept
      {
         //Read the header
         auto hr = load_header(m_handle, &m_header);
         if (FAILED(hr))
         {
            return hr;
         }

         //Read the dictionary metadata
         CONTENT_DICTIONARY_METADATA_BUFFER dictMeta;
         hr = load_dictionary_metadata(
            m_handle,
            m_header.dictionary_offset(),
            &dictMeta);
         if (FAILED(hr))
         {
            return hr;
         }

         size_t dictEntryOffset = m_header.dictionary_offset() +
            sizeof(CONTENT_DICTIONARY_METADATA_BUFFER);

         //For each entry in the dictionary:
         for (size_t i = 0; i < dictMeta.count(); i++)
         {
            CONTENT_DICTIONARY_ENTRY_BUFFER dictEntry;
            hr = load_dictionary_entry(m_handle,
                                       dictEntryOffset,
                                       &dictEntry);
            if (FAILED(hr))
            {
               return hr;
            }

            m_dict.push_back(dictEntry);

            if (dictEntry.metadata()->shared())
            {
               using NumCharsType = uint16_t;

               //First bytes are the number of characters in the path.
               //Next bytes is the path. It is a wide string. 
               //Not null-terminated.
               NumCharsType numChars = 0;
               hr = read_file_sync(&m_handle,
                                   sizeof(numChars),
                                   dictEntry.offset(),
                                   &numChars);
               if (FAILED(hr))
               {
                  return hr;
               }

               std::wstring path(numChars, L'\0');
               hr = read_file_sync(&m_handle,
                                   numChars * sizeof(wchar_t),
                                   dictEntry.offset() + sizeof(numChars),
                                   path.data());
               if (FAILED(hr))
               {
                  return hr;
               }

               try
               {
                  m_entryDataToWrite.push_back(
                     entries::content_variant_entry(path.c_str()));
               }
               catch (std::invalid_argument&)
               {
                  #ifdef DEBUG
                  OutputDebugString(L"The shared entry path is ill-formed.\n");
                  #endif
                  return E_INVALIDARG;
               }
            }
            else
            {
               auto entrySize = dictEntry.size();
               std::vector<uint8_t> ret;
               ret.resize(entrySize);
               hr = read_file_sync(&m_handle,
                                   entrySize,
                                   dictEntry.offset(),
                                   ret.data());
               if (FAILED(hr))
               {
                  return hr;
               }

               m_entryDataToWrite.push_back(
                  entries::content_variant_entry(ret.data(), ret.size()));
            }

            dictEntryOffset += sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER);
         }

         return S_OK;
      }

      void check_and_throw_file_size()
      {
         if (!valid_content_file_size(m_handle))
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
      std::vector<entries::content_variant_entry> m_entryDataToWrite;

      /*
       File handle.
       */
      file_handle m_handle;
   };

   HRESULT qgl_open_content_file(const wchar_t* filePath,
                                 qgl_version_t v,
                                 icontent_file** out_p)
   {
      if (out_p == nullptr)
      {
         #ifdef DEBUG
         OutputDebugString(L"out_p cannot be nullptr.");
         #endif
         return E_INVALIDARG;
      }

      icontent_file* ret = nullptr;
      switch (std::hash<qgl_version_t>{}(v))
      {
         case qgl::hashes::VERSION_0_1_HASH:
         case qgl::hashes::VERSION_0_2_HASH:
         {
            ret = new(std::nothrow) content_file(filePath);
            break;
         }
         default:
         {
            #ifdef DEBUG
            OutputDebugString(L"This QGL version is not supported.");
            #endif
            return E_NOINTERFACE;
         }
      }

      if (ret == nullptr)
      {
         #ifdef DEBUG
         OutputDebugString(L"Out of memory!");
         #endif
         return E_OUTOFMEMORY;
      }

      *out_p = ret;
      return S_OK;
   }

   HRESULT qgl_open_content_file_sf(
      const winrt::Windows::Storage::StorageFile& f,
      qgl_version_t v,
      icontent_file** out_p)
   {
      if (out_p == nullptr)
      {
         #ifdef DEBUG
         OutputDebugString(L"out_p cannot be nullptr.");
         #endif
         return E_INVALIDARG;
      }

      icontent_file* ret = nullptr;
      switch (std::hash<qgl_version_t>{}(v))
      {
         case qgl::hashes::VERSION_0_1_HASH:
         case qgl::hashes::VERSION_0_2_HASH:
         {
            ret = new(std::nothrow) content_file(f);
            break;
         }
         default:
         {
            #ifdef DEBUG
            OutputDebugString(L"This QGL version is not supported.");
            #endif
            return E_NOINTERFACE;
         }
      }

      if (ret == nullptr)
      {
         #ifdef DEBUG
         OutputDebugString(L"Out of memory!");
         #endif
         return E_OUTOFMEMORY;
      }

      *out_p = ret;
      return S_OK;
   }
}