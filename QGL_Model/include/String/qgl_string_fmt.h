#pragma once
#include "include/qgl_model_include.h"
#include "include/String/qgl_string.h"
#include <sstream>

namespace qgl
{
   /*
    Writes the null-terminated string to the out stream.
    Returns the stream.
    */
   template<class Stream>
   static Stream& toprintf(Stream& ostrm, const sys_char* fmt)
   {
      ostrm << fmt;
      return ostrm;
   }

   /*
    
    */
   template<class Stream, typename Param, typename... Params>
   static Stream& toprintf(Stream& ostrm,
                    const sys_char* fmt,
                    const Param& arg,
                    const Params&... args)
   {
      while (*fmt != TEXT('\0'))
      {
         const auto c = *fmt;
         if (c == TEXT('%'))
         {
            ostrm << arg;
            return toprintf<Stream>(ostrm, fmt + 1, args...);
         }

         ostrm << c;
         fmt++;
      }

      return ostrm;
   }

   static sys_str tsprintf(const sys_char* fmt)
   {
      return sys_str(fmt);
   }

   template<typename... Params>
   static sys_str tsprintf(const sys_char* fmt,
                    const Params&... args)
   {
      std::basic_stringstream<sys_char> ss;
      return toprintf<decltype(ss)>(ss, fmt, args...).str();
   }
}