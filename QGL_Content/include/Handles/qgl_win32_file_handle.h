#pragma once
#include "include/qgl_content_include.h"
#include "include/qgl_file_helpers.h"

namespace qgl::content
{
   class win32_file_handle final
   {
      public:
      win32_file_handle(const std::wstring& path, file_open_modes m)
      {
         auto sa = make_default_security_attributes();
         auto params = make_default_open_file_params(sa);

         DWORD openMode = 0;
         DWORD createMode = 0;
         switch (m)
         {
            case file_open_modes::read:
            {
               openMode = GENERIC_READ;
               createMode = OPEN_EXISTING;
               break;
            }
            case file_open_modes::write:
            {
               openMode = GENERIC_WRITE;
               createMode = OPEN_ALWAYS;
               break;
            }
            case file_open_modes::readwrite:
            {
               openMode = GENERIC_READ | GENERIC_WRITE;
               createMode = OPEN_ALWAYS;
               break;
            }
         }

         m_hndl = CreateFile2FromAppW(path.c_str(),
                     openMode,
                     DEFAULT_SHARE_MODE,
                     createMode,
                     &params);

         if (m_hndl == INVALID_HANDLE_VALUE)
         {
            winrt::throw_last_error();
         }
      }

      /*
       Do not allow copying a file handle.
       */
      win32_file_handle(const win32_file_handle&) = delete;

      /*
       Move constructor.
       */
      win32_file_handle(win32_file_handle&& r) noexcept
      {
         attach(r.detach());
      }

      /*
       Destructor.
       */
      ~win32_file_handle() noexcept
      {
         close();
      }

      /*
       Move assign operator.
       */
      win32_file_handle& operator=(win32_file_handle&& r) noexcept
      {
         attach(r.detach());
         return *this;
      }

      void close() noexcept
      {
         if (valid())
         {
            CloseHandle(m_hndl);
            m_hndl = INVALID_HANDLE_VALUE;
         }
      }

      void read(size_t bytes, std::byte* buffer, size_t offset)
      {
         check_bytes_size(bytes);

         auto sec = make_default_security_attributes();
         auto overlapped = make_overlapped(offset);

         auto bytesToRead = static_cast<DWORD>(bytes);
         auto ret = read_file(bytesToRead, buffer, &overlapped);

         if (ret.first != TRUE || ret.second != bytesToRead)
         {
            auto lastError = GetLastError();
            if (lastError != ERROR_IO_PENDING)
            {
               winrt::throw_hresult(HRESULT_FROM_WIN32(lastError));
            }
         }
      }

      void async_read(count_promise&& promise,
                      size_t bytes, std::byte* buffer, size_t offset)
      {
         check_bytes_size(bytes);

         auto sec = make_default_security_attributes();
         auto overlapped = make_overlapped(offset);
         winrt::handle completeEvent{ CreateEvent(&sec, TRUE, FALSE, nullptr) };
         overlapped.hEvent = completeEvent.get();

         auto bytesToRead = static_cast<DWORD>(bytes);
         auto ret = read_file(bytesToRead, buffer, &overlapped);

         if (ret.first != TRUE || ret.second != bytesToRead)
         {
            auto error_code = GetLastError();
            if (error_code != ERROR_IO_PENDING)
            {
               throw std::system_error(
                  error_code, std::system_category(),
                  "Read file failed without pending.");
            }

            wait_for_result(std::forward<count_promise>(promise),
                            completeEvent.get(),
                            bytesToRead,
                            &overlapped);
         }
         else
         {
            // Else, the operation completed in time.
            promise.set_value(ret.second);
         }

         // Event is closed now.
      }

      void write(size_t bytes, const std::byte* buffer, size_t offset)
      {
         check_bytes_size(bytes);

         auto sec = make_default_security_attributes();
         auto overlapped = make_overlapped(offset);

         auto bytesToWrite = static_cast<DWORD>(bytes);
         auto ret = write_file(bytesToWrite, buffer, &overlapped);

         if (ret.first != TRUE || ret.second != bytesToWrite)
         {
            auto lastError = GetLastError();
            if (lastError != ERROR_IO_PENDING)
            {
               winrt::throw_hresult(HRESULT_FROM_WIN32(lastError));
            }
         }
      }

      void async_write(count_promise&& promise,
                       size_t bytes, const std::byte* buffer, size_t offset)
      {
         check_bytes_size(bytes);

         auto sec = make_default_security_attributes();
         auto overlapped = make_overlapped(offset);
         winrt::handle completeEvent{ CreateEvent(&sec, TRUE, FALSE, nullptr) };
         overlapped.hEvent = completeEvent.get();

         auto bytesToWrite = static_cast<DWORD>(bytes);
         auto ret = write_file(bytesToWrite, buffer, &overlapped);

         if (ret.first != TRUE || ret.second != bytesToWrite)
         {

            auto error_code = GetLastError();
            if (error_code != ERROR_IO_PENDING)
            {
               throw std::system_error(
                  error_code, std::system_category(),
                  "Write file failed without pending.");
            }

            wait_for_result(std::forward<count_promise>(promise),
                   completeEvent.get(),
                   bytesToWrite,
                   &overlapped);
         }
         else
         {
            // Else the operation completed in time.
            promise.set_value(ret.second);
         }

         // Event is closed now.
      }

