#pragma once
#include "include/qgl_model_include.h"
#include "include/Errors/qgl_e_codes.h"

namespace qgl
{
   #ifdef WIN32
   
   inline std::string HrToString(HRESULT hr)
   {
      char s_str[64] = {};
      sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<unsigned long>(hr));
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
    Throws a not_implemented exception.
    */
   inline void throw_not_implemented()
   {
      throw not_implemented();
   }
}