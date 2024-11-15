#pragma once
#include "include/qgl_content_include.h"
#include <compressapi.h>
#include <QGLPlatform.h>

namespace qgl::content::compression
{
   enum class compression_flags : uint8_t
   {
      // No compression.
      none = 0,

      // The file's content blocks are compressed.
      content = 1,

      // The file's dictionary is compressed.
      dictionary = 2,

      both = 3,
   };

   enum class compression_types : uint8_t
   {
      none = 0,

      // Very fast with low resource requirements
      // Medium compression ratio
      // High compression and decompression speeds
      // Low memory requirement
      xpress = 1,

      // Compression ratio is higher than COMPRESS_ALGORITHM_XPRESS
      // Medium compression ratio
      // Medium to high compression and decompression speeds
      // Low memory requirement
      xpress_huff = 2,

      // Good algorithm if the size of the data to be compressed is over 2MB.
      // High compression ratio
      // Low compression speed and high decompression speed
      // Medium to high memory requirement
      lzms = 3,
   };

   class compressor final
   {
      public:
      using compression_promise = typename std::promise<std::vector<std::byte>>;

      compressor(compression_types t) :
         m_type(t),
         m_cHndl(nullptr),
         m_dHndl(nullptr)
      {
         if (WIN32_MAP.count(t) == 0)
         {
            throw std::invalid_argument(
               "The compression type passed is not supported.");
         }

         auto compType = WIN32_MAP.at(m_type);
         winrt::check_bool(CreateCompressor(compType, nullptr, &m_cHndl));
         winrt::check_bool(CreateDecompressor(compType, nullptr, &m_dHndl));
      }

      compressor(const compressor& r) :
         m_type(r.m_type)
      {
         auto compType = WIN32_MAP.at(m_type);
         winrt::check_bool(CreateCompressor(compType, nullptr, &m_cHndl));
         winrt::check_bool(CreateDecompressor(compType, nullptr, &m_dHndl));
      }

      compressor(compressor&& r) noexcept
      {
         m_type = r.m_type;
         m_cHndl = r.m_cHndl;
         m_dHndl = r.m_dHndl;

         r.m_cHndl = nullptr;
         r.m_dHndl = nullptr;
      }

      ~compressor() noexcept
      {
         if (m_cHndl)
         {
            CloseCompressor(m_cHndl);
         }

         if (m_dHndl)
         {
            CloseDecompressor(m_dHndl);
         }
      }

      compression_types type() const noexcept
      {
         return m_type;
      }

      /*
       Gets the size necessary to hold data_p once it is compressed.
       */
      size_t csize(const std::byte* const data_p, size_t size) const
      {
         size_t ret = 0;
         auto result = Compress(m_cHndl, data_p, size,
                                nullptr, 0, &ret);
         if (!result && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
         {
            winrt::throw_last_error();
         }

         return  ret;
      }

      /*
       Gets the size necessary to hold data_p once it is decompressed.
       */
      size_t dsize(const std::byte* const data_p, size_t size) const
      {
         size_t ret = 0;
         auto result = Decompress(m_dHndl, data_p, size,
                                  nullptr, 0, &ret);
         if (!result && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
         {
            winrt::throw_last_error();
         }

         return ret;
      }

      /*
       Assume that out_p buffer is large enough to hold the compressed data.
       */
      void compress(const std::byte* data_p,
                    size_t size,
                    std::byte* const out_p,
                    size_t outSize) const
      {
         size_t compressedSize = 0;
         winrt::check_bool(Compress(m_cHndl, data_p, size,
                                    out_p, outSize, &compressedSize));
      }

      std::vector<std::byte> compress(const std::vector<std::byte>& data) const
      {
         size_t compressedSize = 0;
         std::vector<std::byte> ret{ csize(data.data(), data.size()) };
         winrt::check_bool(Compress(m_cHndl, data.data(), data.size(),
                                    ret.data(), ret.size(), &compressedSize));
         return ret;
      }

      /*
       Compress using a promise that can be offloaded to a new thread like so:
       compressor c { compression_types:: };
       std::vector<std::byte> data = {...};
       compressor::compression_promise p;
       auto f = p.get_future();
       std::thread compressThread { &compressor::async_compress, &c, data };

       // Other work

       auto compressed = f.get();
       compressThrad.join();
       */
      void async_compress(compression_promise&& promise,
                          const std::vector<std::byte>& data) const
      {
         size_t compressedSize = 0;
         std::vector<std::byte> ret{ csize(data.data(), data.size()) };
         auto result = Compress(m_cHndl, data.data(), data.size(),
                                ret.data(), ret.size(), &compressedSize);
         if (!result)
         {
            auto error_code = GetLastError();
            promise.set_exception(std::make_exception_ptr(
               std::system_error(error_code, 
                                 std::system_category(), 
                                 "Compression failed")));
         }

         promise.set_value(ret);
      }

      /*
       Assume that out_p is large enough to hold the decompressed data.
       */
      void decompress(const std::byte* data,
                      size_t size,
                      std::byte* const out_p,
                      size_t out_size) const
      {
         size_t decompressedSize = 0;
         winrt::check_bool(Decompress(m_dHndl, data, size,
                                      out_p, out_size, &decompressedSize));
      }

      
      std::vector<std::byte> decompress(const std::vector<std::byte>& data) const
      {
         size_t decompressedSize = 0;
         std::vector<std::byte> ret{ dsize(data.data(), data.size()) };
         winrt::check_bool(Decompress(m_dHndl, data.data(), data.size(),
                                      ret.data(), ret.size(), &decompressedSize));
         return ret;
      }

      /*
       Decompress using a promise that can be offloaded to a new thread like so:
       compressor c { compression_types:: };
       std::vector<std::byte> compressedData = {...};
       compressor::compression_promise p;
       auto f = p.get_future();
       std::thread decompressThread { &compressor::async_decompress, &c, compressedData };

       // Other work

       auto decompressed = f.get();
       decompressThrad.join();
       */
      void async_decompress(compression_promise&& promise,
                            const std::vector<std::byte>& data) const
      {
         size_t decompressedSize = 0;
         std::vector<std::byte> ret{ dsize(data.data(), data.size()) };
         auto result = Decompress(m_dHndl, data.data(), data.size(),
                                  ret.data(), ret.size(), &decompressedSize);

         if (!result)
         {
            auto error_code = GetLastError();
            promise.set_exception(std::make_exception_ptr(
               std::system_error(error_code,
                                 std::system_category(),
                                 "Decompression failed")));
         }

         promise.set_value(ret);
      }

      private:
      const std::unordered_map<compression_types, DWORD> WIN32_MAP
      {
         { compression_types::xpress, COMPRESS_ALGORITHM_XPRESS},
         { compression_types::xpress_huff, COMPRESS_ALGORITHM_XPRESS_HUFF},
         { compression_types::lzms, COMPRESS_ALGORITHM_LZMS }
      };

      /*
       Type of compression
       */
      compression_types m_type;

      /*
       Compressor handle
       */
      COMPRESSOR_HANDLE m_cHndl;

      /*
       Decompressor handle
       */
      COMPRESSOR_HANDLE m_dHndl;
   };
}
