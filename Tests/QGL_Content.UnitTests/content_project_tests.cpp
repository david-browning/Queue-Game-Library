#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace winrt::Windows::Storage;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   TEST_CLASS(ContentProjectTests)
   {
      TEST_METHOD(ConstructorNewProjectFile)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\ConstructorNewProjectFile.txt");

         content_project project(newFilePath);

         Assert::AreEqual(static_cast<size_t>(0),
                          project.size(),
                          L"There should be 0 project entries.");

         Assert::IsTrue(project.begin() == project.end(),
                        L"The iterators are not equal.");

         Assert::IsTrue(project.cbegin() == project.cend(),
                        L"The const iterators are not equal.");
      }

      /*
       Assume the new file constructor, flush, and entry accessors are correct.
       */
      TEST_METHOD(ConstructorExistingProjectFile)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root +
                                    L"\\ConstructorExistingProjectFile.txt");

         content_project projectWrite(newFilePath);
         CONTENT_METADATA_BUFFER projectMeta(RESOURCE_TYPE_CAMERA,
                                             CONTENT_LOADER_ID_CAMERA,
                                             L"ProjectName");
         projectWrite.metadata() == projectMeta;

         CONTENT_METADATA_BUFFER entryMeta(RESOURCE_TYPE_BRUSH,
                                           CONTENT_LOADER_ID_BRUSH,
                                           L"Brush");
         projectWrite.emplace_back(entryMeta, L"C:\\SomeFile.txt");

         projectWrite.flush();

         content_project projectRead(newFilePath);
         Assert::IsTrue(projectRead.metadata() == projectWrite.metadata(),
                        L"The project metadata is not equal.");

         for (size_t i = 0; i < projectRead.size(); i++)
         {
            Assert::IsTrue(projectRead[i].first == projectWrite[i].first,
                           L"The project entry metadata is not equal.");

            Assert::IsTrue(projectRead[i].second == projectWrite[i].second,
                           L"The entry paths are not equal.");
         }
      }

      TEST_METHOD(ContentProjectMoveConstructor)
      {

      }

      //TEST_METHOD(ContentProjectFlush)
      //{
      //   //Create a project.
      //   auto root = ApplicationData::Current().LocalFolder().Path();
      //   winrt::hstring newFilePath(root +
      //                              L"\\ContentProjectFlush.txt");

      //   content_project projectWrite(newFilePath);
      //   CONTENT_METADATA_BUFFER projectMeta(RESOURCE_TYPE_CAMERA,
      //                                       CONTENT_LOADER_ID_CAMERA,
      //                                       L"ProjectName");
      //   projectWrite.metadata() == projectMeta;

      //   CONTENT_METADATA_BUFFER entryMeta(RESOURCE_TYPE_BRUSH,
      //                                     CONTENT_LOADER_ID_BRUSH,
      //                                     L"Brush");
      //   projectWrite.emplace_back(entryMeta, L"C:\\SomeFile.txt");

      //   projectWrite.emplace_back(entryMeta, L"C:\\SomeFile2.txt");

      //   //Flush it
      //   projectWrite.flush();

      //   //Read it using tested APIs.
      //   auto hndl = open_file_read(newFilePath);

      //   //Read the magic number and check it.
      //   uint64_t readMagic = 0;
      //   read_file_sync(hndl, sizeof(readMagic), 0, &readMagic);
      //   Assert::AreEqual(QGL_CONTENT_PROJECT_MAGIC_NUMBER,
      //                    readMagic,
      //                    L"The magic numbers are not equal.");

      //   //Read the metadata and check it.
      //   size_t offset = sizeof(readMagic);
      //   CONTENT_METADATA_BUFFER readMetadata;
      //   read_file_sync(hndl, offset, sizeof(readMagic),
      //                  &readMetadata);
      //   Assert::IsTrue(projectMeta == readMetadata,
      //                  L"The project metadata is not correct.");

      //   //Read the number of entries and check it.
      //   offset += sizeof(readMetadata);
      //   uint64_t numEntries = 0;
      //   read_file_sync(hndl, sizeof(numEntries), offset, &numEntries);
      //   Assert::IsTrue(projectWrite.size() == numEntries,
      //                  L"The number of entries is not correct.");
      //   offset += sizeof(numEntries);

      //   //Read the entries and check them.
      //   for (uint64_t i = 0; i < numEntries; i++)
      //   {
      //      uint64_t magicSeperator;
      //      read_file_sync(hndl, sizeof(magicSeperator), offset,
      //                     &magicSeperator);
      //      Assert::AreEqual(QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER,
      //                       magicSeperator,
      //                       L"The separator value is not correct.");
      //      offset += sizeof(magicSeperator);

      //      CONTENT_METADATA_BUFFER readEntryMeta;
      //      read_file_sync(hndl, sizeof(readEntryMeta), offset,
      //                     &readEntryMeta);
      //      Assert::IsTrue(readEntryMeta == projectWrite.at(i).first,
      //                     L"Entry metadata is not correct.");
      //      offset += sizeof(readEntryMeta);

      //      uint64_t numChars = 0;
      //      read_file_sync(hndl, sizeof(numChars), offset,
      //                     &numChars);
      //      Assert::IsTrue(numChars == projectWrite.at(i).second.size(),
      //                     L"The number of characters is not correct.");
      //      offset += sizeof(numChars);

      //      std::wstring readPath;
      //      readPath.resize(numChars);
      //      read_file_sync(hndl, numChars * sizeof(wchar_t), offset,
      //                     readPath.data());
      //      Assert::AreEqual(projectWrite.at(i).second, readPath,
      //                       L"The file path is not correct.");
      //      offset += numChars * sizeof(wchar_t);
      //   }
      //}

      /*
       Assume emplace_back is correct.
       */
      TEST_METHOD(ContentProjectAt)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root +
                                    L"\\ContentProjectAt.txt");

         content_project projectWrite(newFilePath);
         CONTENT_METADATA_BUFFER entryMeta(RESOURCE_TYPE_BRUSH,
                                           CONTENT_LOADER_ID_BRUSH,
                                           L"Brush");
         projectWrite.emplace_back(entryMeta, L"C:\\SomeFile.txt");

         projectWrite.emplace_back(entryMeta, L"C:\\SomeFile2.txt");

         Assert::IsTrue(projectWrite.at(0).first == entryMeta,
                        L"The metadata is not correct for entry 0.");

         Assert::IsTrue(projectWrite.at(0).second == L"C:\\SomeFile.txt",
                        L"The path is not correct for entry 0");

         Assert::IsTrue(projectWrite.at(1).first == entryMeta,
                        L"The metadata is not correct for entry 1.");

         Assert::IsTrue(projectWrite.at(1).second == L"C:\\SomeFile2.txt",
                        L"The path is not correct for entry 1");

         CONTENT_METADATA_BUFFER newEntryMeta(RESOURCE_TYPE_BRUSH,
                                              CONTENT_LOADER_ID_BRUSH,
                                              L"NewEntry");
         auto newEntry = std::make_pair(newEntryMeta, 
                                        std::wstring(L"NewPath.txt"));
         projectWrite.at(1) = newEntry;

         Assert::IsTrue(projectWrite.at(1).first == newEntryMeta,
                        L"The new metadata for entry 1 is not correct.");
         Assert::IsTrue(projectWrite.at(1).second == L"NewPath.txt",
                        L"The new path for entry 1 is not correct.");
      }

      TEST_METHOD(ContentProjectIndexOperator)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root +
                                    L"\\ContentProjectAt.txt");

         content_project projectWrite(newFilePath);
         CONTENT_METADATA_BUFFER entryMeta(RESOURCE_TYPE_BRUSH,
                                           CONTENT_LOADER_ID_BRUSH,
                                           L"Brush");
         projectWrite.emplace_back(entryMeta, L"C:\\SomeFile.txt");

         projectWrite.emplace_back(entryMeta, L"C:\\SomeFile2.txt");

         Assert::IsTrue(projectWrite[0].first == entryMeta,
                        L"The metadata is not correct for entry 0.");

         Assert::IsTrue(projectWrite[0].second == L"C:\\SomeFile.txt",
                        L"The path is not correct for entry 0");

         Assert::IsTrue(projectWrite[1].first == entryMeta,
                        L"The metadata is not correct for entry 1.");

         Assert::IsTrue(projectWrite[1].second == L"C:\\SomeFile2.txt",
                        L"The path is not correct for entry 1");

         CONTENT_METADATA_BUFFER newEntryMeta(RESOURCE_TYPE_BRUSH,
                                              CONTENT_LOADER_ID_BRUSH,
                                              L"NewEntry");
         auto newEntry = std::make_pair(newEntryMeta,
                                        std::wstring(L"NewPath.txt"));
         projectWrite[1] = newEntry;

         Assert::IsTrue(projectWrite[1].first == newEntryMeta,
                        L"The new metadata for entry 1 is not correct.");
         Assert::IsTrue(projectWrite[1].second == L"NewPath.txt",
                        L"The new path for entry 1 is not correct.");
      }

      TEST_METHOD(ContentProjectSize)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root +
                                    L"\\ContentProjectSize.txt");

         content_project projectWrite(newFilePath);
         Assert::IsTrue(0 == projectWrite.size(),
                        L"The size should be 0.");

         projectWrite.emplace_back(CONTENT_METADATA_BUFFER(), L"0");
         Assert::IsTrue(1 == projectWrite.size(),
                        L"The size should be 1.");

         projectWrite.emplace_back(CONTENT_METADATA_BUFFER(), L"2");
         Assert::IsTrue(2 == projectWrite.size(),
                        L"The size should be 2.");
      }

      TEST_METHOD(ContentProjectEmplacing)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root +
                                    L"\\ContentProjectEmplacing.txt");

         content_project projectWrite(newFilePath);















      }

      /*
       Assume emplacing and index operator is correct.
       */
      TEST_METHOD(ContentProjectErasing)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root +
                                    L"\\ContentProjectErasing.txt");

         content_project projectWrite(newFilePath);












      }

      /*
       Assume emplacing is correct.
       */
      TEST_METHOD(ContentProjectIterators)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root +
                                    L"\\ContentProjectIterators.txt");

         content_project projectWrite(newFilePath);

         std::vector<CONTENT_METADATA_BUFFER> metaDatas = 
         {
            CONTENT_METADATA_BUFFER(),
            CONTENT_METADATA_BUFFER(),
            CONTENT_METADATA_BUFFER()
         };

         std::vector<winrt::hstring> paths = 
         {
            L"1",
            L"2",
            L"3"
         };

         for (size_t i = 0; i < metaDatas.size(); i++)
         {
            projectWrite.emplace_back(metaDatas[i],
                                      paths[i]);
         }

         size_t i = 0;
         for (const auto& entry : projectWrite)
         {
            Assert::IsTrue(entry.first == metaDatas[i],
                           L"Metadata is not correct.");

            Assert::IsTrue(entry.second == paths[i],
                           L"Path is not correct.");
            i++;
         }
      }
   };
}