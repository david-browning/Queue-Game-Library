#pragma once
#include "include/qgl_content_include.h"
#include "include/qgl_content_dictionary_buffer.h"
#include "include/qgl_content_dict_entry_buffer.h"

namespace qgl::content
{
   class LIB_EXPORT content_dictionary
   {
      public:
      /*
       Creates an empty dictionary.
       */
      content_dictionary();

      /*
       Creates a dictionary from the header. The dictionary is empty and must be populated
       using the accesses functions.
       */
      content_dictionary(const CONTENT_DICTIONARY_BUFFER& buf);

      content_dictionary(const content_dictionary& r);

      content_dictionary(content_dictionary&& r);

      virtual ~content_dictionary();

      inline void push_back(const CONTENT_DICTIONARY_ENTRY_BUFFER& entry)
      {
         m_entries.push_back(entry);
      }

      inline void push_back(CONTENT_DICTIONARY_ENTRY_BUFFER&& entry)
      {
         m_entries.push_back(entry);
      }

      inline const CONTENT_DICTIONARY_ENTRY_BUFFER& at(size_t idx) const
      {
         return m_entries.at(idx);
      }

      inline CONTENT_DICTIONARY_ENTRY_BUFFER& at(size_t idx)
      {
         return m_entries.at(idx);
      }

      inline const CONTENT_DICTIONARY_ENTRY_BUFFER& operator[](size_t idx) const
      {
         return m_entries[idx];
      }

      inline CONTENT_DICTIONARY_ENTRY_BUFFER& operator[](size_t idx)
      {
         return m_entries[idx];
      }

      inline auto cbegin() const
      {
         return m_entries.cbegin();
      }

      inline auto cend() const
      {
         return m_entries.cend();
      }

      inline auto begin()
      {
         return m_entries.begin();
      }

      inline auto end()
      {
         return m_entries.end();
      }

      inline const CONTENT_DICTIONARY_BUFFER& buffer() const
      {
         return m_buffer;
      }

      inline size_t size() const
      {
         return m_entries.size();
      }

      friend void swap(content_dictionary& first,
                       content_dictionary& second) noexcept
      {
         using std::swap;
         swap(first.m_buffer, second.m_buffer);
         swap(first.m_entries, second.m_entries);
      }

      content_dictionary& operator=(content_dictionary r)  noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:
      CONTENT_DICTIONARY_BUFFER m_buffer;
      #pragma warning(push)
      #pragma warning(disable: 4251)
      std::vector<CONTENT_DICTIONARY_ENTRY_BUFFER> m_entries;
      #pragma warning(pop)
   };
}