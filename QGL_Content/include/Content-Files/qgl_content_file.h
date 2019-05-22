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
   class LIB_EXPORT content_file
   {
      public:
      struct content_buffer_type
      {
         content_buffer_type()
         {

         }

         content_buffer_type(const content_data_buffer_t& b) :
            m_buffer(b)
         {

         }
         content_buffer_type(const shared_content_data_buffer_t& b) :
            m_sharedBuffer(b)
         {

         }

         content_buffer_type(const content_buffer_type&) = default;

         content_buffer_type(content_buffer_type&&) = default;

         ~content_buffer_type() noexcept = default;

         bool shared() const noexcept
         {
            return m_sharedBuffer.size() != 0;
         }

         content_data_buffer_t m_buffer;
         shared_content_data_buffer_t m_sharedBuffer;
      };

      //using content_buffer_type = std::variant<content_data_buffer_t,
         //shared_content_data_buffer_t>;

      using dictionary_container = std::list<CONTENT_DICTIONARY_ENTRY_BUFFER>;
      using content_container = std::list<content_buffer_type>;

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

      void push_back(const CONTENT_METADATA_BUFFER& meta,
                     const content_data_buffer_t& buff);

      void push_back(const CONTENT_METADATA_BUFFER& meta,
                     const shared_content_data_buffer_t& buff);

      void pop_back();

      const CONTENT_DICTIONARY_ENTRY_BUFFER& front() const;

      const CONTENT_DICTIONARY_ENTRY_BUFFER& back() const;

      dictionary_iterator begin();

      const_dictionary_iterator begin() const;

      const_dictionary_iterator cbegin() const;

      dictionary_iterator end();

      const_dictionary_iterator end() const;

      const_dictionary_iterator cend() const;

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

      #pragma warning(push)
      #pragma warning(disable: 4251)
      /*
       An array of pointers to data that is pending writes to the content file.
       This is only populated when the content file is in write mode.
       */
      content_container m_entryDataToWrite;

      dictionary_container m_dict;

      /*
       File handle.
       */
      winrt::file_handle m_handle;

      #pragma warning(pop)

      static constexpr size_t VARIANT_INDEX_BUFFER = 0;
      static constexpr size_t VARIANT_INDEX_PATH = 1;
   };
}