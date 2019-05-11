#pragma once
#include "include/qgl_content_include.h"
#include "include/qgl_content_dictionary.h"
#include "include/Content-Buffers/qgl_content_file_header_buffer.h"

namespace qgl::content
{
   class LIB_EXPORT content_file
   {
      public:

      /*
       Opens a file for reading.
       File path must be absolute.
       */
      content_file(const winrt::hstring& filePath);

      /*
       Opens a new file for writing.
       File path must be absolute.
       */
      content_file(const winrt::hstring& objName,
                   qgl_version_t comVersion,
                   RESOURCE_TYPES resourceType,
                   CONTENT_LOADER_IDS loaderType,
                   const winrt::hstring& filePath);

      /*
       Opens a new file for writing
       File path must be absolute.
       */
      content_file(const CONTENT_FILE_HEADER_BUFFER& headr,
                   const winrt::hstring& filePath);

      /*
       Cannot copy files.
       */
      content_file(const content_file&) = delete;

      /*
       Move constructor
       */
      content_file(content_file&& r) = default;

      virtual ~content_file();

      /*
       Appends a new dictionary entry and data to the content file.
       Only valid when the file was opened for writing. If the file was opened 
       for read access, this throws bad_variant_access.
       */
      template<typename T>
      void push_back(const CONTENT_DICTIONARY_ENTRY_BUFFER& entryBuffer,
                     const T* const data_p)
      {
         if (m_writeMode)
         {
            m_entryDataToWrite.push_back(reinterpret_cast<const void*>(data_p));
            m_dictionary.push_back(entryBuffer);
         }
         else
         {
            throw std::bad_variant_access();
         }
      }

      /*
       Returns a const reference to the idx'th item in the dictionary.
       Throws an exception if idx is out of range.
       */
      inline const CONTENT_DICTIONARY_ENTRY_BUFFER& at(size_t idx) const
      {
         return m_dictionary.at(idx);
      }

      /*
       Returns the number of items in the dictionary.
       */
      inline size_t count() const
      {
         return m_dictionary.size();
      }

      /*
       Returns a const reference to the file's header.
       */
      inline const CONTENT_FILE_HEADER_BUFFER& header() const
      {
         return m_header;
      }

      /*
       Returns a const reference to the file's handle.
       */
      inline const winrt::file_handle& handle() const
      {
         return m_handle;
      }

      void flush();

      private:
      void p_open_file_read(const winrt::hstring& filePath);

      void p_open_file_write(const winrt::hstring& filePath);

      void p_load_header();

      void p_load_dictionary();

      void p_write_header();

      void p_write_dictionary();

      void p_write_dict_data();

      /*
       Content file's header.
       */
      CONTENT_FILE_HEADER_BUFFER m_header;

      /*
       Content file's dictionary.
       */
      content_dictionary m_dictionary;
      
      #pragma warning(push)
      #pragma warning(disable: 4251)

      /*
       An array of pointer to data that is pending writes to the content file.
       This is only populated when the content file is in write mode.
       */
      std::vector<const void*> m_entryDataToWrite;
      
      /*
       File handle to this content file.
       Used for reading and writing the contents of this content file.
       */
      winrt::file_handle m_handle;
      #pragma warning(pop)

      /*
       True if the content file has write permissions.
       */
      bool m_writeMode;
   };
}