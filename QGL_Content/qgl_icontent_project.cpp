#include "pch.h"
#include "include/Interfaces/qgl_icontent_project.h"
#include "include/qgl_file_handle.h"
#include "include/qgl_file_helpers.h"

namespace qgl::content
{
   struct content_project : public icontent_project
   {
      content_project(const wchar_t* filePath)
      {
         auto exists = file_exists(filePath);
         winrt::check_hresult(open_file_readwrite(filePath, &m_handle));
         if (exists)
         {
            winrt::check_hresult(read_in());
         }
      }

      content_project(const winrt::Windows::Storage::StorageFile& f)
      {
         //The storage file already exists, so to check if it is an old file, 
         //check if it is greater than 0 bytes.
         size_t sz = 0;
         winrt::check_hresult(file_size(f, &sz));

         bool exists = sz > 0;
         winrt::check_hresult(open_file_readwrite(f, &m_handle));
         if (exists)
         {
            winrt::check_hresult(read_in());
         }
      }

      content_project(const content_project&) = delete;

      content_project(content_project&&) = default;

      virtual ~content_project() noexcept = default;

      virtual void release()
      {
         //Calls the destructor.
         delete this;
      }

      virtual HRESULT flush() noexcept
      {
         size_t offset = 0;

         //Write the magic number
         auto hr = write_file_sync(&m_handle,
                                   sizeof(QGL_CONTENT_PROJECT_MAGIC_NUMBER),
                                   offset, &QGL_CONTENT_PROJECT_MAGIC_NUMBER);
         if (FAILED(hr))
         {
            return hr;
         }
         offset += sizeof(QGL_CONTENT_PROJECT_MAGIC_NUMBER);

         //Write the metadata
         hr = write_file_sync(&m_handle,
                              sizeof(m_hdr), offset, &m_hdr);
         if (FAILED(hr))
         {
            return hr;
         }
         offset += sizeof(m_hdr);

         //Write the number of entries.
         uint64_t numEntries = static_cast<uint64_t>(size());
         hr = write_file_sync(&m_handle,
                              sizeof(numEntries), offset, &numEntries);
         if (FAILED(hr))
         {
            return hr;
         }
         offset += sizeof(numEntries);

         //For each entry:
         for (const auto& entry : *this)
         {
            //Write the separator.
            hr = write_file_sync(
               &m_handle,
               sizeof(QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER),
               offset,
               &QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER);
            if (FAILED(hr))
            {
               return hr;
            }
            offset += sizeof(QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER);

            //Pad the magic number to 16 bytes by writing an addition 8 bytes.
            static constexpr uint64_t EXTRA_PAD = 0xEEEEEEEEEEEEEEEE;
            hr = write_file_sync(&m_handle,
                                 sizeof(EXTRA_PAD), offset, &EXTRA_PAD);
            if (FAILED(hr))
            {
               return hr;
            }
            offset += sizeof(EXTRA_PAD);

            //Write the metadata
            hr = write_file_sync(&m_handle,
                                 sizeof(entry.first),
                                 offset,
                                 &entry.first);
            if (FAILED(hr))
            {
               return hr;
            }
            offset += sizeof(entry.first);

            //Write the number of characters in the path.
            uint64_t numChars = static_cast<uint64_t>(entry.second.size());
            hr = write_file_sync(&m_handle, numChars, offset, &numChars);
            if (FAILED(hr))
            {
               return hr;
            }
            offset += sizeof(numChars);

            //Write the path. Each character in the path is 2 bytes.
            hr = write_file_sync(&m_handle,
                                 numChars * sizeof(wchar_t),
                                 offset,
                                 entry.second.c_str());
            if (FAILED(hr))
            {
               return hr;
            }
            offset += sizeof(wchar_t) * numChars;
         }

         return S_OK;
      }

      virtual CONTENT_METADATA_BUFFER* metadata() noexcept
      {
         return &m_hdr;
      }

      virtual const CONTENT_METADATA_BUFFER* metadata() const noexcept
      {
         return &m_hdr;
      }

      virtual size_t size() const noexcept
      {
         return m_entries.size();
      }

      virtual void emplace_back(
         const content_project_entry_pair::first_type* entry,
         const wchar_t* absPath)
      {
         m_entries.emplace_back(*entry, winrt::to_hstring(absPath));
      }

      virtual content_project_entry_pair* at(size_t idx) noexcept
      {
         return &m_entries[idx];
      }

      /*
       Returns a const reference to the idx'th project entry.
       This throws out_of_range if the index is out of bounds.
       */
      virtual const content_project_entry_pair* at(size_t idx) const noexcept
      {
         return &m_entries[idx];
      }

      /*
       Returns the project entry at the given position.
       */
      virtual iterator erase(const_iterator position)
      {
         return m_entries.erase(position);
      }

      /*
       Removes the project entries between first and last, inclusive.
       */
      virtual iterator erase(const_iterator first, const_iterator last)
      {
         return m_entries.erase(first, last);
      }

      /*
       Returns an iterator to the beginning of the project entries.
       */
      virtual iterator begin() noexcept
      {
         return m_entries.begin();
      }

