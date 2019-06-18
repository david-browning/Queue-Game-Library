#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace winrt::Windows::Storage;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   TEST_CLASS(ContentProjectCompilerTests)
   {
      public:
      TEST_METHOD(CompileProjectStorageFile)
      {
         test_compile_project_storage_file();
      }

      TEST_METHOD(CompileProjectFile)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\CompileProjectFile.txt");
         DeleteFile(newFilePath.c_str());

         winrt::hstring compiledPath(root + L"\\CompiledProjectFile.txt");
         DeleteFile(compiledPath.c_str());

         CONTENT_METADATA_BUFFER entry1Meta(RESOURCE_TYPE_INTEGER,
                                            CONTENT_LOADER_ID_INT16,
                                            L"Entry1");
         winrt::hstring entry1Path(root + L"\\Int16.txt");
         DeleteFile(entry1Path.c_str());
         file_handle int16Handle;
         open_file_write(entry1Path.c_str(), &int16Handle);
         int16_t int16Data = int16_t(0xFF'AA);
         write_file_sync(&int16Handle, sizeof(int16Data), 0, &int16Data);
         int16Handle.close();

         //Create a project.
         {
            icontent_project* project = nullptr;
            winrt::check_hresult(qgl_open_content_project(
               newFilePath.c_str(),
               qgl::QGL_VERSION_0_1_WIN,
               &project));
            winrt::check_hresult(project->emplace_data_back(
               &entry1Meta,
               entry1Path.c_str()));
            winrt::check_hresult(project->flush());

            //Compile the project file.
            winrt::check_hresult(compile_content_project(project,
                                                         compiledPath.c_str()));

         }

         //Open the compiled project file.
         icontent_file* compiledFile = nullptr;
         winrt::check_hresult(qgl_open_content_file(compiledPath.c_str(),
                                                    qgl::QGL_VERSION_0_1_WIN,
                                                    &compiledFile));

         for (size_t i = 0; i < compiledFile->size(); i++)
         {
            const auto dictEntry = compiledFile->at(i);
            Assert::IsTrue(*dictEntry->metadata() == entry1Meta,
                           L"The entry metadata is not correct.");
         }

         compiledFile->release();
      }

      private:
      winrt::fire_and_forget test_compile_project_storage_file()
      {
         auto root = ApplicationData::Current().LocalFolder();


         CONTENT_METADATA_BUFFER projectMeta(RESOURCE_TYPE_BRUSH,
                                             CONTENT_LOADER_ID_BRUSH,
                                             L"Brush");

         CONTENT_METADATA_BUFFER entry1Meta(RESOURCE_TYPE_INTEGER,
                                            CONTENT_LOADER_ID_INT16,
                                            L"Entry1");

         winrt::hstring entry1Path(root.Path() + L"\\StorageFileInt16.txt");
         DeleteFile(entry1Path.c_str());
         file_handle int16Handle;
         open_file_write(entry1Path.c_str(), &int16Handle);
         int16_t int16Data = int16_t(0xFF'AA);
         write_file_sync(&int16Handle, sizeof(int16Data), 0, &int16Data);
         int16Handle.close();

         //Create a project
         {
            auto projectF = co_await root.CreateFileAsync(
               L"CompileProjectStorageFile.txt",
               CreationCollisionOption::ReplaceExisting);

            icontent_project* project = nullptr;
            winrt::check_hresult(qgl_open_content_project_sf(
               projectF,
               qgl::QGL_VERSION_0_1_WIN,
               &project));
            *project->metadata() = projectMeta;
            winrt::check_hresult(project->emplace_data_back(
               &entry1Meta,
               entry1Path.c_str()));
            winrt::check_hresult(project->emplace_data_back(
               &entry1Meta,
               entry1Path.c_str()));
            project->flush();

            //Compile it
            auto contentF = co_await root.CreateFileAsync(
               L"CompiledContentProject.txt",
               CreationCollisionOption::ReplaceExisting);

            winrt::check_hresult(compile_content_project_sf(project,
                                                            contentF));
         }

         //Load the compiled file into a content_file
         auto contentF = co_await root.CreateFileAsync(
            L"CompiledContentProject.txt",
            CreationCollisionOption::OpenIfExists);
         icontent_file* file = nullptr;
         winrt::check_hresult(qgl_open_content_file_sf(contentF,
                                                       qgl::QGL_VERSION_0_1_WIN,
                                                       &file));

         //Verify the header metadata is correct.
         Assert::IsTrue(*file->header()->metadata() == projectMeta,
                        L"The metadata is not correct.");

         for (size_t i = 0; i < file->size(); i++)
         {
            Assert::IsTrue(*file->at(i)->metadata() == entry1Meta,
                           L"Entry 1 meta is not correct.");
         }

         file->release();
      }
   };
}