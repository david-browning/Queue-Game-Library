#pragma once
#include "qgl_content_include.h"
#include "qgl_content_dict_entry_buffer.h"
#include "qgl_content_file_header_buffer.h"

namespace qgl::content
{
   class LIB_EXPORT content_project
   {
      public:
      using FileStringT = winrt::hstring;
      using entry_pair = std::pair<CONTENT_DICTIONARY_ENTRY_BUFFER, FileStringT>;
      using container = std::list<entry_pair>;
      using iterator = container::iterator;
      using const_iterator = container::const_iterator;

      content_project();

      content_project(const content_project& r) = delete;

      content_project(content_project&& r) = default;

      virtual ~content_project() noexcept = default;

      inline size_t count() const
      {
         return m_entries.size();
      }

      void clear();

      void emplace_back(const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
                        const winrt::hstring& absPath);

      void emplace(const_iterator position,
                   const CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
                   const winrt::hstring& absPath);

      void pop_back();

      iterator erase(const_iterator position);

      iterator erase(const_iterator first, const_iterator last);

      iterator begin() noexcept;

      const_iterator begin() const noexcept;

      const_iterator cbegin() const noexcept;

      iterator end() noexcept;

      const_iterator end() const noexcept;

      const_iterator cend() const noexcept;

      const CONTENT_FILE_HEADER_BUFFER& header() const
      {
         return m_header;
      }

      void header(const CONTENT_FILE_HEADER_BUFFER& hdr)
      {
         m_header = hdr;
      }

      friend void swap(content_project& first, content_project& second)
      {
         using std::swap;
         swap(first.m_entries, second.m_entries);
         swap(first.m_header, second.m_header);
      }

      content_project& operator=(content_project r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:

      #pragma warning(push)
      #pragma warning(disable: 4251)
      container m_entries;
      #pragma warning(pop)

      CONTENT_FILE_HEADER_BUFFER m_header;
   };

   extern LIB_EXPORT content_project load_project(const winrt::hstring& absPath);

   extern LIB_EXPORT void save_project(const content_project& proj, const winrt::hstring& absPath);
}