#pragma once
#include "include/qgl_content_include.h"
#include "include/qgl_file_handle.h"
#include <winrt/Windows.Storage.h>

namespace qgl::content
{
   /*
    Populates an OVERLAPPED so it can be used to read and write a file using
    an offset.
    */
   extern QGL_CONTENT_API void make_overlapped(size_t offsetBytes,
                                               OVERLAPPED* over_p);

   /*
    Reads bytesToRead bytes from a file and stores it in buffer_p.
    Specify the offset into the file using offsetBytes.
    File handle must be open with read permissions.
    Throws an exception on error.
    */
   template<typename T>
   void read_file_sync(const file_handle* hdnl_p,
                       size_t bytesToRead,
                       size_t offsetBytes,
                       T* const buffer_p)
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
            winrt::check_win32(lastError);
         }
      }
   }

   /*
    Writes bytesToWrite from buffer_p to the file.
    Specify the offset into the file using offsetBytes.
    File handle must be open with write permissions.
    Throws an exception on error.
    */
   template<typename T>
   void write_file_sync(const file_handle* hdnl_p,
                        size_t bytesToWrite,
                        size_t offsetBytes,
                        const T* const buffer_p)
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
            winrt::check_win32(lastError);
         }
      }
   }

   /*
    Opens a file for read access.
    Creates the file if it does not exist.
    Throws an exception on error.
    */
   extern QGL_CONTENT_API void open_file_read(const wchar_t* filePath,
                                              file_handle* out_p);

   /*
    Opens a storage file for read access.
    Throws an exception on error
    */
   extern QGL_CONTENT_API void open_file_read(
      const winrt::Windows::Storage::StorageFile& f,
      file_handle* out_p);

  /*
   Opens a file for write access.
   Creates the file if it does not exist.
   Throws an exception on error.
   */
   extern QGL_CONTENT_API void open_file_write(const wchar_t* filePath,
                                               file_handle* out_p);

   /*
    Opens a file for write access.
    Throws an exception on error.
    */
   extern QGL_CONTENT_API void open_file_write(
      const winrt::Windows::Storage::StorageFile& f,
      file_handle* out_p);

  /*
   Opens a file for read and write access.
   Creates the file if it does not exist.
   Throws an exception on error.
   */
   extern QGL_CONTENT_API void open_file_readwrite(const wchar_t* filePath,
                                                   file_handle* out_p);

   /*
    Opens a storage file for read and write access.
    TODO: Don't know if the file is valid after the storage file goes out of
    scope.
    Throws an exception on error.
    */
   extern QGL_CONTENT_API void open_file_readwrite(
      const winrt::Windows::Storage::StorageFile& f,
      file_handle* out_p);

  /*
   Sets a handle's file pointer to zero and marks it as the end of the file.
   This truncates the file to 0 bytes.
   The file handle must have been opened with write access.
   Throws an exception on error.
   */
   extern QGL_CONTENT_API void truncate_file(const file_handle* hdnl_p);

   /*
   Returns how many bytes large the file is.
   The file handle must have been opened with read permissions.
   Throws an exception on error.
   */
   extern QGL_CONTENT_API size_t file_size(const file_handle* hdnl_p);

   /*
    Returns the size in bytes of the file.
    */
   extern QGL_CONTENT_API size_t file_size(
      const winrt::Windows::Storage::StorageFile& f);

   /*
    Loads the entire file into memory
    Returns a vector of bytes.
    */
   extern QGL_CONTENT_API std::vector<uint8_t> file_data(
      const file_handle* hdnl_p);

  /*
   Returns true if a file or folder contains the given attribute.
   The path must be absolute.
   Attribute: Can be any attribute described here:
   https://docs.microsoft.com/en-us/windows/desktop/FileIO/file-attribute-constants
   */
   template<DWORD Attribute>
   bool attribute_exists(const wchar_t* absPath)
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
   bool QGL_CONTENT_API dir_exists(const wchar_t* absPath);

   /*
    Returns true if the file exists.
    Returns false if the file does not exist or the path does not point to a
    file.
    The file path must be an absolute path.
    */
   bool QGL_CONTENT_API file_exists(const wchar_t* absPath);
}