      size_t size() const
      {
         //Get the file info and check the result.
         FILE_STANDARD_INFO fileInfo = {};
         auto res = GetFileInformationByHandleEx(
            m_hndl,
            FILE_INFO_BY_HANDLE_CLASS::FileStandardInfo,
            &fileInfo,
            sizeof(fileInfo));

         if (!res)
         {
            winrt::throw_last_error();
         }

         return static_cast<size_t>(fileInfo.EndOfFile.QuadPart);
      }

      bool valid() const noexcept
      {
         return m_hndl != INVALID_HANDLE_VALUE;
      }

      explicit operator bool() const noexcept
      {
         valid();
      }

      void attach(HANDLE handle) noexcept
      {
         close();
         m_hndl = handle;
      }

      HANDLE detach() noexcept
      {
         auto ret = m_hndl;
         m_hndl = INVALID_HANDLE_VALUE;
         return ret;
      }

      private:
      static constexpr DWORD DEFAULT_SHARE_MODE =
         FILE_SHARE_READ | FILE_SHARE_WRITE;

      OVERLAPPED make_overlapped(size_t offsetBytes) noexcept
      {
         OVERLAPPED ret = { 0 };
         ret.hEvent = nullptr;
         ret.Offset = offsetBytes & 0xFFFF'FFFF;
         ret.OffsetHigh = (offsetBytes >> 32) & 0xFFFF'FFFF;
         return ret;
      }

      SECURITY_ATTRIBUTES make_default_security_attributes() noexcept
      {
         SECURITY_ATTRIBUTES ret;
         ret.bInheritHandle = TRUE;
         ret.nLength = sizeof(SECURITY_ATTRIBUTES);
         ret.lpSecurityDescriptor = nullptr;
         return ret;
      }

      CREATEFILE2_EXTENDED_PARAMETERS make_default_open_file_params(
         SECURITY_ATTRIBUTES& attr) noexcept
      {
         CREATEFILE2_EXTENDED_PARAMETERS ret;
         ret.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
         ret.dwFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED;
         ret.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
         ret.dwSecurityQosFlags = SECURITY_ANONYMOUS;
         ret.hTemplateFile = nullptr;
         ret.lpSecurityAttributes = &attr;
         return ret;
      }

      void check_bytes_size(size_t bytes)
      {
         if (bytes >= MAXDWORD)
         {
            throw std::overflow_error{
               "Cannot read more than MAXDWORD bytes from a file." };
         }
      }

      std::pair<BOOL, DWORD> read_file(
         DWORD bytesToRead,
         std::byte* buffer,
         OVERLAPPED* overlapped_p)
      {
         std::pair<BOOL, DWORD> ret;
         ret.first = ReadFile(m_hndl, static_cast<void*>(buffer),
                              bytesToRead, &ret.second, overlapped_p);
         return ret;
      }

      std::pair<BOOL, DWORD> write_file(
         DWORD bytesToWrite,
         const std::byte* buffer,
         OVERLAPPED* overlapped_p)
      {
         std::pair<BOOL, DWORD> ret;
         ret.first = WriteFile(m_hndl, static_cast<const void*>(buffer),
                               bytesToWrite, &ret.second, overlapped_p);
         return ret;
      }

      void wait_for_result(
         count_promise&& promise,
         HANDLE waitHandle,
         DWORD expectedBytes,
         OVERLAPPED* overlapped_p)
      {
         auto waitResult = WaitForSingleObject(waitHandle, INFINITE);

         switch (waitResult)
         {
            case WAIT_OBJECT_0:
            {
               // Read operation completed
               check_overlapped(std::forward<count_promise>(promise),
                                expectedBytes,
                                overlapped_p);

               break;
            }
            case WAIT_FAILED:
            {
               auto error_code = GetLastError();
               promise.set_exception(std::make_exception_ptr(
                  std::system_error(error_code,
                                    std::system_category(),
                                    "Wait failed")));
               break;
            }
            case WAIT_TIMEOUT:
            {
               promise.set_exception(std::make_exception_ptr(
                  std::system_error(ERROR_TIMEOUT,
                                    std::system_category(),
                                    "Timed out")));
               break;
            }
            case WAIT_ABANDONED:
            {
               promise.set_exception(std::make_exception_ptr(
                  std::system_error(ERROR_ABANDONED_WAIT_0,
                                    std::system_category(),
                                    "Abandoned")));
               break;
            }
            default:
            {
               // https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject
               throw std::runtime_error{ "This should not happen." };
            }
         }
      }

      void check_overlapped(
         count_promise&& promise,
         DWORD expectedBytes,
         OVERLAPPED* overlapped_p)
      {
         DWORD bytesRead = 0;
         auto ret = GetOverlappedResult(m_hndl, overlapped_p, &bytesRead, TRUE);
         if (ret)
         {
            // Success

            // Check the correct number of bytes were processed.
            if (bytesRead != expectedBytes)
            {
               throw std::runtime_error{
                  "Did not process the expected number of bytes." };
            }

            // Everything is good.
            promise.set_value(static_cast<size_t>(bytesRead));
            return;
         }
         else
         {
            // Fail
            auto error_code = GetLastError();
            promise.set_exception(std::make_exception_ptr(
               std::system_error(error_code,
                                 std::system_category(),
                                 "Could not get the read result.")));
         }
      }

      HANDLE m_hndl = INVALID_HANDLE_VALUE;
   };
}