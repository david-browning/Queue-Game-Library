#include "pch.h"
#include "CppUnitTest.h"
#include <random>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl::content::compression;

namespace QGL_Content_UnitTests
{
   TEST_CLASS(CompressionFutureTests)
   {
      public:
      TEST_METHOD(CompressWithFuture)
      {
         std::random_device rd;
         std::mt19937 gen{ rd() };
         std::vector<std::byte> uncompressedData{ 2048 };

         // Generate random bytes and store them in the vector
         std::generate_n(uncompressedData.begin(),
                         uncompressedData.size(),
                         [&]() { return static_cast<std::byte>(gen()); });

         compressor c{ compression_types::xpress };
         compressor::compression_promise compressPromise;
         auto compressFuture = compressPromise.get_future();
         std::thread compressThread{
            &compressor::async_compress,
            &c,
            std::move(compressPromise),
            uncompressedData };

         auto compressedData = compressFuture.get();
         compressThread.join();

         compressor::compression_promise decompressPromise;
         auto decompressFuture = decompressPromise.get_future();
         std::thread decompressThread{
            &compressor::async_decompress,
            &c,
            std::move(decompressPromise),
            compressedData };

         auto decompressedData = decompressFuture.get();

         Assert::AreEqual(
            uncompressedData.size(), decompressedData.size(),
            L"The uncompressed and decompressed data is not the same length");
         for (size_t i = 0; i < uncompressedData.size(); i++)
         {
            Assert::AreEqual(
               uncompressedData[i], decompressedData[i],
               L"Decompressed byte is not the same as the uncompressed byte.");
         }

         decompressThread.join();
      }
   };
}