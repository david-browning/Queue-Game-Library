#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Project/qgl_content_project_entry.h"
#include "include/Content-Buffers/qgl_content_metadata_buffer.h"

namespace qgl::content
{
   class icontent_project : public iqgl
   {
      public:
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
      virtual const CONTENT_METADATA_BUFFER* const_metadata() 
         const noexcept = 0;

      /*
       Returns the number of entries in the project.
       */
      virtual size_t size() const noexcept = 0;

      /*
       Constructs a shared entry and inserts it at the end.
       Returns:
         E_INVALIDARG if the string is not well formed. See qgl::content::
            entries::shared_content_entry for information about good strings.
         S_OK on success.
       */
      [[nodiscard]] virtual HRESULT push_shared_entry(
         const CONTENT_METADATA_BUFFER* entry,
         const wchar_t* str) = 0;

      /*
       Constructs a shared entry and inserts it at the end.
       Returns:
         S_OK on success.
       */
      [[nodiscard]] virtual HRESULT push_data_entry(
         const CONTENT_METADATA_BUFFER* entry,
         const wchar_t* str) = 0;

      /*
       Returns a reference to the idx'th project entry.
       Does not bounds checking.
       Do not free or reallocate the returned pointer.
       */
      virtual helpers::content_project_entry* at(size_t idx) noexcept = 0;

      /*
       Returns a const reference to the idx'th project entry.
       Does not bounds checking.
       Do not free or reallocate the returned pointer.
       */
      virtual const helpers::content_project_entry* const_at(
         size_t idx) const noexcept = 0;

      /*
       Removes the entry at the given index from the content file. All entries
       after the removed entry get shifted so the collection of entries is
       contiguous.
       Does nothing if the index is out of range.
       */
      virtual void erase(size_t position) noexcept = 0;

      /*
       Removes all entries from this content project.
       */
      virtual void clear() noexcept = 0;
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