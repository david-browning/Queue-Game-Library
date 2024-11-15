#pragma once
#include "include/qgl_content_include.h"
#include "include/Descriptors/qgl_file_header.h"
#include "include/Descriptors/qgl_content_metadata.h"
#include "include/Descriptors/qgl_file_dictionary.h"
#include "include/Files/qgl_content_file_helpers.h"

namespace qgl::content
{
   /*
    Representation of a content file. A content file contains metadata about
    the content and a collection of data defining the content. The content data
    is addressed using a dictionary. Each dictionary entry has a GUID.
    The dictionary can be iterated through using the begin() and end() functions.

    Be sure to flush the file before this goes out of scope otherwise, changes
    will be discarded.
    */
   template<class FileHandle>
   class content_file final
   {
      public:
      struct staged_dict_entry final
      {
         constexpr staged_dict_entry()
         {
         }

         staged_dict_entry(const descriptors::content_metadata& m,
                           file_buffer_t&& d) :
            metadata(m),
            data(std::forward<file_buffer_t>(d))
         {

         }

         staged_dict_entry(const staged_dict_entry&) = default;

         staged_dict_entry(staged_dict_entry&&) noexcept = default;

         ~staged_dict_entry() noexcept = default;

         friend void swap(staged_dict_entry& l, staged_dict_entry& r) noexcept
         {
            using std::swap;
            swap(l.metadata, r.metadata);
            swap(l.data, r.data);
         }

         staged_dict_entry& operator=(staged_dict_entry r) noexcept
         {
            swap(*this, r);
            return *this;
         }

         descriptors::content_metadata metadata;
         file_buffer_t data;
      };

      using map_t = typename std::map<guid, staged_dict_entry>;
      using iterator = typename map_t::iterator;
      using const_iterator = typename map_t::const_iterator;

      /*
       Reads the data from the file. This takes ownership of the file handle.
       */
      content_file(FileHandle&& file) :
         m_hndl(std::forward<FileHandle>(file))
      {
         if (m_hndl.size() > 0)
         {
            read_contents();
         }
      }

      /*
       Do not allow copying a file.
       */
      content_file(const content_file&) = delete;

      /*
       Move constructor.
       */
      content_file(content_file&&) noexcept = default;

      /*
       Destructor. Be sure to call "flush()" before the destructor is invoked
       or any changes will not be saved.
       */
      ~content_file() noexcept = default;

      /*
       Flushes any changes to the content file to the disk.
       */
      void flush()
      {
         flush_contents();
      }

      /*
       Returns a reference to the file's metadata.
       */
      descriptors::content_metadata& metadata() noexcept
      {
         return m_metadata;
      }

      /*
       Returns a reference to the file's metadata.
       */
      const descriptors::content_metadata& metadata() const noexcept
      {
         return m_metadata;
      }

      /*
       Returns a reference to the metadata of an entry in the dictionary.
       */
      descriptors::content_metadata& metadata(const guid& g) noexcept
      {
         return m_entries.at(g).metadata;
      }

      /*
       Returns a reference to the metadata of an entry in the dictionary.
       */
      const descriptors::content_metadata& metadata(
         const guid& g) const noexcept
      {
         return m_entries.at(g).metadata;
      }

      /*
       Adds an entry to the dictionary. Specify the content's metadata and the
       raw byte buffer.
       */
      void insert(const descriptors::content_metadata& meta,
                  const std::byte* data,
                  size_t bytes)
      {
         auto& entry = m_entries[meta.id];
         entry.metadata = meta;
         entry.data.resize(bytes);
         memcpy(entry.data.data(), data, bytes);
      }

      /*
       Removes an entry from the dictionary.
       */
      void erase(const guid& g)
      {
         m_entries.erase(g);
      }

      const std::byte* at(const guid& g) const
      {
         return m_entries.at(g).data.data();
      }

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

      auto& dict_flags() noexcept
      {
         return m_dictFlags;
      }

      const auto& dict_flags() const noexcept
      {
         return m_dictFlags;
      }

      private:
      void read_contents()
      {
         // Read the file header.
         auto header = read_file_header(m_hndl);
         m_metadata = header.metadata;

         // Read the dictionary
         auto dict = read_file_dictionary(m_hndl, header);
         m_dictFlags = dict.flags();

         // For each entry:
         for (const auto& entry : dict)
         {
            // Read the content block. This also does the decompression.
            auto contentBuffer = read_content_block(m_hndl, entry);

            insert(entry.metadata, contentBuffer.data(), contentBuffer.size());
         }
      }

      void flush_contents()
      {
         auto curOffset = sizeof(descriptors::file_header);
         auto dictSize = m_entries.size() * sizeof(descriptors::dictionary_entry);

         // Create the file dictionary.
         // If a piece of content is compressed, set the dictionary entry's size
         // to the compressed size.
         descriptors::file_dictionary dict;
         dict.flags() = m_dictFlags;
         for (const auto& entry : m_entries)
         {
            descriptors::dictionary_entry newEntry;
            newEntry.metadata = entry.second.metadata;
            newEntry.flags = 0;
            newEntry.offset = curOffset;
            newEntry.size = entry.second.data.size();

            if (entry.second.metadata.compression_flags() ==
                compression::compression_flags::both ||
               entry.second.metadata.compression_flags() ==
                compression::compression_flags::content)
            {
               compression::compressor c{entry.second.metadata.compression_type()};
               auto compressedData = c.compress(entry.second.data);
               newEntry.size = compressedData.size();

               write_content_block(m_hndl, newEntry, compressedData);
            }
            else
            {
               write_content_block(m_hndl, newEntry, entry.second.data);
            }

            dict.push_back(std::move(newEntry));
            curOffset += newEntry.size;;
         }

         // Write the header and dictionary
         descriptors::file_header header;
         header.offset = sizeof(descriptors::file_header);
         header.metadata = m_metadata;
         write_file_header(m_hndl, header);
         write_file_dictionary(m_hndl, header, dict);
      }

      descriptors::content_metadata m_metadata;
      map_t m_entries;
      FileHandle m_hndl;
      mem::flags<64, true> m_dictFlags;
   };
}