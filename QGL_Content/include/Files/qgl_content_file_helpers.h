#pragma once
#include "include/qgl_content_include.h"
#include "include/Descriptors/qgl_file_header.h"
#include "include/Descriptors/qgl_file_dictionary.h"

namespace qgl::content
{
   namespace impl
   {
      using dict_count_type = typename uint64_t;
   }


   //////////////////////// Read Content File Functions ////////////////////////



   /*
    Synchronously reads the file header for a content file.
    */
   template<class FileHandle>
   inline descriptors::file_header read_file_header(FileHandle& h)
   {
      descriptors::file_header ret;
      h.read(sizeof(ret), reinterpret_cast<std::byte*>(&ret), 0);
      return ret;
   }

   /*
    Asynchronously reads the file header for a content file using a promise.
    */
   using read_header_promise = typename std::promise<descriptors::file_header>;
   template<class FileHandle>
   inline void read_file_header(FileHandle& h, read_header_promise&& p)
   {
      // Allocate the return data
      descriptors::file_header ret;

      // Set up a promise
      count_promise wp;
      auto wpFuture = wp.get_future();

      // Read the file header into the allocated data.
      h.async_read(std::move(wp), sizeof(ret), 
                   reinterpret_cast<std::byte*>(&ret), 0);

      // Wait for the read to finish.
      wpFuture.wait();

      // Pass the file header back to the promise.
      p.set_value(std::move(ret));
   }

   /*
    Synchronously reads the dictionary from a content file and does the optional
    decompression.
    */
   template<class FileHandle>
   inline descriptors::file_dictionary read_file_dictionary(
      FileHandle& h,
      const descriptors::file_header& header)
   {
      auto curOffset = header.offset;

      // Read the dictionary count. If the data is compressed, this is actually
      // the size of the dictionary.
      impl::dict_count_type numEntries = 0;
      h.read(sizeof(numEntries),
             reinterpret_cast<std::byte*>(&numEntries),
             curOffset);
      curOffset += sizeof(numEntries);

      // These fields will get set later.
      mem::flags<64, true> dictFlags;
      std::vector<descriptors::dictionary_entry> entries;

      switch (header.metadata.compression_flags())
      {
         case compression::compression_flags::dictionary:
         case compression::compression_flags::both:
         {
            // Read the compressed data
            file_buffer_t compressedData{ numEntries };
            h.read(numEntries, compressedData.data(), curOffset);

            // Decompress the data
            compression::compressor c{ header.metadata.compression_type() };
            auto decompressedData = c.decompress(compressedData);

            // Extract the number of entries and flags.
            memcpy(&numEntries, decompressedData.data(), sizeof(numEntries));
            memcpy(&dictFlags,
                   decompressedData.data() + sizeof(numEntries),
                   sizeof(dictFlags));

            // Extract the entries.
            entries.resize(numEntries);
            memcpy(entries.data(),
                   decompressedData.data() + sizeof(numEntries) + sizeof(dictFlags),
                   numEntries * sizeof(descriptors::dictionary_entry));

            break;
         }
         case compression::compression_flags::content:
         case compression::compression_flags::none:
         {
            h.read(sizeof(dictFlags),
                   reinterpret_cast<std::byte*>(&dictFlags),
                   curOffset);
            curOffset += sizeof(dictFlags);

            entries.resize(numEntries);
            auto dictSize = numEntries * sizeof(descriptors::dictionary_entry);

            // Read all the dictionary entries now.
            h.read(dictSize,
                   reinterpret_cast<std::byte*>(entries.data()),
                   curOffset);
            break;
         }
         default:
         {
            throw std::invalid_argument{ "Unknown compression flag." };
         }
      }

      return descriptors::file_dictionary{
         std::move(entries),
            std::move(dictFlags) };
   }

   using read_dictionary_promise = typename std::promise<descriptors::file_dictionary>;

