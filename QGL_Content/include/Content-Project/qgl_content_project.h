#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Buffers/qgl_content_metadata_buffer.h"
#include "include/Content-Buffers/qgl_content_dictionary_metadata_buffer.h"
#include "include/Content-Buffers/qgl_content_dict_entry_buffer.h"
#include "include/Content-Buffers/qgl_content_file_header_buffer.h"

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
    Number of entries : 2 bytes
    N * 
      {
         QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER
         CONTENT_METADATA_BUFFER
         Number of characters in path : 2 Bytes
         WString Path
      }
    */
   class LIB_EXPORT content_project
   {
      public:
      using entry_pair = std::pair<CONTENT_METADATA_BUFFER, winrt::hstring>;
      using container = std::list<entry_pair>;
      using iterator = container::iterator;
      using const_iterator = container::const_iterator;

      /*
       Open a content project file in read-write mode.
       File path must be absolute.
       If the file does not exist, this creates a new one.
       If the file exists, the constructor checks if the file is valid. If the 
       file is not valid, this throws an exception.
       */
      content_project(const winrt::hstring& filePath);

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
       This function is only valid if the file was opened with write 
       permissions.
       */
      void flush();

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
       Removes the last project entry from this.
       */
      void pop_back()
      {
         m_entries.pop_back();
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

      /*
       Returns a const iterator to the end of the project entries.
       */
      const_iterator cend() const noexcept
      {
         return m_entries.cend();
      }

      private:

      void read_in();

      /*
       Collection of entries.
       */
      container m_entries;

      /*
       Content file header.
       */
      CONTENT_METADATA_BUFFER m_hdr;

      /*
       File handle.
       */
      winrt::handle m_handle;
   };
}