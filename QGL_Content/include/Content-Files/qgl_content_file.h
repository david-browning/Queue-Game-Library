#pragma once
#include "include/qgl_content_include.h"
#include "include/qgl_content_dictionary.h"
#include "include/Content-Buffers/qgl_content_file_header_buffer.h"
#include "include/Content-Files/qgl_content_file_helpers.h"

namespace qgl::content
{
   /*
    Representation of a content file. A content file contains metadata about
    the content and a collection of data defining the content. The content data
    is addressed using a dictionary. The dictionary can be iterated through
    using the begin() and end() functions.
    WriteMode: Set to true to open the file with write permissions.
    */
   template<bool WriteMode>
   class content_file
   {
      public:
      /*
       Opens a content file.
       File path must be absolute.
       If the file does not exist, this creates a new one only if WriteMode is
       true.
       If the file does not exist and WriteMode is false, then this throws an
       exception.
       If the file exists, the constructor checks if the file is valid. If the
       file is not valid, this throws an exception.
       */
      content_file(const winrt::hstring& filePath);

      /*
       Cannot copy a file.
       */
      content_file(const content_file&) = delete;

      /*
       Move constructor.
       */
      content_file(content_file&& m) = default;

      /*
       The destructor does not flush the file but does close the file handle. 
       If the file is not flushed and this goes out of scope, any changes to 
       the file will be lost.
       */
      virtual ~content_file() noexcept = default;

       /*
        Returns a const reference to the idx'th item in the dictionary.
        Throws an exception if idx is out of range.
        */
      const CONTENT_DICTIONARY_ENTRY_BUFFER& at(size_t idx) const
      {
         return m_dictionary.at(idx);
      }

      /*
       Returns a reference to the idx'th item in the dictionary.
       Throws an exception if idx is out of range.
       This function is only valid if the file was opened with write 
       permissions.
       */
      template<typename = std::enable_if<WriteMode == true>>
      CONTENT_DICTIONARY_ENTRY_BUFFER& at(size_t idx)
      {
         return m_dictionary.at(idx);
      }

      /*
       Returns the number of items in the dictionary.
       */
      inline size_t size() const noexcept
      {
         return m_dictionary.size();
      }

      /*
       Returns a const reference to the file's header.
       */
      const CONTENT_FILE_HEADER_BUFFER& header() const noexcept
      {
         return m_header;
      }

      /*
       Returns a reference to the file's header.
       This function is only valid if the file was opened with write
       permissions.
       */
      template<typename = std::enable_if<WriteMode == true>>
      CONTENT_FILE_HEADER_BUFFER& header()
      {
         return m_header;
      }

      /*
       Returns a const reference to the file's handle.
       The file handle is valid as long as this is in scope. The destructor
       closes the handle.
       */
      const winrt::file_handle& handle() const noexcept
      {
         return m_handle;
      }

      /*
       Flushes any changes to the content file to the disk.
       This function is only valid if the file was opened with write 
       permissions.
       */
      template<typename = std::enable_if<WriteMode == true>>
      void flush()
      {
         write_header(m_handle, m_header);
         write_dictionary(m_handle,
                          m_header.dictionary_offset(),
                          m_dictionary);

         write_dictionary_data(m_handle,
                               dictionary_data_offset(m_header,
                                                      m_dictionary.buffer()),
                               m_dictionary.begin(),
                               m_dictionary.end(),
                               m_entryDataToWrite.begin(),
                               m_entryDataToWrite.end());
      }

      private:
      
      /*
       File header.
       */
      CONTENT_FILE_HEADER_BUFFER m_header;

      /*
       File dictionary.
       */
      content_dictionary m_dictionary;

      /*
       An array of pointers to data that is pending writes to the content file.
       This is only populated when the content file is in write mode.
       */
      std::list<const void*> m_entryDataToWrite;

      /*
       File handle.
       */
      winrt::file_handle m_handle;
   };
}