   /*
    Asynchronously reads the dictionary from a content file and does the optional
    decompression.
    */
   template<class FileHandle>
   inline void read_file_dictionary(
      FileHandle& h,
      const descriptors::file_header& header,
      read_dictionary_promise&& p)
   {
      auto curOffset = header.offset;

      // Read the dictionary count. If the data is compressed, this is the size
      // of the compressed dictionary.
      impl::dict_count_type numEntries = 0;
      count_promise entriesPromise;
      auto entriesFuture = entriesPromise.get_future();
      h.async_read(std::move(entriesPromise),
                   sizeof(numEntries),
                   reinterpret_cast<std::byte*>(&numEntries),
                   curOffset);
      curOffset += sizeof(numEntries);
      entriesFuture.wait();

      // These fields will get set later.
      mem::flags<64, true> dictFlags;
      std::vector<descriptors::dictionary_entry> entries;

      switch (header.metadata.compression_flags())
      {
         case compression::compression_flags::dictionary:
         case compression::compression_flags::both:
         {
            // Read the compressed data
            file_buffer_t compressedData{ numEntries };
            count_promise compressedPromise;
            auto compressedFuture = compressedPromise.get_future();
            h.async_read(std::move(compressedPromise),
                         compressedData.size(),
                         compressedData.data(),
                         curOffset);

            // Decompress the data
            compression::compressor c{ header.metadata.compression_type() };
            auto decompressedData = c.decompress(compressedData);

            // Extract the number of entries and flags.
            memcpy(&numEntries, decompressedData.data(), sizeof(numEntries));
            memcpy(&dictFlags,
                   decompressedData.data() + sizeof(numEntries),
                   sizeof(dictFlags));

            // Extract the entries.
            entries.resize(numEntries);
            memcpy(entries.data(),
                   decompressedData.data() + sizeof(numEntries) + sizeof(dictFlags),
                   numEntries * sizeof(descriptors::dictionary_entry));

            break;
         }
         case compression::compression_flags::content:
         case compression::compression_flags::none:
         {
            count_promise flagsPromise;
            auto flagsFuture = flagsPromise.get_future();
            h.read(std::move(flagsPromise),
                   sizeof(dictFlags),
                   reinterpret_cast<std::byte*>(&dictFlags),
                   curOffset);

            curOffset += sizeof(dictFlags);
            auto dictSize = numEntries * sizeof(descriptors::dictionary_entry);

            // Read all the dictionary entries now.
            entries.resize(numEntries);
            count_promise entriesPromise;
            auto entriesFuture = entriesPromise.get_future();
            h.async_read(std::move(entriesPromise),
                         dictSize,
                         reinterpret_cast<std::byte*>(entries.data()),
                         curOffset);

            flagsFuture.wait();
            entriesFuture.wait();

            break;
         }
         default:
         {
            throw std::invalid_argument{ "Unknown compression flag." };
         }
      }

      descriptors::file_dictionary ret{
         std::move(entries),
            std::move(dictFlags) };
      p.set_value(std::move(ret));
   }

   /*
    Synchronously reads the content block and does the optional decompression.
    */
   template<class FileHandle>
   inline file_buffer_t read_content_block(
      FileHandle& h,
      const descriptors::dictionary_entry& entry)
   {
      file_buffer_t ret;
      switch (entry.metadata.compression_flags())
      {
         case compression::compression_flags::content:
         case compression::compression_flags::both:
         {
            file_buffer_t compressedData{ entry.size };
            h.read(entry.size, compressedData.data(), entry.offset);
            compression::compressor c{ entry.metadata.compression_type() };
            ret = c.decompress(compressedData);
            break;
         }
         case compression::compression_flags::dictionary:
         case compression::compression_flags::none:
         {
            // Just read the content block
            ret.resize(entry.size);
            h.read(entry.size, ret.data(), entry.offset);
            break;
         }
         default:
         {
            throw std::invalid_argument{ "Unknown compression flag." };
         }
      }

      return ret;
   }

   /*
    Asynchronously reads the content block and does the optional decompression.
    */
   template<class FileHandle>
   inline void read_content_block(
      FileHandle& h,
      const descriptors::dictionary_entry& entry,
      read_promise&& p)
   {
      file_buffer_t ret;
      count_promise readPromise;
      auto readFuture = readPromise.get_future();

      switch (entry.metadata.compression_flags())
      {
         case compression::compression_flags::content:
         case compression::compression_flags::both:
         {
            file_buffer_t compressedData{ entry.size };
            h.async_read(std::move(readPromise),
                         entry.size,
                         compressedData.data(),
                         entry.offset);
            readFuture.wait();

            compression::compressor c{ entry.metadata.compression_type() };
            ret = c.decompress(compressedData);
            break;
         }
         case compression::compression_flags::dictionary:
         case compression::compression_flags::none:
         {
            // Just read the content block
            ret.resize(entry.size);
            h.async_read(std::move(readPromise),
                         entry.size,
                         ret.data(),
                         entry.offset);
            readFuture.wait();
            break;
         }
         default:
         {
            throw std::invalid_argument{ "Unknown compression flag." };
         }
      }

      p.set_value(ret);
   }



