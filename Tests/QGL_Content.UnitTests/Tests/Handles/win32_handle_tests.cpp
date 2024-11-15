#include "pch.h"
#include "CppUnitTest.h"
#include <random>
#include <winrt/Windows.Storage.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   TEST_CLASS(Win32FileHandleTests)
   {
      public:
      TEST_METHOD(OpenNew)
      {
         std::wstring path{ installed_path() + L"/opennew.txt"};
         win32_file_handle h{ path, file_open_modes::readwrite };
         
         // This should not throw
      }

      TEST_METHOD(OpenExisting)
      {
         std::wstring path{ installed_path() + L"/openExisting.txt"};
         if (file_exists(path))
         {
            delete_file(path);
         }

         win32_file_handle createHandle{ path, file_open_modes::readwrite };
         Assert::AreEqual(static_cast<size_t>(0), createHandle.size(),
                          L"New file size should be 0.");

         std::random_device rd;
         std::mt19937 gen{ rd() };
         std::vector<std::byte> data{ 2048 };

         // Generate random bytes and store them in the vector
         std::generate_n(data.begin(),
                         data.size(),
                         [&]() { return static_cast<std::byte>(gen()); });

         createHandle.write(data.size(), data.data(), 0);
         createHandle.close();

         win32_file_handle openHandle{ path, file_open_modes::readwrite };
         Assert::AreEqual(data.size(), openHandle.size(),
                          L"The opened file does not the correct size.");

         std::vector<std::byte> readData{openHandle.size()};
         openHandle.read(readData.size(), readData.data(), 0);
         for (size_t i = 0; i < readData.size(); i++)
         {
            Assert::AreEqual(data[i], readData[i], L"The bytes are not equal.");
         }
      }

      TEST_METHOD(Future)
      {
         std::wstring path{ installed_path() + L"/fileSize.txt"};
         if (file_exists(path))
         {
            delete_file(path);
         }

         win32_file_handle h{ path, file_open_modes::readwrite };
         Assert::AreEqual(static_cast<size_t>(0), h.size(),
                          L"New file size should be 0.");

         std::random_device rd;
         std::mt19937 gen{ rd() };
         std::vector<std::byte> data{ 2048 };

         // Generate random bytes and store them in the vector
         std::generate_n(data.begin(),
                         data.size(),
                         [&]() { return static_cast<std::byte>(gen()); });

         count_promise writePromise;
         auto writeFuture{ writePromise.get_future() };
         std::thread writeThread(
            static_cast<void(win32_file_handle::*)(
               count_promise&&,
               size_t,
               const std::byte*,
               size_t)>(&win32_file_handle::async_write),
            &h,
            std::move(writePromise),
            data.size(),
            data.data(),
            0 );

         auto writeCount = writeFuture.get();
         writeThread.join();
         Assert::AreEqual(data.size(), writeCount, 
                          L"Did not write the correct number of bytes.");
  
         std::vector<std::byte> readData{h.size()};
         count_promise readPromise;
         auto readFuture{ readPromise.get_future() };
         std::thread readThread(
            static_cast<void(win32_file_handle::*)(
               count_promise&&,
               size_t,
               std::byte*,
               size_t)>(&win32_file_handle::async_read),
            &h,
            std::move(readPromise),
            readData.size(),
            readData.data(),
            0);

         auto readCount = readFuture.get();
         readThread.join();
         Assert::AreEqual(data.size(), readCount,
                          L"Did not write the correct number of bytes.");

         for (size_t i = 0; i < readData.size(); i++)
         {
            Assert::AreEqual(data[i], readData[i], L"The bytes are not equal.");
         }
      }

      TEST_METHOD(FileSize)
      {
         std::wstring path{ installed_path() + L"/fileSize.txt"};
         if (file_exists(path))
         {
            delete_file(path);
         }

         win32_file_handle h{ path, file_open_modes::readwrite };
         Assert::AreEqual(static_cast<size_t>(0), h.size(), 
                          L"New file size should be 0.");

         std::random_device rd;
         std::mt19937 gen{ rd() };
         std::vector<std::byte> data{ 2048 };

         // Generate random bytes and store them in the vector
         std::generate_n(data.begin(),
                         data.size(),
                         [&]() { return static_cast<std::byte>(gen()); });

         h.write(data.size(), data.data(), 0);
         Assert::AreEqual(data.size(), h.size(),
                          L"Did not write the correct amount of data to the file.");
      }

      TEST_METHOD(MoveConstructor)
      {
         std::wstring path{ installed_path() + L"/move.txt"};
         if (file_exists(path))
         {
            delete_file(path);
         }

         win32_file_handle h{ path, file_open_modes::readwrite };
         auto moved = std::move(h);

         Assert::IsFalse(h.valid(), L"The old handle should not be valid.");
         Assert::IsTrue(moved.valid(), L"The moved handle should be valid.");
      }
   };
}