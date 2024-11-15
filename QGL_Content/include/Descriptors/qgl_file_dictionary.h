#pragma once
#include "include/qgl_content_include.h"
#include "include/Descriptors/qgl_content_metadata.h"

namespace qgl::descriptors
{
   /*
    Describes an entry in a content file's dictionary.
    */
#pragma pack(push, 1)
   struct dictionary_entry final
   {
      constexpr dictionary_entry()
      {

      }

      dictionary_entry(const dictionary_entry&) = default;

      dictionary_entry(dictionary_entry&&) noexcept = default;

      ~dictionary_entry() noexcept = default;

      friend void swap(dictionary_entry& l, dictionary_entry& r) noexcept
      {
         using std::swap;
         swap(l.flags, r.flags);
         swap(l.metadata, r.metadata);
         swap(l.offset, r.offset);
         swap(l.size, r.size);
      }

      dictionary_entry& operator=(dictionary_entry r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Offset in the file (in bytes) to the object's data.
       */
      uint64_t offset = 0;

      /*
       Size (in bytes) of the object's data.
       */
      uint64_t size = 0;

      mem::flags<64, true> flags = 0;

      /*
       Information about the object that this dictionary entry points to.
       */
      content_metadata metadata;
   };
#pragma pack(pop)

   /*
    Describes a dictionary in a content file.
    */
   class file_dictionary final
   {
      public:
      using iterator = typename std::vector<dictionary_entry>::iterator;
      using const_iterator = typename std::vector<dictionary_entry>::const_iterator;

      /*
       Default constructor. Empty dictionary.
       */
      file_dictionary()
      {

      }

      /*
       Construct from data read from a file.
       */
      file_dictionary(std::vector<dictionary_entry>&& entries,
                      mem::flags<64, true>&& flags) :
         m_entries(std::forward<std::vector<dictionary_entry>>(entries)),
         m_flags(std::forward<mem::flags<64, true>>(flags))
      {

      }

      file_dictionary(const file_dictionary&) = default;

      file_dictionary(file_dictionary&&) noexcept = default;

      ~file_dictionary() noexcept = default;

      friend void swap(file_dictionary& l, file_dictionary& r) noexcept
      {
         using std::swap;
         swap(l.m_flags, r.m_flags);
         swap(l.m_entries, r.m_entries);
      }

      file_dictionary& operator=(file_dictionary r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Number of entries in the dictionary.
       */
      size_t size() const noexcept
      {
         return m_entries.size();
      }

      iterator begin() noexcept
      {
         return m_entries.begin();
      }

      iterator end() noexcept
      {
         return m_entries.end();
      }

      const_iterator begin() const noexcept
      {
         return m_entries.begin();
      }

      const_iterator end() const noexcept
      {
         return m_entries.end();
      }

      const_iterator cbegin() const noexcept
      {
         return m_entries.cbegin();
      }

      const_iterator cend() const noexcept
      {
         return m_entries.cend();
      }

      dictionary_entry& operator[](size_t i) noexcept
      {
         return m_entries[i];
      }

      const dictionary_entry& operator[](size_t i) const noexcept
      {
         return m_entries[i];
      }

      dictionary_entry& at(size_t i)
      {
         check_bounds(i);
         return this->operator[](i);
      }

      const dictionary_entry& at(size_t i) const
      {
         check_bounds(i);
         return this->operator[](i);
      }

      void push_back(const dictionary_entry& e)
      {
         m_entries.push_back(e);
      }

      void push_back(dictionary_entry&& e)
      {
         m_entries.push_back(std::forward<dictionary_entry>(e));
      }

      auto& flags()
      {
         return m_flags;
      }

      const auto& flags() const
      {
         return m_flags;
      }

      const dictionary_entry* data() const noexcept
      {
         return m_entries.data();
      }

      private:
      /*
       Checks that i is in bounds.
       */
      void check_bounds(size_t i) const
      {
         if (i >= m_entries.size())
         {
            throw std::out_of_range {"Index out of range."};
         }
      }

      std::vector<dictionary_entry> m_entries;

      /*
       Currently unused.
       */
      mem::flags<64, true> m_flags = 0;
   };
}