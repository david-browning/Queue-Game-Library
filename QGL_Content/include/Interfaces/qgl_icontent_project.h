#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Buffers/qgl_content_metadata_buffer.h"

namespace qgl::content
{
   struct icontent_project : public iqgl
   {
      using content_project_entry_pair =
         std::pair<CONTENT_METADATA_BUFFER, file_string>;
      using cpep = content_project_entry_pair;
      using container = std::vector<content_project_entry_pair>;
      using iterator = container::iterator;
      using const_iterator = container::const_iterator;

      virtual ~icontent_project() noexcept
      {
      }

      /*
       Flushes any changes to the content file to the disk.
       */
      virtual void flush() = 0;

      /*
       Returns a reference to the content project's metadata.
       */
      virtual CONTENT_METADATA_BUFFER* metadata() noexcept = 0;

      /*
       Returns a const reference to the content project's metadata.
       */
      virtual const CONTENT_METADATA_BUFFER* metadata() const noexcept = 0;

      /*
       Returns the number of entries in the project.
       */
      virtual size_t size() const noexcept = 0;

      /*
       Constructs and places a project entry at the end.
       */
      virtual void emplace_back(
         const content_project_entry_pair::first_type* entry,
         const wchar_t* absPath) = 0;

      /*
       Returns a reference to the idx'th project entry.
       This throws out_of_range if the index is out of bounds.
       */
      virtual content_project_entry_pair& at(size_t idx) = 0;

      /*
       Returns a const reference to the idx'th project entry.
       This throws out_of_range if the index is out of bounds.
       */
      virtual const content_project_entry_pair& at(size_t idx) const = 0;

      /*
       Returns a reference to the idx'th project entry.
       This does no bounds checking.
       */
      virtual content_project_entry_pair& operator[](size_t idx) noexcept = 0;

      /*
       Returns a const reference to the idx'th project entry.
       This does no bounds checking.
       */
      virtual const content_project_entry_pair& operator[](
         size_t idx) const noexcept = 0;

        /*
       Returns the project entry at the given position.
       */
      virtual iterator erase(const_iterator position) = 0;

      /*
       Removes the project entries between first and last, inclusive.
       */
      virtual iterator erase(const_iterator first, const_iterator last) = 0;

      /*
       Returns an iterator to the beginning of the project entries.
       */
      virtual iterator begin() noexcept = 0;

      virtual const_iterator begin() const noexcept = 0;

      /*
       Returns a const iterator to the beginning of the project entries.
       */
      virtual const_iterator cbegin() const noexcept = 0;

      /*
       Returns an iterator to the end of the project entries.
       */
      virtual iterator end() noexcept = 0;

      virtual const_iterator end() const noexcept = 0;

      /*
       Returns a const iterator to the end of the project entries.
       */
      virtual const_iterator cend() const noexcept = 0;
   };

   /*
    Open a content project file in read-write mode.
    File path must be absolute.
    If the file does not exist, this creates a new one.
    When creating a new file, the content metadata is default and the list
    of project entries is empty.

    If the file exists, the constructor checks if the file is valid. If the
    file is not valid, this throws an exception.
    */
   extern QGL_CONTENT_API icontent_project* qgl_open_content_project(
      const wchar_t* filePath);

   /*
    Opens a content project file in read-write mode. The storage file must
    allow write permissions.
    If the file does not exist, this creates a new one.
    When creating a new file, the content metadata is default and the list
    of project entries is empty.

    If the file exists, the constructor checks if the file is valid. If the
    file is not valid, this throws an exception.
    */
   extern QGL_CONTENT_API icontent_project* qgl_open_content_project(
      const winrt::Windows::Storage::StorageFile& f);
}