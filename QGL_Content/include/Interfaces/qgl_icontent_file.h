#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Buffers/qgl_content_metadata_buffer.h"
#include "include/Content-Buffers/qgl_content_file_header_buffer.h"
#include "include/Content-Buffers/qgl_content_dict_entry_buffer.h"
#include "include/qgl_file_handle.h"

namespace qgl::content
{
   /*
    Representation of a content file. A content file contains metadata about
    the content and a collection of data defining the content. The content data
    is addressed using a dictionary. The dictionary can be iterated through
    using the begin() and end() functions.
    */
   class icontent_file : public iqgl
   {
      public:
      /*
       Flushes any changes to the content file to the disk.
       Returns:
         Error codes returned by file reading and writing functions.
            See qgl_file_helpers.h
         S_OK if success.
       */
      virtual HRESULT flush() noexcept = 0;

      /*
       Returns a const reference to the file's header.
       Do not free or reallocate the returned pointer.
       */
      virtual const CONTENT_FILE_HEADER_BUFFER* const_header()
         const noexcept = 0;

      /*
       Returns a reference to the file's header.
       Do not free or reallocate the returned pointer.
       */
      virtual CONTENT_FILE_HEADER_BUFFER* header() noexcept = 0;

      /*
       Returns the number of items in the dictionary.
       */
      virtual size_t size() const noexcept = 0;

      /*
       Returns a const reference to the file's handle.
       The file handle is valid as long as this is in scope. The destructor
       closes the handle.
       */
      virtual const file_handle* handle() const noexcept = 0;

      /*
       Inserts a data entry at the end of this content file.
       The metadata and buffer are copied and can go out of scope after calling
       this.
       */
      virtual void push_data_entry(const CONTENT_METADATA_BUFFER* meta,
                                   const void* buff,
                                   size_t buffBytes) noexcept = 0;

      /*
       Inserts a shared entry at the end of this content file.
       The metadata and buffer are copied and can go out of scope after calling
       this.
       */
      virtual void push_shared_entry(const CONTENT_METADATA_BUFFER* meta,
                                     const wchar_t* str) noexcept = 0;

      /*
       Does not bounds checking.
       Do not free or reallocate the returned pointer.
       */
      virtual CONTENT_DICTIONARY_ENTRY_BUFFER* at(size_t idx) noexcept = 0;

      /*
       Does not bounds checking.
       Do not free or reallocate the returned pointer.
       */
      virtual const CONTENT_DICTIONARY_ENTRY_BUFFER* const_at(
         size_t idx) const noexcept = 0;

      /*
       Removes the entry at the given index from the content file. All entries
       after the removed entry get shifted so the collection of entries is
       contiguous.
       Does nothing if the index is out of range.
       */
      virtual void erase(size_t idx) noexcept = 0;
   };

   /*
    Opens a content file in read write mode.
    File path must be absolute.
    If the file does not exist, this creates a new one.
    If the file exists, the constructor checks if the file is valid. If the
    file is not valid, this throws std::domain_error.
    */
   extern "C"[[nodiscard]] QGL_CONTENT_API HRESULT QGL_CC
      qgl_open_content_file(
         const wchar_t* filePath,
         qgl_version_t v,
         icontent_file** out_p);

   extern "C"[[nodiscard]] QGL_CONTENT_API HRESULT QGL_CC
      qgl_open_content_file_sf(
         const winrt::Windows::Storage::StorageFile& f,
         qgl_version_t v,
         icontent_file** out_p);
}