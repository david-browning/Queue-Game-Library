#pragma once
#include "include/qgl_model_include.h"
#include "include/Memory/qgl_mem_traits.h"

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

   template<typename T>
   constexpr T mask(size_t start, size_t end, T cur) noexcept
   {
      return start > end ? 
         cur :
         mask(start + 1, 
              end, 
              cur | (static_cast<T>(1) << static_cast<T>(start)));
   }

   /*
    Converts a single hex character to an integer. Assume the character is
    [0-9] | [a-f] | [A-F]
    */
   constexpr uint8_t from_hex(char c)
   {
      c = c <= 'F' && c >= 'A' ?
         'a' + c - 'A' :
         c;

      return (c >= '0' && c <= '9') ?
         c - '0' :
         c - 'a' + 10;
   }

   /*
    Converts string of hex characters to an integer. Returns the smallest int
    necessary to store n number of hex characters where n includes the null
    terminator.
    For the first call to this recursive function, pass 0 for the charIdx and 
    cur.
    Assume the string is null terminated.
    Assume the characters are [0-9] | [a-f] | [A-F]
    */
   template<size_t n>
   constexpr typename mem::traits::make_type<(n - 1) * 4>::type from_hex(
      const char s[n],
      size_t charIdx,
      typename mem::traits::make_type<(n - 1) * 4>::type cur)
   {
      static_assert((n - 1) % 2 == 0,
                    "There must be an odd number of characters in the string, "
                    "including the null terminator");

      static_assert(n <= (sizeof(uint64_t) * CHAR_BIT / 4) + 1,
                    "Cannot store a number larger than 8 bytes. "
                    "The string is too long.");

      static_assert(n > 2, "There must be at least 3 characters, "
                    "including the null terminator");

      using T = typename qgl::mem::traits::make_type<(n - 1) * 4>::type;
      auto tSize = sizeof(T) * CHAR_BIT;
      return charIdx == n - 1 ?
         cur >> (tSize - (4 * (n - 1))) : // The shift is to correct trailing zeros.
         from_hex<n>(s, charIdx + 2,
                      cur |
                      ((T)from_hex(s[charIdx]) << (tSize - (4 * (charIdx + 1)))) |
                      ((T)from_hex(s[charIdx + 1]) << (tSize - (4 * (charIdx + 2))))
                     );
   }

   /*
    Checks if the idx'th bit is set and moves it to the 0'th bit position.
    This means if the bit is set, this returns 1. Otherwise, 0.
    */
   template<typename T>
   constexpr size_t set_and_move(T val, size_t idx)
   {
      return (val >> idx) & 1;
   }

   /*
    Calculates the number of bits set in "val". 
    bits: Number of bits in type T.
    curIdx: The current index while iterating through "val".
    */
   template<typename T>
   constexpr size_t bits_set(T val, size_t bits, size_t curIdx)
   {
      return curIdx >= bits ?
         0 :
         set_and_move(val, curIdx) + bits_set(val, bits, curIdx + 1);
   }
} 