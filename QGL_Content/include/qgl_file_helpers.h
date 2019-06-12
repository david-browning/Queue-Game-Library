#pragma once
#include "include/qgl_content_include.h"
#include "include/qgl_file_handle.h"
#include <winrt/Windows.Storage.h>

namespace qgl::content
{
   inline void make_overlapped(size_t offsetBytes, OVERLAPPED* over_p)
   {
      ZeroMemory(over_p, sizeof(OVERLAPPED));
      over_p->hEvent = nullptr;
      over_p->Offset = offsetBytes & 0xFFFF'FFFF;
      over_p->OffsetHigh = (offsetBytes >> 32) & 0xFFFF'FFFF;
   }

   /*
    Reads bytesToRead bytes from a file and stores it in buffer_p.
    Specify the offset into the file using offsetBytes.
    File handle must be open with read permissions.
    Returns a failed HRESULT on error.
    */
   template<typename T>
   HRESULT read_file_sync(const file_handle* hdnl_p,
                          size_t bytesToRead,
                          size_t offsetBytes,
                          T* const buffer_p) noexcept
   {
      DWORD bytesRead = 0;
      OVERLAPPED overlapped;
      make_overlapped(offsetBytes, &overlapped);

      auto result = ReadFile(hdnl_p->get(), (void*)buffer_p,
         (DWORD)bytesToRead, &bytesRead, &overlapped);
      if (result != TRUE || bytesRead != bytesToRead)
      {
         auto lastError = GetLastError();
         if (lastError != ERROR_IO_PENDING)
         {
            return HRESULT_FROM_WIN32(lastError);
         }
      }

      return S_OK;
   }

   /*
    Writes bytesToWrite from buffer_p to the file.
    Specify the offset into the file using offsetBytes.
    File handle must be open with write permissions.
    Returns a failed HRESULT on error.
    */
   template<typename T>
   HRESULT write_file_sync(const file_handle* hdnl_p,
                           size_t bytesToWrite,
                           size_t offsetBytes,
                           const T* const buffer_p) noexcept
   {
      DWORD bytesWritten = 0;
      OVERLAPPED overlapped;
      make_overlapped(offsetBytes, &overlapped);

      auto result = WriteFile(hdnl_p->get(), (void*)buffer_p,
         (DWORD)bytesToWrite, &bytesWritten, &overlapped);
      if (result != TRUE || bytesWritten != bytesToWrite)
      {
         auto lastError = GetLastError();
         if (lastError != ERROR_IO_PENDING)
         {
            return HRESULT_FROM_WIN32(lastError);
         }
      }

      return S_OK;
   }

   /*
    Opens a file for read access.
    Creates the file if it does not exist.
    Returns a failed HRESULT on error.
    */
   extern "C" QGL_CONTENT_API HRESULT QGL_API_CC open_file_read(
      const wchar_t* filePath,
      file_handle* out_p) noexcept;

   /*
    Opens a storage file for read access.
    Returns E_UNEXPECTED on error.
    */
   extern QGL_CONTENT_API HRESULT QGL_API_CC open_file_read_sf(
      const winrt::Windows::Storage::StorageFile& f,
      file_handle* out_p) noexcept;

  /*
   Opens a file for write access.
   Creates the file if it does not exist.
   Returns a failed HRESULT on error.
   */
   extern "C" QGL_CONTENT_API HRESULT QGL_API_CC open_file_write(
      const wchar_t* filePath,
      file_handle* out_p) noexcept;

   /*
    Opens a file for write access.
    Returns E_UNEXPECTED on error.
    */
   extern QGL_CONTENT_API HRESULT QGL_API_CC open_file_write_sf(
      const winrt::Windows::Storage::StorageFile& f,
      file_handle* out_p) noexcept;

  /*
   Opens a file for read and write access.
   Creates the file if it does not exist.
   Returns a failed HRESULT on error.
   */
   extern "C" QGL_CONTENT_API HRESULT QGL_API_CC open_file_readwrite(
      const wchar_t* filePath,
      file_handle* out_p) noexcept;

   /*
    Opens a storage file for read and write access.
    TODO: Don't know if the file is valid after the storage file goes out of
    scope.
    Returns E_UNEXPECTED on error.
    */
   extern QGL_CONTENT_API HRESULT QGL_API_CC open_file_readwrite_sf(
      const winrt::Windows::Storage::StorageFile& f,
      file_handle* out_p) noexcept;

  /*
   Sets a handle's file pointer to zero and marks it as the end of the file.
   This truncates the file to 0 bytes.
   The file handle must have been opened with write access.
   Returns E_UNEXPECTED on error.
   */
   extern "C" QGL_CONTENT_API HRESULT QGL_API_CC truncate_file(
      const file_handle* hdnl_p) noexcept;

   /*
   Returns how many bytes large the file is.
   The file handle must have been opened with read permissions.
   Returns a failed HRESULT on error.
   */
   extern "C" QGL_CONTENT_API HRESULT QGL_API_CC file_size(
      const file_handle* hdnl_p,
      size_t* out_p) noexcept;

   /*
    Returns the size in bytes of the file.
    Returns a failed HRESULT on error.
    */
   extern QGL_CONTENT_API HRESULT QGL_API_CC file_size_sf(
      const winrt::Windows::Storage::StorageFile& f,
      size_t* out_p) noexcept;

  /*
   Returns true if a file or folder contains the given attribute.
   The path must be absolute.
   Attribute: Can be any attribute described here:
   https://docs.microsoft.com/en-us/windows/desktop/FileIO/file-attribute-constants
   */
   template<DWORD Attribute>
   bool attribute_exists(const wchar_t* absPath) noexcept
   {
      auto attr = GetFileAttributes(absPath);
      if (attr != INVALID_FILE_ATTRIBUTES)
      {
         return attr & Attribute;
      }
      else
      {
         return false;
      }
   }

   /*
    Returns true if the directory exists.
    Returns false if the directory does not exist, or the path does not point
    to a directory.
    The directory path must be absolute.
    */
   extern "C" QGL_CONTENT_API bool QGL_API_CC dir_exists(
      const wchar_t* absPath) noexcept;

   /*
    Returns true if the file exists.
    Returns false if the file does not exist or the path does not point to a
    file.
    The file path must be an absolute path.
    */
   extern "C" QGL_CONTENT_API bool QGL_API_CC file_exists(
      const wchar_t* absPath) noexcept;
}