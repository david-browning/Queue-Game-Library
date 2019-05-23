#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Buffers/qgl_content_metadata_buffer.h"
#include "include/Content-Buffers/qgl_content_dictionary_metadata_buffer.h"
#include "include/Content-Buffers/qgl_content_dict_entry_buffer.h"
#include "include/Content-Buffers/qgl_content_file_header_buffer.h"
#include "include/qgl_file_handle.h"

namespace qgl::content
{
   /*
    Representation of a content project file. A content project contains 
    metadata of the content and a list of addition files supporting the project.
    Each entry in the project file is a pair of content metadata and a filepath
    where the content entry's data is stored. 
    WriteMode: Set to true to open the file with write permissions.

    File Structure:
    QGL_CONTENT_PROJECT_MAGIC_NUMBER
    CONTENT_METADATA_BUFFER
    Number of entries : 8 bytes
    N * 
      {
         QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER
         CONTENT_METADATA_BUFFER
         Number of characters in path : 8 Bytes
         WString Path
      }
    */
   class LIB_EXPORT content_project
   {
      public:
      using entry_pair = std::pair<CONTENT_METADATA_BUFFER, winrt::hstring>;
      using container = std::vector<entry_pair>;
      using iterator = container::iterator;
      using const_iterator = container::const_iterator;

      /*
       Open a content project file in read-write mode.
       File path must be absolute.
       If the file does not exist, this creates a new one.
       When creating a new file, the content metadata is default and the list
       of project entries is empty.

       If the file exists, the constructor checks if the file is valid. If the 
       file is not valid, this throws an exception.
       */
      content_project(const winrt::hstring& filePath);

      /*
       Opens a content project file in read-write mode. The storage file must
       allow write permissions.
       If the file does not exist, this creates a new one.
       When creating a new file, the content metadata is default and the list 
       of project entries is empty.

       If the file exists, the constructor checks if the file is valid. If the 
       file is not valid, this throws an exception.
       */
      content_project(const winrt::Windows::Storage::StorageFile& f);

      /*
       Cannot copy projects.
       */
      content_project(const content_project&) = delete;

      /*
       Move constructor.
       */
      content_project(content_project&&) = default;

      /*
       The destructor does not flush the file but does close the file handles.
       If the file is not flushed and the destructor is called, any changes to
       the file will be lost. Call flush() to save the file.
       */
      virtual ~content_project() noexcept = default;

      /*
       Flushes any changes to the content file to the disk.
       */
      void flush();

      /*
       Returns a reference to the content project's metadata.
       */
      CONTENT_METADATA_BUFFER& metadata() noexcept
      {
         return m_hdr;
      }

      /*
       Returns a const reference to the content project's metadata.
       */
      const CONTENT_METADATA_BUFFER& metadata() const noexcept
      {
         return m_hdr;
      }

      /*
       Returns the number of entries in the project.
       */
      size_t size() const noexcept
      {
         return m_entries.size();
      }

      /*
       Constructs and places a project entry at the end.
       */
      void emplace_back(const entry_pair::first_type& entry,
                        const winrt::hstring& absPath)
      {
         m_entries.emplace_back(entry, absPath);
      }

      /*
       Constructs and places a project entry at the given position.
       */
      void emplace(const_iterator position,
                   const entry_pair::first_type& entry,
                   const winrt::hstring& absPath)
      {
         m_entries.emplace(position, entry, absPath);
      }

      /*
       Returns a reference to the idx'th project entry.
       This throws out_of_range if the index is out of bounds.
       */
      entry_pair& at(size_t idx)
      {
         return m_entries.at(idx);
      }

      /*
       Returns a const reference to the idx'th project entry.
       This throws out_of_range if the index is out of bounds.
       */
      const entry_pair& at(size_t idx) const
      {
         return m_entries.at(idx);
      }

      /*
       Returns a reference to the idx'th project entry. 
       This does no bounds checking.
       */
      entry_pair& operator[](size_t idx) noexcept
      {
         return m_entries[idx];
      }
           
      /*
       Returns a const reference to the idx'th project entry.
       This does no bounds checking.
       */
      const entry_pair& operator[](size_t idx) const noexcept
      {
         return m_entries[idx];
      }

      /*
       Returns the project entry at the given position.
       */
      iterator erase(const_iterator position)
      {
         return m_entries.erase(position);
      }

      /*
       Removes the project entries between first and last, inclusive. 
       */
      iterator erase(const_iterator first, const_iterator last)
      {
         return m_entries.erase(first, last);
      }

      /*
       Returns an iterator to the beginning of the project entries.
       */
      iterator begin() noexcept
      {
         return m_entries.begin();
      }

      const_iterator begin() const noexcept
      {
         return m_entries.cbegin();
      }

      /*
       Returns a const iterator to the beginning of the project entries.
       */
      const_iterator cbegin() const noexcept
      {
         return m_entries.cbegin();
      }

      /*
       Returns an iterator to the end of the project entries.
       */
      iterator end() noexcept
      {
         return m_entries.end();
      }

      const_iterator end() const noexcept
      {
         return m_entries.cend();
      }

      /*
       Returns a const iterator to the end of the project entries.
       */
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
      void read_in();

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
}