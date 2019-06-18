#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Buffers/qgl_content_metadata_buffer.h"

namespace qgl::content
{
   class icontent_project : public iqgl
   {
      public:
      using content_project_entry_pair =
         std::pair<CONTENT_METADATA_BUFFER, file_string>;
      using cpep = content_project_entry_pair;
      using container = std::vector<content_project_entry_pair>;
      using iterator = container::iterator;
      using const_iterator = container::const_iterator;

      /*
       Flushes any changes to the content file to the disk.
       */
      virtual HRESULT flush() noexcept = 0;

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
       Constructs a shared entry and inserts it at the position.
       Returns:
         E_INVALIDARG if the string is not well formed. See qgl::content::
            entries::shared_content_entry for information about good strings.
         E_BOUNDS if the position is invalid.
         S_OK on success.
       */
      [[nodiscard]] virtual HRESULT insert_shared_entry(
         const content_project_entry_pair::first_type* entry,
         const wchar_t* str,
         size_t idx) noexcept = 0;

      /*
       Constructs a data entry in the content project. str must be an absolute
       path. This function does not check if the string points to a valid file.
       Returns:
         E_BOUNDS if the position is invalid.
         S_OK on success.
       */
      [[nodiscard]] virtual HRESULT insert_data_entry(
         const content_project_entry_pair::first_type* entry,
         const wchar_t* str,
         size_t idx) noexcept = 0;

      /*
       Constructs a shared entry and inserts it at the end.
       Returns:
         E_INVALIDARG if the string is not well formed. See qgl::content::
            entries::shared_content_entry for information about good strings.
         S_OK on success.
       */
      [[nodiscard]] virtual HRESULT emplace_shared_back(
         const content_project_entry_pair::first_type* entry,
         const wchar_t* str) = 0;

      /*
       Constructs a shared entry and inserts it at the end.
       Returns:
         S_OK on success.
       */
      [[nodiscard]] virtual HRESULT emplace_data_back(
         const content_project_entry_pair::first_type* entry,
         const wchar_t* str) = 0;

      /*
       Returns a reference to the idx'th project entry.
       Does not bounds checking.
       Do not free or reallocate the returned pointer.
       */
      virtual content_project_entry_pair* at(
         size_t idx) noexcept = 0;

      /*
       Returns a const reference to the idx'th project entry.
       Does not bounds checking.
       Do not free or reallocate the returned pointer.
       */
      virtual const content_project_entry_pair* at(
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

    Be sure to call release on the returned pointer or wrap it
    using make_unique.
    Returns:
      E_INVALIDARG if out_p is nullptr.
      E_OUTOFMEMORY if memory cannot be allocated.
      E_NOINTERFACE if the version is not correct.
      E_BADMAGIC if the file is not correct.
      S_OK if the pointer was constructed.
    */
   extern "C"[[nodiscard]] QGL_CONTENT_API HRESULT QGL_CC 
      qgl_open_content_project(
         const wchar_t* filePath,
         qgl_version_t v,
         icontent_project** out_p);

      /*
       Opens a content project file in read-write mode. The storage file must
       allow write permissions.
       If the file does not exist, this creates a new one.
       When creating a new file, the content metadata is default and the list
       of project entries is empty.

       Be sure to call release on the returned pointer or wrap it
       using make_unique.
       Returns:
         E_INVALIDARG if out_p is nullptr.
         E_OUTOFMEMORY if memory cannot be allocated.
         E_NOINTERFACE if the version is not correct.
         E_BADMAGIC if the file is not correct.
         S_OK if the pointer was constructed.
       */
   extern "C"[[nodiscard]] QGL_CONTENT_API HRESULT QGL_CC
      qgl_open_content_project_sf(
         const winrt::Windows::Storage::StorageFile& f,
         qgl_version_t v,
         icontent_project** out_p);
}