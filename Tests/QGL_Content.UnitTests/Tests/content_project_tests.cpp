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
         DeleteFile(newFilePath.c_str());

         icontent_project* project = nullptr;
         winrt::check_hresult(qgl_open_content_project(
            newFilePath.c_str(),
            qgl::QGL_VERSION_0_1_WIN,
            &project));

         Assert::AreEqual(static_cast<size_t>(0),
                          project->size(),
                          L"There should be 0 project entries.");

         project->release();
      }

      /*
       Assume the new file constructor, flush, and entry accessors are correct.
       */
      TEST_METHOD(ConstructorExistingProjectFile)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root +
                                    L"\\ConstructorExistingProjectFile.txt");
         DeleteFile(newFilePath.c_str());

         icontent_project* projectWrite = nullptr;
         winrt::check_hresult(qgl_open_content_project(
            newFilePath.c_str(),
            qgl::QGL_VERSION_0_1_WIN,
            &projectWrite));
         CONTENT_METADATA_BUFFER projectMeta(RESOURCE_TYPE_CAMERA,
                                             CONTENT_LOADER_ID_CAMERA,
                                             L"ProjectName");
         (*projectWrite->metadata()) = projectMeta;

         CONTENT_METADATA_BUFFER entryMeta(RESOURCE_TYPE_BRUSH,
                                           CONTENT_LOADER_ID_BRUSH,
                                           L"Brush");
         winrt::check_hresult(projectWrite->push_data_entry(
            &entryMeta,
            L"C:\\SomeFile.txt"));
         winrt::check_hresult(projectWrite->push_data_entry(
            &entryMeta,
            L"C:\\SomeFile.txt"));

         projectWrite->flush();

         icontent_project* projectRead = nullptr;
         winrt::check_hresult(qgl_open_content_project(
            newFilePath.c_str(),
            qgl::QGL_VERSION_0_1_WIN,
            &projectRead));
         Assert::IsTrue(*projectRead->metadata() == *projectWrite->metadata(),
                        L"The project metadata is not equal.");

         for (size_t i = 0; i < projectRead->size(); i++)
         {
            Assert::IsTrue(*projectRead->at(i)->const_metadata() ==
                           *projectWrite->at(i)->const_metadata(),
                           L"The project entry metadata is not equal.");

            Assert::AreEqual(std::wstring(projectRead->at(i)->const_path()),
                             std::wstring(projectWrite->at(i)->const_path()),
                             L"The entry paths are not equal.");
         }
         projectRead->release();
         projectWrite->release();
      }

      TEST_METHOD(ContentProjectFlush)
      {
         //Create a project.
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root +
                                    L"\\ContentProjectFlush.txt");
         DeleteFile(newFilePath.c_str());

         icontent_project* projectWrite = nullptr;
         winrt::check_hresult(qgl_open_content_project(
            newFilePath.c_str(),
            qgl::QGL_VERSION_0_1_WIN,
            &projectWrite));
         CONTENT_METADATA_BUFFER projectMeta(RESOURCE_TYPE_CAMERA,
                                             CONTENT_LOADER_ID_CAMERA,
                                             L"ProjectName");
         *projectWrite->metadata() = projectMeta;

         CONTENT_METADATA_BUFFER entryMeta(RESOURCE_TYPE_BRUSH,
                                           CONTENT_LOADER_ID_BRUSH,
                                           L"Brush");
         winrt::check_hresult(projectWrite->push_data_entry(
            &entryMeta,
            L"C:\\SomeFile.txt"));
         winrt::check_hresult(projectWrite->push_data_entry(
            &entryMeta,
            L"C:\\SomeFile2.txt"));

         //Flush it
         projectWrite->flush();

         //Read it using tested APIs.
         file_handle hndl;
         open_file_read(newFilePath.c_str(), &hndl);

         //Read the magic number and check it.
         uint64_t readMagic = 0;
         read_file_sync(&hndl, sizeof(readMagic), 0, &readMagic);
         Assert::AreEqual(QGL_CONTENT_PROJECT_MAGIC_NUMBER,
                          readMagic,
                          L"The magic numbers are not equal.");

         //Read the metadata and check it.
         size_t offset = sizeof(readMagic);
         CONTENT_METADATA_BUFFER readMetadata;
         read_file_sync(&hndl, sizeof(readMetadata), offset,
                        &readMetadata);
         Assert::IsTrue(projectMeta == readMetadata,
                        L"The project metadata is not correct.");

         //Read the number of entries and check it.
         offset += sizeof(readMetadata);
         uint64_t numEntries = 0;
         read_file_sync(&hndl, sizeof(numEntries), offset, &numEntries);
         Assert::IsTrue(projectWrite->size() == numEntries,
                        L"The number of entries is not correct.");
         offset += sizeof(numEntries);

         //Read the entries and check them.
         for (uint64_t i = 0; i < numEntries; i++)
         {
            uint64_t magicSeperator;
            read_file_sync(&hndl, sizeof(magicSeperator), offset,
                           &magicSeperator);
            Assert::AreEqual(QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER,
                             magicSeperator,
                             L"The separator value is not correct.");
            offset += (sizeof(magicSeperator) * 2);

            CONTENT_METADATA_BUFFER readEntryMeta;
            read_file_sync(&hndl, sizeof(readEntryMeta), offset,
                           &readEntryMeta);
            Assert::IsTrue(
               readEntryMeta == *projectWrite->at(i)->const_metadata(),
               L"Entry metadata is not correct.");
            offset += sizeof(readEntryMeta);

            uint64_t numChars = 0;
            read_file_sync(&hndl, sizeof(numChars), offset,
                           &numChars);
            Assert::IsTrue(numChars == projectWrite->at(i)->path_size(),
                           L"The number of characters is not correct.");
            offset += sizeof(numChars);

            std::wstring readPath;
            readPath.resize(numChars);
            read_file_sync(&hndl, numChars * sizeof(wchar_t), offset,
                           readPath.data());
            Assert::AreEqual(std::wstring(projectWrite->at(i)->const_path()),
                             readPath,
                             L"The file path is not correct.");
            offset += numChars * sizeof(wchar_t);
         }

         projectWrite->release();
      }

      /*
       Assume emplace_back is correct.
       */
      TEST_METHOD(ContentProjectAt)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root +
                                    L"\\ContentProjectAt.txt");
         DeleteFile(newFilePath.c_str());

         icontent_project* projectWrite = nullptr;
         winrt::check_hresult(qgl_open_content_project(
            newFilePath.c_str(),
            qgl::QGL_VERSION_0_1_WIN,
            &projectWrite));
         CONTENT_METADATA_BUFFER entryMeta(RESOURCE_TYPE_BRUSH,
                                           CONTENT_LOADER_ID_BRUSH,
                                           L"Brush");
         winrt::check_hresult(projectWrite->push_data_entry(
            &entryMeta,
            L"C:\\SomeFile.txt"));
         winrt::check_hresult(projectWrite->push_data_entry(
            &entryMeta,
            L"C:\\SomeFile2.txt"));

         Assert::IsTrue(*projectWrite->at(0)->const_metadata() == entryMeta,
                        L"The metadata is not correct for entry 0.");

         Assert::AreEqual(std::wstring(projectWrite->at(0)->const_path()),
                          std::wstring(L"C:\\SomeFile.txt"),
                          L"The path is not correct for entry 0");

         Assert::IsTrue(*projectWrite->at(1)->const_metadata() == entryMeta,
                        L"The metadata is not correct for entry 1.");

         Assert::AreEqual(std::wstring(projectWrite->at(1)->const_path()),
                          std::wstring(L"C:\\SomeFile2.txt"),
                          L"The path is not correct for entry 1");

         CONTENT_METADATA_BUFFER newEntryMeta(RESOURCE_TYPE_BRUSH,
                                              CONTENT_LOADER_ID_BRUSH,
                                              L"NewEntry");
         auto newEntry = helpers::content_project_entry(&newEntryMeta,
                                                        L"NewPath.txt");
         *projectWrite->at(1) = newEntry;

         Assert::IsTrue(*projectWrite->at(1)->const_metadata() == newEntryMeta,
                        L"The new metadata for entry 1 is not correct.");
         Assert::AreEqual(std::wstring(projectWrite->at(1)->const_path()),
                          std::wstring(L"NewPath.txt"),
                          L"The new path for entry 1 is not correct.");
         projectWrite->release();
      }

      TEST_METHOD(ContentProjectSize)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root +
                                    L"\\ContentProjectSize.txt");
         DeleteFile(newFilePath.c_str());

         icontent_project* projectWrite = nullptr;
         winrt::check_hresult(qgl_open_content_project(
            newFilePath.c_str(),
            qgl::QGL_VERSION_0_1_WIN,
            &projectWrite));
         Assert::IsTrue(0 == projectWrite->size(),
                        L"The size should be 0.");

         CONTENT_METADATA_BUFFER def;
         winrt::check_hresult(projectWrite->push_data_entry(&def, L"0"));
         Assert::IsTrue(1 == projectWrite->size(),
                        L"The size should be 1.");

         winrt::check_hresult(projectWrite->push_data_entry(&def, L"2"));
         Assert::IsTrue(2 == projectWrite->size(),
                        L"The size should be 2.");
         projectWrite->release();
      }

      /*
       Assume emplacing and index operator is correct.
       */
      TEST_METHOD(ContentProjectErasing)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root +
                                    L"\\ContentProjectErasing.txt");
         DeleteFile(newFilePath.c_str());

         icontent_project* projectWrite = nullptr;
         winrt::check_hresult(qgl_open_content_project(
            newFilePath.c_str(),
            qgl::QGL_VERSION_0_1_WIN,
            &projectWrite));
         CONTENT_METADATA_BUFFER entry1;
         CONTENT_METADATA_BUFFER entry2;
         CONTENT_METADATA_BUFFER entry3;

         //Insert an item.
         winrt::check_hresult(projectWrite->push_data_entry(
            &entry1,
            L"Str0"));

         //Erase it.
         projectWrite->erase(0);

         //Verify size is 0
         Assert::AreEqual(static_cast<size_t>(0),
                          projectWrite->size(),
                          L"Removed only entry: Size should be 0");


         //Insert 3 items
         winrt::check_hresult(projectWrite->push_data_entry(
            &entry1,
            L"Str1"));
         winrt::check_hresult(projectWrite->push_data_entry(
            &entry2,
            L"Str2"));
         winrt::check_hresult(projectWrite->push_data_entry(
            &entry3,
            L"Str3"));

         //Verify size is 3.
         Assert::AreEqual(static_cast<size_t>(3),
                          projectWrite->size(),
                          L"There should be 3 entries.");

         //Remove middle item.
         projectWrite->erase(1);

         //Verify size is 2.
         Assert::AreEqual(static_cast<size_t>(2),
                          projectWrite->size(),
                          L"There should be 2 entries.");

         //Remove all items.
         projectWrite->clear();

         //Verify size is 0.
         Assert::AreEqual(static_cast<size_t>(0),
                          projectWrite->size(),
                          L"Removed range: size is not 0.");
         projectWrite->release();
      }

      /*
       Assume emplacing is correct.
       */
      TEST_METHOD(ContentProjectIterators)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root +
                                    L"\\ContentProjectIterators.txt");
         DeleteFile(newFilePath.c_str());

         icontent_project* projectWrite = nullptr;
         winrt::check_hresult(qgl_open_content_project(
            newFilePath.c_str(),
            qgl::QGL_VERSION_0_1_WIN,
            &projectWrite));

         std::vector<CONTENT_METADATA_BUFFER> metaDatas =
         {
            CONTENT_METADATA_BUFFER(),
            CONTENT_METADATA_BUFFER(),
            CONTENT_METADATA_BUFFER()
         };

         std::vector<std::wstring> paths =
         {
            L"1",
            L"2",
            L"3"
         };

         for (size_t i = 0; i < metaDatas.size(); i++)
         {
            winrt::check_hresult(projectWrite->push_data_entry(
               &metaDatas[i],
               paths[i].c_str()));
         }

         for (size_t i = 0; i < projectWrite->size(); i++)
         {
            auto entry = projectWrite->const_at(i);
            Assert::IsTrue(*entry->const_metadata() == metaDatas[i],
                           L"Metadata is not correct.");

            Assert::AreEqual(std::wstring(entry->const_path()),
                             paths[i],
                             L"Path is not correct.");
         }
         projectWrite->release();
      }

      TEST_METHOD(InsertSharedEntry)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root +
                                    L"\\ContentProjectInsertShared.txt");
         DeleteFile(newFilePath.c_str());

         icontent_project* projectWrite = nullptr;
         winrt::check_hresult(qgl_open_content_project(
            newFilePath.c_str(),
            qgl::QGL_VERSION_0_1_WIN,
            &projectWrite));

         CONTENT_METADATA_BUFFER meta1(RESOURCE_TYPE_BRUSH,
                                       CONTENT_LOADER_ID_BRUSH,
                                       L"Brush");
         auto goodStr = L"Q:Shared Path 1";

         //Test inserting a bad string.
         auto badStr = L"T:Shared Path 1";
         auto hr = projectWrite->push_shared_entry(&meta1,
                                                   badStr);
         Assert::AreEqual(E_INVALIDARG, hr, L"Should fail with E_INVALIDARG");

         hr = projectWrite->push_shared_entry(&meta1,
                                              goodStr);
         Assert::AreEqual(S_OK, hr, L"Inserting should have worked.");
         Assert::AreEqual(size_t(1), projectWrite->size(),
                          L"There should be 1 entry now.");
      }

      TEST_METHOD(InsertDataEntry)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root +
                                    L"\\ContentProjectInsertData.txt");
         DeleteFile(newFilePath.c_str());

         icontent_project* projectWrite = nullptr;
         winrt::check_hresult(qgl_open_content_project(
            newFilePath.c_str(),
            qgl::QGL_VERSION_0_1_WIN,
            &projectWrite));

         CONTENT_METADATA_BUFFER meta1(RESOURCE_TYPE_BRUSH,
                                       CONTENT_LOADER_ID_BRUSH,
                                       L"Brush");

         auto hr = projectWrite->push_data_entry(&meta1, L"Test");
         Assert::AreEqual(S_OK, hr, L"Inserting should have worked.");
         Assert::AreEqual(size_t(1), projectWrite->size(),
                          L"There should be 1 entry now.");
      }

      TEST_METHOD(StorageFileOpenFlushRead)
      {
         test_storage_file();
      }

      private:
      winrt::fire_and_forget test_storage_file()
      {
         auto root = ApplicationData::Current().LocalFolder();
         auto f = co_await root.CreateFileAsync(
            L"StorageFileOpenFlushRead.txt",
            CreationCollisionOption::ReplaceExisting);


         icontent_project* projectWrite = nullptr;
         winrt::check_hresult(qgl_open_content_project_sf(
            f,
            qgl::QGL_VERSION_0_1_WIN,
            &projectWrite));
         CONTENT_METADATA_BUFFER projectMeta(RESOURCE_TYPE_CAMERA,
                                             CONTENT_LOADER_ID_CAMERA,
                                             L"ProjectName");
         *projectWrite->metadata() = projectMeta;

         CONTENT_METADATA_BUFFER entryMeta(RESOURCE_TYPE_BRUSH,
                                           CONTENT_LOADER_ID_BRUSH,
                                           L"Brush");
         winrt::check_hresult(projectWrite->push_data_entry(
            &entryMeta,
            L"C:\\SomeFile.txt"));
         winrt::check_hresult(projectWrite->push_data_entry(
            &entryMeta,
            L"C:\\SomeFile2.txt"));

         //Flush it
         projectWrite->flush();

         icontent_project* projectRead = nullptr;
         winrt::check_hresult(qgl_open_content_project(
            f.Path().c_str(),
            qgl::QGL_VERSION_0_1_WIN,
            &projectRead));
         Assert::IsTrue(*projectRead->metadata() == *projectWrite->metadata(),
                        L"The project metadata is not equal.");

         for (size_t i = 0; i < projectRead->size(); i++)
         {
            Assert::IsTrue(*projectRead->at(i)->const_metadata() ==
                           *projectWrite->at(i)->const_metadata(),
                           L"The project entry metadata is not equal.");

            Assert::AreEqual(std::wstring(projectRead->at(i)->const_path()),
                             std::wstring(projectWrite->at(i)->const_path()),
                             L"The entry paths are not equal.");
         }

         projectWrite->release();
         projectRead->release();
      }
   };
}