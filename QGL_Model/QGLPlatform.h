#pragma once

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define NOMINMAX 
#include <Windows.h>
#include <objbase.h>
#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.Threading.h>

namespace qgl
{
   typedef HANDLE handle_t;

   template<typename Handle = handle_t>
   constexpr Handle INVALID_HANDLE = INVALID_HANDLE_VALUE;

   typedef HANDLE thread_handle_t;

   typedef HANDLE file_handle_t;
   static const file_handle_t NO_FILE = INVALID_HANDLE_VALUE;

   typedef HRESULT result_t;

   static constexpr uint16_t FACILITY_QGL = 0x547;

   /*
    A "platform pointer" is a smart pointer that encapsulates a
    platform-specific interface.
    */
   template<typename T>
   using pptr = typename winrt::com_ptr<T>;

   using phandle = typename winrt::handle;

#ifdef UNICODE
   typedef wchar_t sys_char;

#else
   typedef char sys_char;
#endif

   using sys_str = typename std::basic_string<sys_char>;


   /*
    Creates an object that can be waited upon.
    */
   inline phandle make_waitable(
      bool manualReset = true, bool initialState = false)
   {
      handle_t h = CreateEvent(nullptr, manualReset, initialState, nullptr);
      if (!h)
      {
         winrt::throw_last_error();
      }

      return phandle{ h };
   }
}


#else

#include <unistd.h>
#include <uuid/uuid.h>

namespace qgl
{
   using handle_t = typename void*

      template<typename Handle = handle_t>
   constexpr Handle INVALID_HANDLE = static_cast<Handle>(-1);

   typedef void* thread_handle_t;

   typedef int file_handle_t;
   static constexpr file_handle_t NO_FILE = -1;

   typedef int result_t;

   typedef char sys_char;
   using sys_str = typename std::basic_string<sys_char>;

   inline int make_waitable(
      bool manualReset = true, bool initialState = false)
   {
      return 0;
   }
}
#endif

