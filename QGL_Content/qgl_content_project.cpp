#include "pch.h"
#include "include\Content-Project\qgl_content_project.h"
#include "include/qgl_file_handle.h"
#include "include/qgl_file_helpers.h"

namespace qgl::content
{
   struct content_project::impl
   {
      public:
      impl(const wchar_t* filePath)
      {
         auto exists = file_exists(filePath);
         open_file_readwrite(filePath, &m_handle);
         if (exists)
         {
            read_in();
         }
      }

      impl(const winrt::Windows::Storage::StorageFile& f)
      {
         //The storage file already exists, so to check if it is an old file, 
         //check if it is greater than 0 bytes.
         auto exists = file_size(f) > 0;
         open_file_readwrite(f, &m_handle);
         if (exists)
         {
            read_in();
         }
      }

      impl(const impl&) = delete;

      impl(impl&& m) = default;

      virtual ~impl() noexcept = default;

      void flush()
      {
         size_t offset = 0;

         //Write the magic number
         write_file_sync(&m_handle, 
                         sizeof(QGL_CONTENT_PROJECT_MAGIC_NUMBER),
                         offset, &QGL_CONTENT_PROJECT_MAGIC_NUMBER);
         offset += sizeof(QGL_CONTENT_PROJECT_MAGIC_NUMBER);

         //Write the metadata
         write_file_sync(&m_handle, 
                         sizeof(m_hdr), offset, &m_hdr);
         offset += sizeof(m_hdr);

         //Write the number of entries.
         uint64_t numEntries = static_cast<uint64_t>(size());
         write_file_sync(&m_handle, 
                         sizeof(numEntries), offset, &numEntries);
         offset += sizeof(numEntries);

         //For each entry:
         for (const auto& entry : *this)
         {
            //Write the separator.
            write_file_sync(&m_handle,
                            sizeof(QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER),
                            offset,
                            &QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER);
            offset += sizeof(QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER);

            //Pad the magic number to 16 bytes by writing an addition 8 bytes.
            static constexpr uint64_t EXTRA_PAD = 0xEEEEEEEEEEEEEEEE;
            write_file_sync(&m_handle, 
                            sizeof(EXTRA_PAD), offset, &EXTRA_PAD);
            offset += sizeof(EXTRA_PAD);

            //Write the metadata
            write_file_sync(&m_handle, 
                            sizeof(entry.first), 
                            offset,
                            &entry.first);
            offset += sizeof(entry.first);

            //Write the number of charcters in the path.
            uint64_t numChars = static_cast<uint64_t>(entry.second.size());
            write_file_sync(&m_handle, numChars, offset, &numChars);
            offset += sizeof(numChars);

            //Write the path. Each character in the path is 2 bytes.
            write_file_sync(&m_handle, 
                            numChars * sizeof(wchar_t),
                            offset,
                            entry.second.c_str());
            offset += sizeof(wchar_t) * numChars;
         }
      }

      CONTENT_METADATA_BUFFER* metadata() noexcept
      {
         return &m_hdr;
      }

      const CONTENT_METADATA_BUFFER* metadata() const noexcept
      {
         return &m_hdr;
      }

      size_t size() const noexcept
      {
         return m_entries.size();
      }

      void emplace_back(const content_project_entry_pair::first_type* entry,
                        const wchar_t* absPath)
      {
         m_entries.emplace_back(*entry, winrt::to_hstring(absPath));
      }

      content_project_entry_pair& at(size_t idx)
      {
         return m_entries.at(idx);
      }

      const content_project_entry_pair& at(size_t idx) const
      {
         return m_entries.at(idx);
      }

      content_project_entry_pair& operator[](size_t idx) noexcept
      {
         return m_entries[idx];
      }

      const content_project_entry_pair& operator[](size_t idx) const noexcept
      {
         return m_entries[idx];
      }

      iterator erase(const_iterator position)
      {
         return m_entries.erase(position);
      }

      iterator erase(const_iterator first, const_iterator last)
      {
         return m_entries.erase(first, last);
      }

      iterator begin() noexcept
      {
         return m_entries.begin();
      }

      const_iterator begin() const noexcept
      {
         return m_entries.cbegin();
      }

      const_iterator cbegin() const noexcept
      {
         return m_entries.cbegin();
      }

      iterator end() noexcept
      {
         return m_entries.end();
      }

      const_iterator end() const noexcept
      {
         return m_entries.cend();
      }

