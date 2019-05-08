#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content
{
   inline void fill_overlapped(size_t offsetBytes, OVERLAPPED* over_p)
   {
      ZeroMemory(over_p, sizeof(OVERLAPPED));
      over_p->hEvent = nullptr;
      over_p->Offset = offsetBytes & 0xFFFF'FFFF;
      over_p->OffsetHigh = (offsetBytes >> 32) & 0xFFFF'FFFF;
   }

   /*
    File handle must be open with read permissions.
    */
   template<typename T>
   void read_file_sync(const winrt::file_handle& fileHandle,
                       size_t bytesToRead,
                       size_t offsetBytes,
                       T* const buffer_p)
   {
      DWORD bytesRead = 0;
      OVERLAPPED overlapped;
      fill_overlapped(offsetBytes, &overlapped);

      auto result = ReadFile(fileHandle.get(), (void*)buffer_p, (DWORD)bytesToRead, &bytesRead, &overlapped);
      if (result != TRUE || bytesRead != bytesToRead)
      {
         auto lastError = GetLastError();
         if (lastError != ERROR_IO_PENDING)
         {
            winrt::check_win32(lastError);
         }
      }
   }

   template<typename T>
   void write_file_sync(const winrt::file_handle& fileHandle,
                        size_t bytesToWrite,
                        size_t offsetBytes,
                        const T* const buffer_p)
   {
      DWORD bytesWritten = 0;
      OVERLAPPED overlapped;
      fill_overlapped(offsetBytes, &overlapped);

      auto result = WriteFile(fileHandle.get(), (void*)buffer_p, (DWORD)bytesToWrite, &bytesWritten, &overlapped);
      if (result != TRUE || bytesWritten != bytesToWrite)
      {
         auto lastError = GetLastError();
         if (lastError != ERROR_IO_PENDING)
         {
            winrt::check_win32(lastError);
         }
      }
   }

   extern LIB_EXPORT winrt::file_handle open_file_read(const winrt::hstring& filePath,
                                                       CREATEFILE2_EXTENDED_PARAMETERS& params);

   extern LIB_EXPORT winrt::file_handle open_file_write(const winrt::hstring& filePath,
                                                        CREATEFILE2_EXTENDED_PARAMETERS& params);

   extern LIB_EXPORT winrt::file_handle open_file_readwrite(const winrt::hstring& filePath,
                                                            CREATEFILE2_EXTENDED_PARAMETERS& params);

   extern LIB_EXPORT void truncate_file(const winrt::file_handle& hdnl);

   /*
   Returns how many bytes large the file is.
   The file handle must have been opened with read permissions.
   */
   extern LIB_EXPORT size_t file_size(const winrt::file_handle& hndl);

   /*
    Loads the entire file into memory
    Returns a vector of bytes.
    */
   extern LIB_EXPORT std::vector<uint8_t> file_data(const winrt::file_handle& hndl);

   template<DWORD Attribute>
   bool attribute_exists(const winrt::hstring& absPath)
   {
      auto attr = GetFileAttributes(absPath.c_str());
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
    If ThrowOnError is true, this throws the last error if getting the attributes fails.
    */
   inline bool dir_exists(const winrt::hstring& absPath)
   {
      return attribute_exists<FILE_ATTRIBUTE_DIRECTORY>(absPath);
   }

   inline bool file_exists(const winrt::hstring& absPath)
   {
      return attribute_exists<FILE_ATTRIBUTE_NORMAL>(absPath);
   }

}