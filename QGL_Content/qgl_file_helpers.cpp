#include "pch.h"
#include "include/qgl_file_helpers.h"
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.AccessCache.h>

typedef DWORD SHGDNF;
#include <WindowsStorageCOM.h>

using namespace winrt::param;
using namespace winrt::Windows::Storage;
using namespace qgl::content;

static constexpr DWORD DEFAULT_SHARE_MODE = FILE_SHARE_READ | FILE_SHARE_WRITE;


void qgl::content::make_overlapped(size_t offsetBytes, OVERLAPPED* over_p)
{
   ZeroMemory(over_p, sizeof(OVERLAPPED));
   over_p->hEvent = nullptr;
   over_p->Offset = offsetBytes & 0xFFFF'FFFF;
   over_p->OffsetHigh = (offsetBytes >> 32) & 0xFFFF'FFFF;
}

SECURITY_ATTRIBUTES make_default_security_attributes()
{
   SECURITY_ATTRIBUTES ret;
   ret.bInheritHandle = TRUE;
   ret.nLength = sizeof(SECURITY_ATTRIBUTES);
   ret.lpSecurityDescriptor = nullptr;
   return ret;
}

CREATEFILE2_EXTENDED_PARAMETERS make_default_open_file_params(
   SECURITY_ATTRIBUTES * attr_p)
{
   CREATEFILE2_EXTENDED_PARAMETERS ret;
   ret.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
   ret.dwFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED;
   ret.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
   ret.dwSecurityQosFlags = SECURITY_ANONYMOUS;
   ret.hTemplateFile = nullptr;
   ret.lpSecurityAttributes = attr_p;
   return ret;
}

file_handle qgl::content::open_file_read(const winrt::hstring& filePath)
{
   auto sa = make_default_security_attributes();
   auto params = make_default_open_file_params(&sa);
   auto h = file_handle(CreateFile2(filePath.c_str(),
                        GENERIC_READ,
                        DEFAULT_SHARE_MODE,
                        OPEN_EXISTING,
                        &params));
   if (!h)
   {
      winrt::throw_last_error();
   }

   return h;
}

file_handle qgl::content::open_file_read(
   const winrt::Windows::Storage::StorageFile & f)
{
   winrt::com_ptr<IStorageItemHandleAccess> handleAccess =
      f.as<IStorageItemHandleAccess>();

   HANDLE hndl;
   auto hr = handleAccess->Create(HANDLE_ACCESS_OPTIONS::HAO_READ,
                                  HANDLE_SHARING_OPTIONS::HSO_SHARE_READ |
                                  HANDLE_SHARING_OPTIONS::HSO_SHARE_WRITE,
                                  HANDLE_OPTIONS::HO_OVERLAPPED |
                                  HANDLE_OPTIONS::HO_SEQUENTIAL_SCAN,
                                  nullptr,
                                  &hndl);

   winrt::check_hresult(hr);

   return file_handle(hndl);
}

file_handle qgl::content::open_file_write(const winrt::hstring& filePath)
{
   auto sa = make_default_security_attributes();
   auto params = make_default_open_file_params(&sa);
   auto hndl = CreateFile2(filePath.c_str(),
                           GENERIC_WRITE,
                           DEFAULT_SHARE_MODE,
                           CREATE_ALWAYS,
                           &params);
   auto h = file_handle(hndl);
   if (!h)
   {
      winrt::throw_last_error();
   }

   return h;
}

file_handle qgl::content::open_file_write(
   const winrt::Windows::Storage::StorageFile & f)
{
   winrt::com_ptr<IStorageItemHandleAccess> handleAccess =
      f.as<IStorageItemHandleAccess>();

   HANDLE hndl;
   auto hr = handleAccess->Create(HANDLE_ACCESS_OPTIONS::HAO_READ |
                                  HANDLE_ACCESS_OPTIONS::HAO_WRITE,
                                  HANDLE_SHARING_OPTIONS::HSO_SHARE_WRITE,
                                  HANDLE_OPTIONS::HO_OVERLAPPED |
                                  HANDLE_OPTIONS::HO_SEQUENTIAL_SCAN,
                                  nullptr,
                                  &hndl);

   winrt::check_hresult(hr);

   return file_handle(hndl);
}

file_handle qgl::content::open_file_readwrite(const winrt::hstring& filePath)
{
   auto sa = make_default_security_attributes();
   auto params = make_default_open_file_params(&sa);
   auto h = file_handle(CreateFile2(filePath.c_str(),
                                    GENERIC_READ | GENERIC_WRITE,
                                    DEFAULT_SHARE_MODE,
                                    OPEN_ALWAYS,
                                    &params));

   if (!h)
   {
      #ifdef DEBUG
      auto hr = GetLastError();
      (hr);
      #endif
      winrt::throw_last_error();
   }

   return h;
}

file_handle qgl::content::open_file_readwrite(
   const winrt::Windows::Storage::StorageFile& f)
{
   winrt::com_ptr<IStorageItemHandleAccess> handleAccess =
      f.as<IStorageItemHandleAccess>();

   HANDLE hndl;
   auto hr = handleAccess->Create(HANDLE_ACCESS_OPTIONS::HAO_READ |
                                  HANDLE_ACCESS_OPTIONS::HAO_WRITE,
                                  HANDLE_SHARING_OPTIONS::HSO_SHARE_READ |
                                  HANDLE_SHARING_OPTIONS::HSO_SHARE_WRITE,
                                  HANDLE_OPTIONS::HO_OVERLAPPED |
                                  HANDLE_OPTIONS::HO_SEQUENTIAL_SCAN,
                                  nullptr,
                                  &hndl);

   winrt::check_hresult(hr);

   return file_handle(hndl);
}

void qgl::content::truncate_file(const file_handle& hdnl)
{
   LARGE_INTEGER distance;
   distance.QuadPart = 0;
   winrt::check_bool(SetFilePointerEx(hdnl.get(), distance,
                                      nullptr, FILE_BEGIN));
   winrt::check_bool(SetEndOfFile(hdnl.get()));
}

size_t qgl::content::file_size(const file_handle& hndl)
{
   //Get the file info and check the result.
   FILE_STANDARD_INFO fileInfo = {};
   auto res = GetFileInformationByHandleEx(
      hndl.get(),
      FILE_INFO_BY_HANDLE_CLASS::FileStandardInfo,
      &fileInfo,
      sizeof(fileInfo));

   if (!res)
   {
      winrt::throw_last_error();
   }

   return static_cast<size_t>(fileInfo.EndOfFile.QuadPart);
}

LIB_EXPORT size_t qgl::content::file_size(
   const winrt::Windows::Storage::StorageFile & f)
{
   auto hndl = open_file_read(f);
   return file_size(hndl);
}

std::vector<uint8_t> qgl::content::file_data(const file_handle& hndl)
{
   auto fileSize = file_size(hndl);
   std::vector<uint8_t> ret(fileSize);
   read_file_sync(hndl, fileSize, 0, ret.data());
   return ret;
}
