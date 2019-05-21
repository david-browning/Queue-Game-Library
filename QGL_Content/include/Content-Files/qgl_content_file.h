#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Dictionary/qgl_content_dictionary.h"
#include "include/Content-Buffers/qgl_content_file_header_buffer.h"
#include "include/Content-Files/qgl_content_file_helpers.h"
#include "include/qgl_file_helpers.h"
#include <variant>

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
   LIB_EXPORT class content_file
   {
      public:
      using content_buffer_type = std::variant<content_data_buffer_t, 
         shared_content_data_buffer_t>;

      using value_type = std::pair<CONTENT_DICTIONARY_ENTRY_BUFFER,
         content_buffer_type>;
      
      using data_container = std::vector<value_type>;
      using iterator = data_container::iterator;
      using const_iterator = data_container::const_iterator;

      /*
       Opens a content file in read write mode.
       File path must be absolute.
       If the file does not exist, this creates a new one.
       If the file exists, the constructor checks if the file is valid. If the
       file is not valid, this throws an exception.
       */
      content_file(const winrt::hstring& filePath);

      /*
       Opens a content file in read write mode.
       File path must be absolute.
       If the file does not exist, this creates a new one.
       If the file exists, the constructor checks if the file is valid. If the
       file is not valid, this throws an exception.
       */
      content_file(const winrt::Windows::Storage::StorageFile& f);

      /*
       Cannot copy a file.
       */
      content_file(const content_file&) = delete;

      /*
       Move constructor.
       */
      content_file(content_file&&) = default;

      /*
       The destructor does not flush the file but does close the file handle.
       If the file is not flushed and this goes out of scope, any changes to
       the file will be lost.
       */
      virtual ~content_file() noexcept = default;

      /*
       Flushes any changes to the content file to the disk.
       This function is only valid if the file was opened with write
       permissions.
       */
      void flush();

      const CONTENT_DICTIONARY_ENTRY_BUFFER& at(size_t idx) const;

      void push_back(const CONTENT_METADATA_BUFFER& meta,
                        const content_data_buffer_t& buff);

      void push_back(const CONTENT_METADATA_BUFFER& meta,
                        const shared_content_data_buffer_t& buff);

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
      const winrt::file_handle& handle() const noexcept;

      private:

      void read_in();

      void check_and_throw_file_size();

      /*
       File header.
       */
      CONTENT_FILE_HEADER_BUFFER m_header;

      /*
       An array of pointers to data that is pending writes to the content file.
       This is only populated when the content file is in write mode.
       */
      data_container m_entryDataToWrite;

      /*
       File handle.
       */
      winrt::file_handle m_handle;

      static constexpr size_t VARIANT_INDEX_BUFFER = 0;
      static constexpr size_t VARIANT_INDEX_PATH = 1;
   };
}