   /////////////////////// Write Content File Functions ///////////////////////



   /*
    Synchronously writes the file header to a file.
    */
   template<class FileHandle>
   inline void write_file_header(
      FileHandle& h,
      const descriptors::file_header& header)
   {
      // File header is the first thing in the file.
      h.write(sizeof(header), reinterpret_cast<const std::byte*>(&header), 0);
   }

   /*
    Asynchronously writes the file header to a file.
    */
   template<class FileHandle>
   inline void write_file_header(
      FileHandle& h,
      const descriptors::file_header& header,
      count_promise&& p)
   {
      h.async_write(std::forward<count_promise>(p),
                    sizeof(header),
                    reinterpret_cast<const std::byte*>(&header),
                    0);
   }

   /*
    Synchronously writes the dictionary to the file.
    */
   template<class FileHandle>
   inline void write_file_dictionary(
      FileHandle& h,
      const descriptors::file_header& header,
      const descriptors::file_dictionary& dict)
   {
      if (header.offset != sizeof(header))
      {
         throw std::runtime_error{
            "The header size and dictionary offset do not match." };
      }

      auto dictCount = static_cast<impl::dict_count_type>(dict.size());
      auto dictFlags = dict.flags();
      auto curOffset = header.offset;

      switch (header.metadata.compression_flags())
      {
         case compression::compression_flags::dictionary:
         case compression::compression_flags::both:
         {
            // Combine the dictionary count, flags, and dictionary entries.
            auto uncompressedSize = sizeof(impl::dict_count_type) + sizeof(dict.flags()) +
               (sizeof(descriptors::dictionary_entry) * dict.size());

            file_buffer_t uncompressedData{ uncompressedSize };
            memcpy(uncompressedData.data(), &dictCount, sizeof(dictCount));
            memcpy(uncompressedData.data() + sizeof(dictCount),
                   &dictFlags,
                   sizeof(dictFlags));
            memcpy(uncompressedData.data() + sizeof(dictCount) + sizeof(dictFlags),
                   dict.data(),
                   dictCount * sizeof(descriptors::dictionary_entry));

            // Compress the data.
            compression::compressor c{ header.metadata.compression_type() };
            auto compressedData = c.compress(uncompressedData);

            // Write the size of the compressed data
            auto compressedSize = static_cast<impl::dict_count_type>(compressedData.size());
            h.write(sizeof(compressedSize),
                    reinterpret_cast<const std::byte*>(&compressedSize),
                    curOffset);
            curOffset += sizeof(compressedSize);

            // Write the compressed data
            h.write(compressedSize,
                    compressedData.data(),
                    curOffset);

            break;
         }
         case compression::compression_flags::content:
         case compression::compression_flags::none:
         {
            // Write the dictionary count.
            h.write(sizeof(dictCount),
                    reinterpret_cast<const std::byte*>(&dictCount),
                    curOffset);
            curOffset += sizeof(dictCount);

            // Write the dictionary flags
            h.write(sizeof(dictFlags),
                    reinterpret_cast<const std::byte*>(&dictFlags),
                    curOffset);
            curOffset += sizeof(dictFlags);

            // Write the dictionary entries.
            h.write(dictCount * sizeof(descriptors::dictionary_entry),
                    reinterpret_cast<const std::byte*>(dict.data()),
                    curOffset);

            break;
         }
         default:
         {
            throw std::invalid_argument{ "Unknown compression flag." };
         }
      }
   }

