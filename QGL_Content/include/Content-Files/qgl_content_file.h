#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Dictionary/qgl_content_dictionary.h"
#include "include/Content-Buffers/qgl_content_file_header_buffer.h"
#include "include/Content-Files/qgl_content_file_helpers.h"
#include "include/qgl_file_helpers.h"

namespace qgl::content
{
   /*
    Representation of a content file. A content file contains metadata about
    the content and a collection of data defining the content. The content data
    is addressed using a dictionary. The dictionary can be iterated through
    using the begin() and end() functions.
    WriteMode: Set to true to open the file with write permissions.

    File Structure:
    CONTENT_FILE_HEADER_BUFFER
    CONTENT_DICTIONARY_METADATA_BUFFER
    N * CONTENT_DICTIONARY_ENTRY_BUFFER
    N * Buffers
    */
   template<bool WriteMode>
   class content_file
   {
      public:
      using data_container = std::list<content_data_buffer_t>;
      using DataIterator = data_container::iterator;

      /*
       Opens a content file.
       File path must be absolute.
       If the file does not exist, this creates a new one only if WriteMode is
       true.
       If the file does not exist and WriteMode is false, then this throws
       invalid_argument.
       If the file exists, the constructor checks if the file is valid. If the
       file is not valid, this throws an exception.
       */
      content_file(const winrt::hstring& filePath)
      {
         //Check if the file exists.
         const bool existingFile = file_exists(filePath);

         if (WriteMode)
         {
            //Open file for read write mode.
            m_handle = open_file_readwrite(filePath);
         }
         else
         {
            //If read mode and the file does not exist:
            if (!existingFile)
            {
               throw std::invalid_argument("The file does not exist.");
            }

            //Open file for read mode.
            m_handle = open_file_read(filePath);
         }

         //File exists. Try to read it.
         if (existingFile)
         {
            check_and_throw_file_size();
            read_in();
         }
      }

      /*
       Opens a content file.
       If the file is empty, then this creates a new empty content file only if
       WriteMode is true and the handle has write permissions.
       If the file is not empty, this attempts to read and verify the content
       file data. If the file is not valid or the handle does not have read
       permissions, this throws an exception.
       */
      content_file(const winrt::Windows::Storage::StorageFile& f)
      {
         bool existingFile = file_size(f) > 0;
         if (WriteMode)
         {
            m_handle = open_file_readwrite(f);
         }
         else
         {
            //If read mode and the file does not exist:
            if (!existingFile)
            {
               throw std::invalid_argument("The file does not exist.");
            }

            //Open file for read mode.
            m_handle = open_file_read(f);
         }

         //File exists. Try to read it.
         if (existingFile)
         {
            check_and_throw_file_size();
            read_in();
         }
      }

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
        Throws an out_of_range if idx is out of range.
        */
      const CONTENT_DICTIONARY_ENTRY_BUFFER& at(size_t idx) const
      {
         return m_dictionary.at(idx);
      }

      /*
       Returns a reference to the idx'th item in the dictionary.
       Throws an out_of_range if idx is out of range.
       This function is only valid if the file was opened with write
       permissions.
       */
      template<typename = std::enable_if<WriteMode == true>>
      CONTENT_DICTIONARY_ENTRY_BUFFER& at(size_t idx)
      {
         return m_dictionary.at(idx);
      }

      /*
       Emplaces an entry to be written when flush is called.
       */
      template<typename = std::enable_if<WriteMode == true>>
      void emplace_back(const CONTENT_METADATA_BUFFER& entryMeta,
                        const content_data_buffer_t& buffer)
      {
         CONTENT_DICTIONARY_ENTRY_BUFFER dictEntry(buffer.size(),
                                                   entryMeta);
         m_dictionary.push_back(dictEntry);
         m_entryDataToWrite.push_back(buffer);
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
         //Write the header.
         write_header(m_handle, m_header);

         //Write the content data. This also updated the dictionary offsets, 
         //which is necessary before writing the dictionary.
         write_dictionary_content(m_handle,
                                  dictionary_data_offset(m_header,
                                                         m_dictionary.buffer()),
                                  m_dictionary.begin(),
                                  m_dictionary.end(),
                                  m_entryDataToWrite.begin(),
                                  m_entryDataToWrite.end());

         //Dictionary entry offsets should be correct now.
         //Write the dictionary.
         write_dictionary(m_handle,
                          m_header.dictionary_offset(),
                          m_dictionary);
      }

      private:

      void read_in()
      {
         //Read the file header.
         m_header = content_file_helpers::load_header(m_handle);

         //Read the dictionary.
         m_dictionary = content_file_helpers::load_dictionary(m_handle,
                                                              m_header.dictionary_offset());

         //Read all the entry data in.
         for (auto& entry : m_dictionary)
         {
            auto data = content_file_helpers::load_content_data(m_handle,
                                                                entry);
            m_entryDataToWrite.push_back(data);
         }

         if (!content_file_helpers::valid_content_file(m_handle))
         {
            throw std::invalid_argument("The file has incorrect data.");
         }
      }

      void check_and_throw_file_size()
      {
         if (!content_file_helpers::valid_content_file_size(m_handle))
         {
            throw std::invalid_argument(
               "The file exists, but it is too small to be valid");
         }
      }

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
      data_container m_entryDataToWrite;

      /*
       File handle.
       */
      winrt::file_handle m_handle;
   };

   using content_file_write = content_file<true>;
   using content_file_read = content_file<false>;
}