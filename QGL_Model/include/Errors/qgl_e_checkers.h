#pragma once
#include "include/qgl_model_include.h"
#include "QGLPlatform.h"

namespace qgl
{
   #ifdef WIN32
   
   inline std::string resultToString(result_t r)
   {
      char s_str[64] = {};
      sprintf_s(s_str, "result_t of 0x%08X", static_cast<unsigned long>(r));
      return std::string(s_str);
   }

   inline const auto& throw_last_error = winrt::throw_last_error;

   #else

   inline void throw_last_error()
   {
      throw errno;
   }

   #endif

   /*
    Checks if the pointer is valid and throws the last error if it is not.
    */
   template<typename T, T* InavlidValue = nullptr>
   inline void check_pointer(const T* const toCheck)
   {
      if (toCheck == InavlidValue)
      {
         throw_last_error();
      }
   }

   /*
    Throws an exception is the result is a failure.
    */
   inline const auto& check_result = winrt::check_hresult;
}