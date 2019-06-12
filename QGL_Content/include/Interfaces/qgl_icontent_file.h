#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Buffers/qgl_content_metadata_buffer.h"
#include "include/Content-Buffers/qgl_content_file_header_buffer.h"
#include "include/Content-Buffers/qgl_content_dict_entry_buffer.h"
#include "include/Content-Files/qgl_data_content_entry.h"
#include "include/Content-Files/qgl_shared_content_entry.h"
#include "include/qgl_file_handle.h"

namespace qgl::content
{
   /*
    Representation of a content file. A content file contains metadata about
    the content and a collection of data defining the content. The content data
    is addressed using a dictionary. The dictionary can be iterated through
    using the begin() and end() functions.
    */
   struct icontent_file : public iqgl
   {
      /*
       Flushes any changes to the content file to the disk.
       Returns:
         Error codes returned by file reading and writing functions. 
            See qgl_file_helpers.h
         S_OK if success.
       */
      virtual HRESULT flush() noexcept = 0;

      /*
       The metadata and buffer are copied and can go out of scope after calling
       this.
       */
      virtual void push_back(const CONTENT_METADATA_BUFFER* meta,
                             const DATA_CONTENT_ENTRY* buff) noexcept = 0;

      /*
       The metadata and buffer are copied and can go out of scope after calling
       this.
       */
      virtual void push_back(const CONTENT_METADATA_BUFFER* meta,
                             const SHARED_CONTENT_ENTRY* buff) noexcept = 0;

      /*
       Returns a const reference to the file's header.
       Do not free or reallocate the returned pointer.
       */
      virtual const CONTENT_FILE_HEADER_BUFFER* header() const noexcept = 0;

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
       Does not bounds checking.
       Do not free or reallocate the returned pointer.
       */
      virtual CONTENT_DICTIONARY_ENTRY_BUFFER* at(
         size_t idx) noexcept = 0;

      /*
       Does not bounds checking.
       Do not free or reallocate the returned pointer.
       */
      virtual const CONTENT_DICTIONARY_ENTRY_BUFFER* at(
         size_t idx) const noexcept = 0;
   };

   /*
    Opens a content file in read write mode.
    File path must be absolute.
    If the file does not exist, this creates a new one.
    If the file exists, the constructor checks if the file is valid. If the
    file is not valid, this throws std::domain_error.
    */
   extern "C" QGL_CONTENT_API HRESULT QGL_API_CC qgl_open_content_file(
      const wchar_t* filePath,
      qgl_version_t v,
      icontent_file** out_p);

   extern "C" QGL_CONTENT_API HRESULT QGL_API_CC qgl_open_content_file_sf(
      const winrt::Windows::Storage::StorageFile& f,
      qgl_version_t v,
      icontent_file** out_p);
}