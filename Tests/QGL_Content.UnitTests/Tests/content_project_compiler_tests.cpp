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

         CONTENT_METADATA_BUFFER entry2Meta(RESOURCE_TYPE_LIGHT,
                                            CONTENT_LOADER_ID_LIGHT,
                                            L"Entry2");

         winrt::hstring entry1Path(L"C:\\Int16.txt");
         winrt::hstring entry2Path(L"C:\\Light.txt");

         //Create a project
         {
            auto projectF = co_await root.CreateFileAsync(
               L"CompileProjectStorageFile.txt",
               CreationCollisionOption::ReplaceExisting);

            content_project project(projectF);

            project.emplace_back(entry1Meta, entry1Path);
            project.emplace_back(entry2Meta, entry2Path);
            project.flush();

            //Compile it
            auto contentF = co_await root.CreateFileAsync(
               L"CompiledContentProject.txt",
               CreationCollisionOption::ReplaceExisting);

            try
            {
               compile_content_project(project, contentF);
            }
            catch (winrt::hresult_error&)
            {
               Assert::Fail(L"Exception compiling project.");
            }
         }

         //Load the compiled file into a content_file
         auto contentF = co_await root.CreateFileAsync(
            L"CompiledContentProject.txt",
            CreationCollisionOption::OpenIfExists);
         content_file<false> file(contentF);

         //Verify the header metadata is correct.
         Assert::IsTrue(file.header().metadata() == projectMeta,
                        L"The metadata is not correct.");

         Assert::IsTrue(file.at(0).metadata() == entry1Meta,
                        L"Entry 1 meta is not correct.");

         Assert::IsTrue(file.at(1).metadata() == entry1Meta,
                        L"Entry 2 meta is not correct.");
      }
   };
}