#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content
{
    /*
     Populates an OVERLAPPED so it can be used to read and write a file using
     an offset.
     */
   void fill_overlapped(size_t offsetBytes, OVERLAPPED* over_p);

   /*
    Returns a SECURITY_ATTRIBUTES for use with reading and writing files.
    The default attributes allow file handles to be inherited.
    */
   SECURITY_ATTRIBUTES fill_security_attributes();

   /*
    Returns a CREATEFILE2_EXTENDED_PARAMETERS for use with reading and 
    writing files. It supports opening files for reading and writing.
    The SECURITY_ATTRIBUTES pointer must remain valid for the lifetime of the
    CREATEFILE2_EXTENDED_PARAMETERS.
    */
   CREATEFILE2_EXTENDED_PARAMETERS fill_createfile_extended_parameters(
      SECURITY_ATTRIBUTES* attr_p);

   /*
    Reads bytesToRead bytes from a file and stores it in buffer_p.
    Specify the offset into the file using offsetBytes.
    File handle must be open with read permissions.
    Throws an exception on error.
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

      auto result = ReadFile(fileHandle.get(), (void*)buffer_p,
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
   void write_file_sync(const winrt::file_handle& fileHandle,
                        size_t bytesToWrite,
                        size_t offsetBytes,
                        const T* const buffer_p)
   {
      DWORD bytesWritten = 0;
      OVERLAPPED overlapped;
      fill_overlapped(offsetBytes, &overlapped);

      auto result = WriteFile(fileHandle.get(), (void*)buffer_p,
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
    Throws an exception on error.
    */
   extern LIB_EXPORT winrt::file_handle open_file_read(
      const winrt::hstring& filePath,
      CREATEFILE2_EXTENDED_PARAMETERS& params);

  /*
   Opens a file for write access.
   Throws an exception on error.
   */
   extern LIB_EXPORT winrt::file_handle open_file_write(
      const winrt::hstring& filePath,
      CREATEFILE2_EXTENDED_PARAMETERS& params);

  /*
   Opens a file for read and write access.
   Throws an exception on error.
   */
   extern LIB_EXPORT winrt::file_handle open_file_readwrite(
      const winrt::hstring& filePath,
      CREATEFILE2_EXTENDED_PARAMETERS& params);

  /*
   Sets a handle's file pointer to zero and marks it as the end of the file.
   This truncates the file to 0 bytes.
   The file handle must have been opened with write access.
   Throws an exception on error.
   */
   extern LIB_EXPORT void truncate_file(const winrt::file_handle& hdnl);

   /*
   Returns how many bytes large the file is.
   The file handle must have been opened with read permissions.
   Throws an exception on error.
   */
   extern LIB_EXPORT size_t file_size(const winrt::file_handle& hndl);

   /*
    Loads the entire file into memory
    Returns a vector of bytes.
    */
   extern LIB_EXPORT std::vector<uint8_t> file_data(
      const winrt::file_handle& hndl);

  /*
   Returns true if a file or folder contains the given attribute.
   The path must be absolute.
   Attribute: Can be any attribute described here:
   https://docs.microsoft.com/en-us/windows/desktop/FileIO/file-attribute-constants
   */
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
    Returns false if the directory does not exist, or the path does not point 
    to a directory.
    The directory path must be absolute.
    */
   inline bool dir_exists(const winrt::hstring& absPath)
   {
      return attribute_exists<FILE_ATTRIBUTE_DIRECTORY>(absPath);
   }

   /*
    Returns true if the file exists.
    Returns false if the file does not exist or the path does not point to a
    file.
    The file path must be an absolute path.
    */
   inline bool file_exists(const winrt::hstring& absPath)
   {
      return attribute_exists<FILE_ATTRIBUTE_NORMAL>(absPath);
   }

}