      const_iterator cend() const noexcept
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
      void read_in()
      {
         size_t offset = 0;

         //Read the magic number and check it.
         uint64_t readMagicNumber = 0;
         read_file_sync(&m_handle, 
                        sizeof(readMagicNumber), 
                        offset, 
                        &readMagicNumber);
         offset += sizeof(readMagicNumber);
         if (readMagicNumber != QGL_CONTENT_PROJECT_MAGIC_NUMBER)
         {
            throw std::exception("The project's magic number is not correct.");
         }

         //Read the metadata
         read_file_sync(&m_handle, sizeof(m_hdr), offset, &m_hdr);
         offset += sizeof(m_hdr);

         //Read the number of entries. 8 bytes.
         uint64_t numEntries = 0;
         read_file_sync(&m_handle, sizeof(numEntries), offset, &numEntries);
         offset += sizeof(numEntries);

         //For each entry to read:
         while (numEntries > 0)
         {
            //Read the magic number and check it.
            read_file_sync(&m_handle, sizeof(readMagicNumber), offset,
                           &readMagicNumber);
            offset += (sizeof(readMagicNumber) * 2);
            if (readMagicNumber != QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER)
            {
               #ifdef DEBUG
               char exceptionMessage[128];
               sprintf_s(exceptionMessage, "The entry's magic number is %llX",
                       readMagicNumber);
               #else
               const char* exceptionMessage = 
                  "The entry's magic number is not correct.";
               #endif
               throw std::exception(exceptionMessage);
            }

            //Read the metadata
            CONTENT_METADATA_BUFFER meta;
            read_file_sync(&m_handle, sizeof(meta), offset, &meta);
            offset += sizeof(meta);

            //Read the number of characters
            uint64_t numChars = 0;
            read_file_sync(&m_handle, sizeof(numChars), offset, &numChars);
            offset += sizeof(numChars);

            //Read the path wstring. Need to read as a wstring because 
            //hstring does
            //not support resize.
            std::wstring path(numChars, L'\0');
            read_file_sync(&m_handle, numChars * sizeof(wchar_t),
                           offset, path.data());
            offset += numChars * sizeof(wchar_t);

            //Emplace back a new entry.
            emplace_back(&meta, path.c_str());

            numEntries--;
         }
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

   content_project::content_project(const wchar_t* filePath) :
      m_impl_p(new impl(filePath))
   {

   }

   content_project::content_project(
      const winrt::Windows::Storage::StorageFile& f) :
      m_impl_p(new impl(f))
   {

   }

   content_project::content_project(content_project&& m) 
   {
      delete m_impl_p;
      m_impl_p = m.m_impl_p;
      m.m_impl_p = nullptr;
   }

   content_project::~content_project() noexcept
   {
      delete m_impl_p;
   }

   void content_project::flush()
   {
      m_impl_p->flush();
   }

   CONTENT_METADATA_BUFFER* content_project::metadata() noexcept
   {
      return m_impl_p->metadata();
   }

   const CONTENT_METADATA_BUFFER* content_project::metadata() const noexcept
   {
      return m_impl_p->metadata();
   }

   size_t content_project::size() const noexcept
   {
      return m_impl_p->size();
   }

   void content_project::emplace_back(const cpep::first_type* entry,
                                      const wchar_t* absPath)
   {
      m_impl_p->emplace_back(entry, absPath);
   }

   content_project::cpep& content_project::at(size_t idx)
   {
      return m_impl_p->at(idx);
   }

   const content_project::cpep& content_project::at(size_t idx) const
   {
      return m_impl_p->at(idx);
   }

   content_project::cpep& content_project::operator[](size_t idx) noexcept
   {
      return (*m_impl_p)[idx];
   }

   const content_project::cpep& content_project::operator[](
      size_t idx) const noexcept
   {
      return (*m_impl_p)[idx];
   }

   content_project::iterator content_project::erase(const_iterator position)
   {
      return m_impl_p->erase(position);
   }

   content_project::iterator content_project::erase(const_iterator first,
                                                    const_iterator last)
   {
      return m_impl_p->erase(first, last);
   }

   content_project::iterator content_project::begin() noexcept
   {
      return m_impl_p->begin();
   }

   content_project::const_iterator content_project::begin() const noexcept
   {
      return m_impl_p->begin();
   }

   content_project::const_iterator content_project::cbegin() const noexcept
   {
      return m_impl_p->cbegin();
   }

   content_project::iterator content_project::end() noexcept
   {
      return m_impl_p->end();
   }

   content_project::const_iterator content_project::end() const noexcept
   {
      return m_impl_p->end();
   }

   content_project::const_iterator content_project::cend() const noexcept
   {
      return m_impl_p->cend();
   }
}