   /*
    Asynchronously writes the dictionary to the file.
    */
   template<class FileHandle>
   inline void write_file_dictionary(
      FileHandle& h,
      const descriptors::file_header& header,
      const descriptors::file_dictionary& dict,
      count_promise&& p)
   {
      if (header.offset != sizeof(header))
      {
         throw std::runtime_error{
            "The header size and dictionary offset do not match." };
      }

      size_t writeSize = 0;
      auto dictCount = static_cast<impl::dict_count_type>(dict.size());
      auto dictSize = dictCount * sizeof(descriptors::dictionary_entry);
      auto dictFlags = dict.flags();
      auto curOffset = header.offset;

      switch (header.metadata.compression_flags())
      {
         case compression::compression_flags::content:
         case compression::compression_flags::none:
         {
            // Combine the dictionary count, flags, and dictionary entries.
            auto uncompressedSize = sizeof(dictCount) +
               sizeof(dict.flags()) +
               (sizeof(descriptors::dictionary_entry) * dict.size());

            file_buffer_t uncompressedData{ uncompressedSize };
            memcpy(uncompressedData.data(), &dictCount, sizeof(dictCount));
            memcpy(uncompressedData.data() + sizeof(dictCount),
                   &dictFlags,
                   sizeof(dictFlags));
            memcpy(uncompressedData.data() + sizeof(dictCount) + sizeof(dictFlags),
                   dict.data(),
                   dictCount * sizeof(descriptors::dictionary_entry));

            compression::compressor c{ header.metadata.compression_type() };
            auto compressedData = c.compress(uncompressedData);

            // Write the size of the compressed data
            auto compressedSize = static_cast<impl::dict_count_type>(compressedData.size());
            count_promise sizePromise;
            auto sizeFuture = sizePromise.get_future();
            h.async_write(std::move(sizePromise),
                          sizeof(compressedSize),
                          reinterpret_cast<const std::byte*>(&compressedSize),
                          curOffset);

            // Write the compressed data
            count_promise dictPromise;
            auto dictFuture = dictPromise.get_future();
            h.async_write(std::move(dictPromise),
                          compressedSize,
                          compressedData.data(),
                          curOffset + sizeof(compressedSize));

            writeSize = sizeof(dictCount) + compressedSize;
            sizeFuture.wait();
            dictFuture.wait();
            break;
         }
         case compression::compression_flags::dictionary:
         case compression::compression_flags::both:
         {
            // Write the dictionary count.
            count_promise dictCountPromise;
            auto dictCountFuture = dictCountPromise.get_future();
            h.async_write(std::move(dictCountPromise),
                          sizeof(dictCount),
                          reinterpret_cast<const std::byte*>(&dictCount),
                          curOffset);

            // Write the dictionary flags.
            count_promise flagsPromise;
            auto flagsFuture = flagsPromise.get_future();
            h.write(std::move(flagsPromise),
                    sizeof(dictFlags),
                    reinterpret_cast<const std::byte*>(&dictFlags),
                    curOffset + sizeof(dictCount));

            // Write the dictionary entries.
            count_promise dictPromise;
            auto dictFuture = dictPromise.get_future();
            h.async_write(std::move(dictPromise),
                          dictSize,
                          reinterpret_cast<const std::byte*>(dict.data()),
                          curOffset + sizeof(dictCount) + sizeof(dictFlags));

            writeSize = sizeof(dictCount) + sizeof(dictFlags) + dictSize;
            break;
         }
         default:
         {
            throw std::invalid_argument{ "Unknown compression flag." };
         }
      }

      p.set_value(writeSize);
   }

   /*
    Synchronously writes the buffer to the content file at the entry's offset.
    Unlike other helper functions, this does not check for compression or
    compress the buffer before writing it to the file. This assumes that the
    buffer was already compressed.
    */
   template<class FileHandle>
   inline void write_content_block(
      FileHandle& h,
      const descriptors::dictionary_entry& entry,
      const file_buffer_t& buffer)
   {
      if (entry.size != buffer.size())
      {
         throw std::invalid_argument{"The buffer sizes don't match."};
      }

      h.write(buffer.size(),
           buffer.data(),
           entry.offset);
   }

   /*
    Asynchronously writes the buffer to the content file at the entry's offset.
    Unlike other helper functions, this does not check for compression or
    compress the buffer before writing it to the file. This assumes that the
    buffer was already compressed.
    */
   template<class FileHandle>
   inline void write_content_block(
      FileHandle& h,
      const descriptors::dictionary_entry& entry,
      const file_buffer_t& buffer,
      count_promise&& p)
   {
      if (entry.size != buffer.size())
      {
         throw std::invalid_argument{"The buffer sizes don't match."};
      }

      h.async_write(std::forward<count_promise>(p),
                   buffer.size(),
                   buffer.data(),
                   entry.offset);
   }
}