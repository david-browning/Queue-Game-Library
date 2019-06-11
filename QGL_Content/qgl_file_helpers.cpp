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

SECURITY_ATTRIBUTES make_default_security_attributes() noexcept
{
   SECURITY_ATTRIBUTES ret;
   ret.bInheritHandle = TRUE;
   ret.nLength = sizeof(SECURITY_ATTRIBUTES);
   ret.lpSecurityDescriptor = nullptr;
   return ret;
}

CREATEFILE2_EXTENDED_PARAMETERS make_default_open_file_params(
   SECURITY_ATTRIBUTES* attr_p) noexcept
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

HRESULT qgl::content::open_file_read(const wchar_t* filePath,
                                     file_handle* out_p) noexcept
{
   auto sa = make_default_security_attributes();
   auto params = make_default_open_file_params(&sa);
   auto h = CreateFile2(filePath,
                        GENERIC_READ,
                        DEFAULT_SHARE_MODE,
                        OPEN_EXISTING,
                        &params);
   if (h == INVALID_HANDLE_VALUE)
   {
      return HRESULT_FROM_WIN32(GetLastError());
   }

   out_p->attach(h);
   return S_OK;
}

HRESULT qgl::content::open_file_read(
   const winrt::Windows::Storage::StorageFile& f,
   file_handle* out_p) noexcept
{
   winrt::com_ptr<IStorageItemHandleAccess> handleAccess =
      f.try_as<IStorageItemHandleAccess>();
   if (!handleAccess)
   {
      return E_UNEXPECTED;
   }

   return handleAccess->Create(HANDLE_ACCESS_OPTIONS::HAO_READ,
                               HANDLE_SHARING_OPTIONS::HSO_SHARE_READ |
                               HANDLE_SHARING_OPTIONS::HSO_SHARE_WRITE,
                               HANDLE_OPTIONS::HO_OVERLAPPED |
                               HANDLE_OPTIONS::HO_SEQUENTIAL_SCAN,
                               nullptr,
                               out_p->put());
}

HRESULT qgl::content::open_file_write(const wchar_t* filePath,
                                      file_handle* out_p) noexcept
{
   auto sa = make_default_security_attributes();
   auto params = make_default_open_file_params(&sa);
   auto h = CreateFile2(filePath,
                        GENERIC_WRITE,
                        DEFAULT_SHARE_MODE,
                        CREATE_ALWAYS,
                        &params);

   if (h == INVALID_HANDLE_VALUE)
   {
      return HRESULT_FROM_WIN32(GetLastError());
   }

   out_p->attach(h);
   return S_OK;
}

HRESULT qgl::content::open_file_write(
   const winrt::Windows::Storage::StorageFile & f,
   file_handle* out_p) noexcept
{
   winrt::com_ptr<IStorageItemHandleAccess> handleAccess =
      f.try_as<IStorageItemHandleAccess>();
   if (!handleAccess)
   {
      return E_UNEXPECTED;
   }

   return handleAccess->Create(HANDLE_ACCESS_OPTIONS::HAO_READ |
                               HANDLE_ACCESS_OPTIONS::HAO_WRITE,
                               HANDLE_SHARING_OPTIONS::HSO_SHARE_WRITE,
                               HANDLE_OPTIONS::HO_OVERLAPPED |
                               HANDLE_OPTIONS::HO_SEQUENTIAL_SCAN,
                               nullptr,
                               out_p->put());
}

HRESULT qgl::content::open_file_readwrite(const wchar_t* filePath,
                                          file_handle* out_p) noexcept
{
   auto sa = make_default_security_attributes();
   auto params = make_default_open_file_params(&sa);
   auto h = CreateFile2(filePath,
                        GENERIC_READ | GENERIC_WRITE,
                        DEFAULT_SHARE_MODE,
                        OPEN_ALWAYS,
                        &params);

   if (h == INVALID_HANDLE_VALUE)
   {
      return HRESULT_FROM_WIN32(GetLastError());
   }

   out_p->attach(h);
   return S_OK;
}

HRESULT qgl::content::open_file_readwrite(
   const winrt::Windows::Storage::StorageFile& f,
   file_handle* out_p) noexcept
{
   winrt::com_ptr<IStorageItemHandleAccess> handleAccess =
      f.try_as<IStorageItemHandleAccess>();
   if (!handleAccess)
   {
      return E_UNEXPECTED;
   }

   return handleAccess->Create(HANDLE_ACCESS_OPTIONS::HAO_READ |
                               HANDLE_ACCESS_OPTIONS::HAO_WRITE,
                               HANDLE_SHARING_OPTIONS::HSO_SHARE_READ |
                               HANDLE_SHARING_OPTIONS::HSO_SHARE_WRITE,
                               HANDLE_OPTIONS::HO_OVERLAPPED |
                               HANDLE_OPTIONS::HO_SEQUENTIAL_SCAN,
                               nullptr,
                               out_p->put());
}

HRESULT qgl::content::truncate_file(const file_handle* hdnl_p) noexcept
{
   LARGE_INTEGER distance;
   distance.QuadPart = 0;
   auto ret = SetFilePointerEx(hdnl_p->get(), distance,
                               nullptr, FILE_BEGIN);
   if (!ret)
   {
      return E_UNEXPECTED;
   }

   ret = SetEndOfFile(hdnl_p->get());
   if (!ret)
   {
      return E_UNEXPECTED;
   }

   return S_OK;
}

HRESULT qgl::content::file_size(const file_handle* hdnl_p,
                                size_t* out_p) noexcept
{
   if (out_p == nullptr)
   {
      return E_INVALIDARG;
   }

   //Get the file info and check the result.
   FILE_STANDARD_INFO fileInfo = {};
   auto res = GetFileInformationByHandleEx(
      hdnl_p->get(),
      FILE_INFO_BY_HANDLE_CLASS::FileStandardInfo,
      &fileInfo,
      sizeof(fileInfo));

   if (!res)
   {
      return HRESULT_FROM_WIN32(GetLastError());
   }

   *out_p = static_cast<size_t>(fileInfo.EndOfFile.QuadPart);
   return S_OK;
}

HRESULT qgl::content::file_size(const winrt::Windows::Storage::StorageFile& f,
                                size_t* out_p) noexcept
{
   file_handle hndl;
   auto hr = open_file_read(f, &hndl);
   if (FAILED(hr))
   {
      return hr;
   }

   return file_size(&hndl, out_p);
}

bool qgl::content::dir_exists(const wchar_t* absPath) noexcept
{
   return attribute_exists<FILE_ATTRIBUTE_DIRECTORY>(absPath);
}

bool qgl::content::file_exists(const wchar_t* absPath) noexcept
{
   auto attr = GetFileAttributes(absPath);
   return attr != INVALID_FILE_ATTRIBUTES &&
      !(attr & FILE_ATTRIBUTE_DIRECTORY);
}
