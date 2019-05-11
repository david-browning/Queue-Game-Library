#pragma once
#include "include/qgl_content_include.h"
#include "include/qgl_content_header_buffer.h"
#include "include/qgl_content_dictionary.h"

namespace qgl::content
{
   class content_project;

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
       Only valid when the file was opened for writing. If the file was opened for read access,
       this throws bad_variant_access.
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

      inline const CONTENT_FILE_HEADER_BUFFER& header() const
      {
         return m_header;
      }

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

      static constexpr size_t READ_INDEX = 0;
      static constexpr size_t WRITE_INDEX = 1;

      CONTENT_FILE_HEADER_BUFFER m_header;
      content_dictionary m_dictionary;
      #pragma warning(push)
      #pragma warning(disable: 4251)
      std::vector<const void*> m_entryDataToWrite;
      winrt::file_handle m_handle;
      #pragma warning(pop)

      bool m_writeMode;
   };
}