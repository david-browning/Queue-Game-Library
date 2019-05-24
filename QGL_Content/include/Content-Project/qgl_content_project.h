#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Buffers/qgl_content_metadata_buffer.h"
#include "include/Content-Buffers/qgl_content_file_header_buffer.h"

namespace qgl::content
{
   QGL_CONTENT_TEMPLATE template struct QGL_CONTENT_API 
      std::pair<CONTENT_METADATA_BUFFER, file_string>;

   using content_project_entry_pair =
      std::pair<CONTENT_METADATA_BUFFER, file_string>;

   QGL_CONTENT_TEMPLATE template class QGL_CONTENT_API
      std::allocator<content_project_entry_pair>;

   QGL_CONTENT_TEMPLATE template class QGL_CONTENT_API 
      std::vector<content_project_entry_pair, 
      std::allocator<content_project_entry_pair>>;

   /*
    Representation of a content project file. A content project contains 
    metadata of the content and a list of addition files supporting the project.
    Each entry in the project file is a pair of content metadata and a file 
    path where the content entry's data is stored. 
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
      using container = std::vector<content_project_entry_pair>;
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
      content_project(const file_string& filePath);

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
      content_project(content_project&&);

      /*
       The destructor does not flush the file but does close the file handles.
       If the file is not flushed and the destructor is called, any changes to
       the file will be lost. Call flush() to save the file.
       */
      virtual ~content_project() noexcept;

      /*
       Flushes any changes to the content file to the disk.
       */
      void flush();

      /*
       Returns a reference to the content project's metadata.
       */
      CONTENT_METADATA_BUFFER& metadata() noexcept;

      /*
       Returns a const reference to the content project's metadata.
       */
      const CONTENT_METADATA_BUFFER& metadata() const noexcept;

      /*
       Returns the number of entries in the project.
       */
      size_t size() const noexcept;

      /*
       Constructs and places a project entry at the end.
       */
      void emplace_back(const content_project_entry_pair::first_type& entry,
                        const file_string& absPath);

      /*
       Constructs and places a project entry at the given position.
       */
      void emplace(const_iterator position,
                   const content_project_entry_pair::first_type& entry,
                   const file_string& absPath);

      /*
       Returns a reference to the idx'th project entry.
       This throws out_of_range if the index is out of bounds.
       */
      content_project_entry_pair& at(size_t idx);

      /*
       Returns a const reference to the idx'th project entry.
       This throws out_of_range if the index is out of bounds.
       */
      const content_project_entry_pair& at(size_t idx) const;

      /*
       Returns a reference to the idx'th project entry. 
       This does no bounds checking.
       */
      content_project_entry_pair& operator[](size_t idx) noexcept;
           
      /*
       Returns a const reference to the idx'th project entry.
       This does no bounds checking.
       */
      const content_project_entry_pair& operator[](size_t idx) const noexcept;

        /*
       Returns the project entry at the given position.
       */
      iterator erase(const_iterator position);

      /*
       Removes the project entries between first and last, inclusive.
       */
      iterator erase(const_iterator first, const_iterator last);

      /*
       Returns an iterator to the beginning of the project entries.
       */
      iterator begin() noexcept;

      const_iterator begin() const noexcept;

      /*
       Returns a const iterator to the beginning of the project entries.
       */
      const_iterator cbegin() const noexcept;

      /*
       Returns an iterator to the end of the project entries.
       */
      iterator end() noexcept;

      const_iterator end() const noexcept;

      /*
       Returns a const iterator to the end of the project entries.
       */
      const_iterator cend() const noexcept;

      private:
      struct impl;
      impl* m_impl_p;
   };
}