      virtual const_iterator begin() const noexcept
      {
         return m_entries.cbegin();
      }

      /*
       Returns a const iterator to the beginning of the project entries.
       */
      virtual const_iterator cbegin() const noexcept
      {
         return m_entries.cbegin();
      }

      /*
       Returns an iterator to the end of the project entries.
       */
      virtual iterator end() noexcept
      {
         return m_entries.end();
      }

      virtual const_iterator end() const noexcept
      {
         return m_entries.cend();
      }

      /*
       Returns a const iterator to the end of the project entries.
       */
      virtual const_iterator cend() const noexcept
      {
         return m_entries.cend();
      }

      private:
      /*
       Reads in the file using m_handle. Also verifies that the file is
       correct.
       Assume m_handle is valid and has RW permissions.
       Throws exception if there is a problem reading or the file is invalid.
       */
      HRESULT read_in() noexcept
      {
         size_t offset = 0;

         //Read the magic number and check it.
         uint64_t readMagicNumber = 0;
         auto hr = read_file_sync(&m_handle,
                                  sizeof(readMagicNumber),
                                  offset,
                                  &readMagicNumber);
         if (FAILED(hr))
         {
            return hr;
         }
         offset += sizeof(readMagicNumber);
         if (readMagicNumber != QGL_CONTENT_PROJECT_MAGIC_NUMBER)
         {
            return E_BADMAGIC;
         }

         //Read the metadata
         hr = read_file_sync(&m_handle, sizeof(m_hdr), offset, &m_hdr);
         if (FAILED(hr))
         {
            return hr;
         }
         offset += sizeof(m_hdr);

         //Read the number of entries. 8 bytes.
         uint64_t numEntries = 0;
         hr = read_file_sync(&m_handle,
                             sizeof(numEntries),
                             offset,
                             &numEntries);
         if (FAILED(hr))
         {
            return hr;
         }
         offset += sizeof(numEntries);

         //For each entry to read:
         while (numEntries > 0)
         {
            //Read the magic number and check it.
            hr = read_file_sync(&m_handle,
                                sizeof(readMagicNumber),
                                offset,
                                &readMagicNumber);
            if (FAILED(hr))
            {
               return hr;
            }
            offset += (sizeof(readMagicNumber) * 2);
            if (readMagicNumber !=
                QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER)
            {
               return E_BADMAGIC;
            }

            //Read the metadata
            CONTENT_METADATA_BUFFER meta;
            hr = read_file_sync(&m_handle,
                                sizeof(meta),
                                offset,
                                &meta);
            if (FAILED(hr))
            {
               return hr;
            }
            offset += sizeof(meta);

            //Read the number of characters
            uint64_t numChars = 0;
            hr = read_file_sync(&m_handle,
                                sizeof(numChars),
                                offset,
                                &numChars);
            if (FAILED(hr))
            {
               return hr;
            }
            offset += sizeof(numChars);

            //Read the path wstring. Need to read as a wstring because 
            //hstring does
            //not support resize.
            std::wstring path(numChars, L'\0');
            hr = read_file_sync(&m_handle,
                                numChars * sizeof(wchar_t),
                                offset,
                                path.data());
            if (FAILED(hr))
            {
               return hr;
            }
            offset += numChars * sizeof(wchar_t);

            //Emplace back a new entry.
            emplace_back(&meta, path.c_str());

            numEntries--;
         }

         return S_OK;
      }

      /*
       Collection of entries.
       */
      container m_entries;

      /*
       File handle.
       */
      file_handle m_handle;

      /*
       Content file header.
       */
      CONTENT_METADATA_BUFFER m_hdr;
   };

   HRESULT qgl_open_content_project(const wchar_t* filePath,
                                    qgl_version_t v,
                                    icontent_project** out_p)
   {
      if (out_p == nullptr)
      {
         return E_INVALIDARG;
      }

      icontent_project*  ret = nullptr;

      switch (std::hash<qgl_version_t>{}(v))
      {
         case hashes::VERSION_0_1_HASH:
         case hashes::VERSION_0_2_HASH:
         {
            ret = new(std::nothrow) content_project(filePath);
            break;
         }
         default:
         {
            return E_NOINTERFACE;
         }
      }

      if (ret == nullptr)
      {
         return E_OUTOFMEMORY;
      }

      *out_p = ret;
      return S_OK;
   }

   HRESULT qgl_open_content_project_sf(
      const winrt::Windows::Storage::StorageFile& f,
      qgl_version_t v,
      icontent_project** out_p)
   {
      if (out_p == nullptr)
      {
         return E_INVALIDARG;
      }

      icontent_project*  ret = nullptr;

      switch (std::hash<qgl_version_t>{}(v))
      {
         case hashes::VERSION_0_1_HASH:
         case hashes::VERSION_0_2_HASH:
         {
            ret = new(std::nothrow) content_project(f);
            break;
         }
         default:
         {
            return E_NOINTERFACE;
         }
      }

      if (ret == nullptr)
      {
         return E_OUTOFMEMORY;
      }

      *out_p = ret;
      return S_OK;
   }
}