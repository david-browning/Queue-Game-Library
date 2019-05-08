#include "pch.h"
#include "include/qgl_file_helpers.h"
#include <winrt/Windows.Storage.AccessCache.h>

using namespace winrt::param;
using namespace winrt::Windows::Storage;

winrt::file_handle qgl::content::open_file_read(const winrt::hstring& filePath,
                                                CREATEFILE2_EXTENDED_PARAMETERS& params)
{
   auto h = winrt::file_handle(CreateFile2(filePath.c_str(),
                                           GENERIC_READ,
                                           FILE_SHARE_READ,
                                           OPEN_EXISTING,
                                           &params));
   if (!h)
   {
      winrt::throw_last_error();
   }

   return h;
}

winrt::file_handle qgl::content::open_file_write(const winrt::hstring& filePath,
                                                 CREATEFILE2_EXTENDED_PARAMETERS& params)
{
   auto h = winrt::file_handle(CreateFile2(filePath.c_str(),
                                           GENERIC_WRITE,
                                           0,
                                           CREATE_ALWAYS,
                                           &params));
   if (!h)
   {
      winrt::throw_last_error();
   }

   return h;
}

winrt::file_handle qgl::content::open_file_readwrite(const winrt::hstring& filePath,
                                                     CREATEFILE2_EXTENDED_PARAMETERS& params)
{
     auto h = winrt::file_handle(CreateFile2(filePath.c_str(),
                                         GENERIC_READ | GENERIC_WRITE,
                                         FILE_SHARE_READ,
                                         OPEN_ALWAYS,
                                         &params));

   if (!h)
   {
      auto hr = GetLastError();
      winrt::throw_last_error();
   }

   return h;
}

LIB_EXPORT void qgl::content::truncate_file(const winrt::file_handle& hdnl)
{
   LARGE_INTEGER distance;
   distance.QuadPart = 0;
   winrt::check_bool(SetFilePointerEx(hdnl.get(), distance, nullptr, FILE_BEGIN));
   winrt::check_bool(SetEndOfFile(hdnl.get()));
}

size_t qgl::content::file_size(const winrt::file_handle& hndl)
{
   //Get the file info and check the result.
   FILE_STANDARD_INFO fileInfo = {};
   auto res = GetFileInformationByHandleEx(hndl.get(),
                                           FILE_INFO_BY_HANDLE_CLASS::FileStandardInfo,
                                           &fileInfo,
                                           sizeof(fileInfo));
   if (!res)
   {
      winrt::throw_last_error();
   }

   return static_cast<size_t>(fileInfo.EndOfFile.QuadPart);
}

std::vector<uint8_t> qgl::content::file_data(const winrt::file_handle& hndl)
{
   auto fileSize = file_size(hndl);
   std::vector<uint8_t> ret(fileSize);
   read_file_sync(hndl, fileSize, 0, ret.data());
   return ret;
}
