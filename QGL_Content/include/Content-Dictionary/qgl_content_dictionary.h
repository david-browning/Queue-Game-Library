#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Buffers/qgl_content_dictionary_metadata_buffer.h"
#include "include/Content-Buffers/qgl_content_dict_entry_buffer.h"

namespace qgl::content
{
    /*
     Content files contain a dictionary to lookup data related to the content.
     A dictionary stores information about data. Each entry contains some
     metadata about the data, as well as an offset to the data.
     */
   class LIB_EXPORT content_dictionary
   {
      public:
      using DictionaryEntryIterator = 
         std::vector<CONTENT_DICTIONARY_ENTRY_BUFFER>::iterator;

      /*
       Creates an empty dictionary.
       */
      content_dictionary();

      /*
       Creates a dictionary from the header. The dictionary is empty and must
       be populated using the accesses functions.
       */
      content_dictionary(const CONTENT_DICTIONARY_METADATA_BUFFER& buf);

      /*
       Copy constructor.
       */
      content_dictionary(const content_dictionary& r) = default;

      /*
       Move constructor.
       */
      content_dictionary(content_dictionary&& r) = default;

      /*
       Destructor
       */
      virtual ~content_dictionary() noexcept = default;

      /*
       Copies the entry into the dictionary.
       */
      void push_back(const CONTENT_DICTIONARY_ENTRY_BUFFER& entry);

      /*
       Moves the dictionary entry into the dictionary.
       */
      void push_back(CONTENT_DICTIONARY_ENTRY_BUFFER&& entry);

      /*
       Returns a const reference to the idx'th item in the dictionary.
       This throws an exception if the index is out of bounds.
       */
      inline const CONTENT_DICTIONARY_ENTRY_BUFFER& at(size_t idx) const
      {
         return m_entries.at(idx);
      }

      /*
       Returns a reference to the idx'th item in the dictionary.
       This throws an exception if the index is out of bounds.
       */
      inline CONTENT_DICTIONARY_ENTRY_BUFFER& at(size_t idx)
      {
         return m_entries.at(idx);
      }

      /*
       Returns a const reference to the idx'th item in the dictionary.
       */
      inline const CONTENT_DICTIONARY_ENTRY_BUFFER& operator[](
         size_t idx) const noexcept
      {
         return m_entries[idx];
      }

      /*
       Returns a reference to the idx'th item in the dictionary.
       */
      inline CONTENT_DICTIONARY_ENTRY_BUFFER& operator[](
         size_t idx) noexcept
      {
         return m_entries[idx];
      }

      /*
       Returns a const iterator to the beginning of the dictionary.
       */
      inline auto cbegin() const noexcept
      {
         return m_entries.cbegin();
      }

      /*
       Returns a const iterator to the end of the dictionary.
       */
      inline auto cend() const noexcept
      {
         return m_entries.cend();
      }

      /*
       Returns an iterator to the beginning of the dictionary.
       */
      inline auto begin() noexcept
      {
         return m_entries.begin();
      }

      /*
       Returns an iterator to the end of the dictionary.
       */
      inline auto end() noexcept
      {
         return m_entries.end();
      }

      /*
       Returns the number of entries in the dictionary.
       */
      inline size_t size() const
      {
         return m_entries.size();
      }

      friend void swap(content_dictionary& first,
                       content_dictionary& second) noexcept
      {
         using std::swap;
         swap(first.m_entries, second.m_entries);
      }
            
      /*
       Returns a metadata buffer that is used for writing the dictionary
       to a file.
       */
      inline const CONTENT_DICTIONARY_METADATA_BUFFER buffer() const
      {
         return CONTENT_DICTIONARY_METADATA_BUFFER(
            m_entries.size(),
            sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER),
            0);
      }

      /*
       Overwrites this with r and returns a reference to this.
       */
      content_dictionary& operator=(content_dictionary r)  noexcept
      {
         swap(*this, r);
         return *this;
      }


      /*
       Returns true if the two dictionaries have the same metadata and
       entries.
       */
      friend bool operator==(const content_dictionary& r,
                             const content_dictionary& l) noexcept
      {
         return r.m_entries == l.m_entries;
      }

      private:

      #pragma warning(push)
      #pragma warning(disable: 4251)
      std::vector<CONTENT_DICTIONARY_ENTRY_BUFFER> m_entries;
      #pragma warning(pop)
   };
}