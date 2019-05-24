#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Buffers/qgl_content_dict_entry_buffer.h"
#include "include/Content-Buffers/qgl_content_file_header_buffer.h"
#include "include/Content-Files/qgl_content_variant_entry.h"
#include "include/qgl_file_handle.h"

namespace qgl::content
{
   /*
    Representation of a content file. A content file contains metadata about
    the content and a collection of data defining the content. The content data
    is addressed using a dictionary. The dictionary can be iterated through
    using the begin() and end() functions.

    File Structure:
    CONTENT_FILE_HEADER_BUFFER
    CONTENT_DICTIONARY_METADATA_BUFFER
    N * CONTENT_DICTIONARY_ENTRY_BUFFER
    N * Buffers
    */
   class LIB_EXPORT content_file
   {
      public:
      using dictionary_container = std::vector<CONTENT_DICTIONARY_ENTRY_BUFFER>;
      using content_container = std::vector<content_variant_entry>;

      using content_iterator = content_container::iterator;
      using content_const_iterator = content_container::const_iterator;

      using dictionary_iterator = dictionary_container::iterator;
      using const_dictionary_iterator = dictionary_container::const_iterator;

      /*
       Opens a content file in read write mode.
       File path must be absolute.
       If the file does not exist, this creates a new one.
       If the file exists, the constructor checks if the file is valid. If the
       file is not valid, this throws std::domain_error.
       */
      content_file(const winrt::hstring& filePath);

      /*
       Opens a content file in read write mode.
       File path must be absolute.
       If the file does not exist, this creates a new one.
       If the file exists, the constructor checks if the file is valid. If the
       file is not valid, this throws std::domain_error.
       */
      content_file(const winrt::Windows::Storage::StorageFile& f);

      /*
       Cannot copy a file.
       */
      content_file(const content_file&) = delete;

      /*
       Move constructor.
       */
      content_file(content_file&&);

      /*
       The destructor does not flush the file but does close the file handle.
       If the file is not flushed and this goes out of scope, any changes to
       the file will be lost.
       */
      virtual ~content_file() noexcept;

      /*
       Flushes any changes to the content file to the disk.
       This function is only valid if the file was opened with write
       permissions.
       */
      void flush();

      void push_back(const CONTENT_METADATA_BUFFER& meta,
                     const DATA_CONTENT_ENTRY& buff);

      void push_back(const CONTENT_METADATA_BUFFER& meta,
                     const SHARED_CONTENT_ENTRY& buff);

      /*
       Returns a const reference to the file's header.
       */
      const CONTENT_FILE_HEADER_BUFFER& header() const noexcept;

      /*
       Returns a reference to the file's header.
       */
      CONTENT_FILE_HEADER_BUFFER& header();

      /*
       Returns the number of items in the dictionary.
       */
      size_t size() const noexcept;

      /*
       Returns a const reference to the file's handle.
       The file handle is valid as long as this is in scope. The destructor
       closes the handle.
       */
      const file_handle& handle() const noexcept;

      CONTENT_DICTIONARY_ENTRY_BUFFER& operator[](size_t idx) noexcept;
      
      const CONTENT_DICTIONARY_ENTRY_BUFFER& operator[](size_t idx) 
         const noexcept;

      private:

      class impl;
      impl* m_impl_p;
   };
}