#include "pch.h"
#include "XCopyBridge.h"

// You should also change the Alias value in the AppExecutionAlias Extension
// in the package.appxmanifest to a value that you define. To edit this file
// manually, open it with the XML Editor.

using namespace winrt;
using namespace winrt::Windows::Storage;


int __cdecl main()
{
   //Required by C++/WinRT.
   init_apartment();

   // You can get parsed command-line arguments from the CRT globals.
   wprintf(L"Parsed command-line arguments:\n");
   for (int i = 0; i < __argc; i++)
   {
      wprintf(L"__argv[%d] = %S\n", i, __argv[i]);
   }

   //Get the working directory.
   //Does not include null terminator.
   //https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-getcurrentdirectory
   auto numChars = GetCurrentDirectory(0, nullptr);
   if (numChars == 0)
   {
      winrt::throw_last_error();
   }

   std::wstring workingDir(numChars, L'\0');
   numChars = GetCurrentDirectory((numChars) * sizeof(wchar_t),
                                  workingDir.data());
   if (numChars == 0)
   {
      winrt::throw_last_error();
   }

   auto writableFolder = ApplicationData::Current().LocalFolder().Path();
   std::wcout << L"Project will be created here:" << std::endl;
   std::wcout << writableFolder.c_str() << std::endl;

   //TODO: Get the file name from the command line args.
   winrt::hstring fileName = L"newProj.qglproj";
   auto fullProjectPath = writableFolder + L'\\' + fileName;
   DeleteFile(fullProjectPath.c_str());
   
   //Create an empty project:
   qgl::content::icontent_project* proj_p = nullptr;
   auto hr = qgl_open_content_project(fullProjectPath.c_str(),
                                      qgl::QGL_VERSION_0_1_WIN,
                                      &proj_p);
   winrt::check_hresult(hr);




   //Main logic.


























   //Make sure this version of Windows supports full trust launching.
   if (!VerifyInvokeSupported())
   {
      return ERROR_BAD_ENVIRONMENT;
   }

   //Create the arguments for xcopy.
   //Need additional params like /y?
   std::wstringstream argsStream;
   argsStream << fullProjectPath.c_str() << L" " << workingDir;

   //Set the arguments for the xcopy launcher.
   SetXCopyParameters(argsStream.str());

   //Invoke xcopy.
   InvokeXCopy();

   //TODO: Get the result of xcopy.

   //TODO: If xcopy returned error, print a message and return the error code.
   getchar();
   return 0;
}
