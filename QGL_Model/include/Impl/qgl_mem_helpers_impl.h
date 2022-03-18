#pragma once
#include "include/qgl_model_include.h"

namespace qgl::impl
{
   /*
    Compile-time endianness swap based on http://stackoverflow.com/a/36937049
    From https://en.cppreference.com/w/cpp/language/fold
    */
   template<class T, std::size_t... N>
   constexpr T bswap(T i, std::index_sequence<N...>)
   {
      return (((i >> N * CHAR_BIT & std::uint8_t(-1)) <<
         (sizeof(T) - 1 - N) * CHAR_BIT) | ...);
   }
}