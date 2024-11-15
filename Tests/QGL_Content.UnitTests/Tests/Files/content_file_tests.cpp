#include "pch.h"
#include "CppUnitTest.h"
#include <random>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   TEST_CLASS(ContentFileTests)
   {
      public:
      TEST_METHOD(NewFile)
      {
         win32_file_handle h{ installed_path() + L"/cotentNew.bin",
            file_open_modes::readwrite };
         content_file f{ std::move(h) };

      }

      TEST_METHOD(SaveFile)
      {
         win32_file_handle h{ installed_path() + L"/cotentSave.bin",
            file_open_modes::readwrite };
         content_file f{ std::move(h) };

         std::string metaName {"TestContent"};
         f.metadata().name = metaName.c_str();
         f.flush();
      }

      TEST_METHOD(ReadFile)
      {
         auto path = installed_path() + L"/cotentSave.bin";
         win32_file_handle h{ path, file_open_modes::readwrite };
         content_file fWrite{ std::move(h) };

         std::string metaName {"TestContent"};
         fWrite.metadata().name = metaName.c_str();
         fWrite.flush();

         win32_file_handle hread{ path, file_open_modes::read };
         content_file fRead{ std::move(hread) };
         std::string readName {fRead.metadata().name.data()};
         Assert::AreEqual(metaName, readName, L"Did not read the correct name.");
         Assert::AreEqual(static_cast<size_t>(0), fRead.size(),
                          L"Size should be 0.");
      }

      TEST_METHOD(CompressDictionary)
      {
         auto path = installed_path() + L"/compressDict.bin";
         win32_file_handle h{ path, file_open_modes::readwrite };
         content_file fWrite{ std::move(h) };
         
         std::string metaName {"TestContent"};
         fWrite.metadata().name = metaName.c_str();
         fWrite.metadata().compression_flags(
            compression::compression_flags::dictionary);
         fWrite.metadata().compression_type(
            compression::compression_types::xpress);

         std::random_device rd;
         std::mt19937 gen{ rd() };
         std::vector<std::byte> contentBuffer{ 2048 };
         std::generate_n(contentBuffer.begin(),
                         contentBuffer.size(),
                         [&]() { return static_cast<std::byte>(gen()); });

         qgl::descriptors::content_metadata contentMetadata;
         contentMetadata.name = "ContentName";
         contentMetadata.id = "4B515DF6B72C4FD8B097E50ED4089DBB";
         contentMetadata.loader = "97CF87EEE0A4427C9F914B2C010C13B1";

         fWrite.insert(contentMetadata, 
                       contentBuffer.data(), contentBuffer.size());
         Assert::AreEqual(static_cast<size_t>(1),
                          fWrite.size(),
                          L"There should be 1 dictionary entry before writing.");
         fWrite.flush();

         win32_file_handle hRead{ path, file_open_modes::read };
         content_file fRead{ std::move(hRead) };
         Assert::AreEqual(static_cast<size_t>(1),
                          fRead.size(),
                          L"There should be 1 entry after reading.");

         auto& firstEntry = *fRead.cbegin();
         Assert::IsTrue(firstEntry.second.metadata.id == contentMetadata.id,
                        L"Content IDs are not equal.");
         Assert::IsTrue(firstEntry.second.metadata.loader == contentMetadata.loader,
                        L"Loader IDs are not equal.");
      }

      TEST_METHOD(CompressContent)
      {

      }

      TEST_METHOD(CompressBoth)
      {

      }
   };
}