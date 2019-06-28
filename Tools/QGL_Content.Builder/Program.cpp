#include "pch.h"

// You should also change the Alias value in the AppExecutionAlias Extension
// in the package.appxmanifest to a value that you define. To edit this file
// manually, open it with the XML Editor.

using namespace winrt;
using namespace winrt::Windows::Storage;

int main()
{
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
   DWORD numChars = GetCurrentDirectory(0, nullptr);
   if (numChars == 0)
   {
      winrt::throw_last_error();
   }

   std::wstring workingDir(numChars + 1, L'\0');
   numChars = GetCurrentDirectory(numChars + 1 * sizeof(wchar_t),
                                  workingDir.data());
   if (numChars == 0)
   {
      winrt::throw_last_error();
   }

   qgl::content::icontent_project* proj_p = nullptr;




   return 0;
}
