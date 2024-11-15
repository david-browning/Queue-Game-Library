#pragma once
#include "QGLTraits.h"

namespace qgl::mem
{
   namespace impl
   {
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
      constexpr typename make_type<(n - 1) * 4>::type from_hex_str(
         const char s[n],
         size_t charIdx,
         typename make_type<(n - 1) * 4>::type cur)
      {
         static_assert((n - 1) % 2 == 0,
                       "There must be an odd number of characters in the string, "
                       "including the null terminator");

         static_assert(n <= (sizeof(uint64_t) * CHAR_BIT / 4) + 1,
                       "Cannot store a number larger than 8 bytes. "
                       "The string is too long.");

         static_assert(n > 2, "There must be at least 3 characters, "
                       "including the null terminator");

         using T = typename make_type<(n - 1) * 4>::type;
         auto tSize = sizeof(T) * CHAR_BIT;
         return charIdx == n - 1 ?
            cur >> (tSize - (4 * (n - 1))) : // The shift is to correct trailing zeros.
            from_hex_str<n>(s,
                        charIdx + 2,
                        cur |
                         ((T)from_hex(s[charIdx]) << (tSize - (4 * (charIdx + 1)))) |
                         ((T)from_hex(s[charIdx + 1]) << (tSize - (4 * (charIdx + 2))))
            );
      }
   }

   /*
    Converts a single hex character to an integer. Assume the character is
    [0-9] | [a-f] | [A-F]
    */
   constexpr uint8_t from_hex(char c)
   {
      return impl::from_hex(c);
   }

   /*
    Assume the string is null terminated.
    Assume the characters are [0-9] | [a-f] | [A-F]
    */
   template<size_t n>
   constexpr typename make_type<(n - 1) * 4>::type from_hex(const char s[n])
   {
      return qgl::mem::impl::from_hex_str<n>(s, 0, 0);
   }

   static_assert(from_hex('C') == 12,
                 "from_hex is not correct (c)");

   static_assert(from_hex('F') == 15,
                 "from_hex is not correct (F)");

   static_assert(from_hex('1') == 1,
                 "from_hex is not correct (1)");

   static_assert(from_hex('0') == 0,
                 "from_hex is not correct (0)");

   static_assert(from_hex<3>("00") == 0,
                 "from_hex is not correct (00)");

   static_assert(from_hex<3>("01") == 1,
                 "from_hex is not correct (01)");

   static_assert(from_hex<3>("0f") == 15,
                 "from_hex is not correct (0f)");

   static_assert(from_hex<3>("10") == 16,
                 "from_hex is not correct (10)");

   static_assert(from_hex<3>("1f") == 31,
                 "from_hex is not correct (1F)");

   static_assert(from_hex<5>("1f1f") == 0x1f1f,
                 "from_hex is not correct (1F1F)");

   static_assert(from_hex<7>("1f1f1f") == 0x1f1f1f,
                 "from_hex is not correct (1F1F1F)");

   static_assert(from_hex<9>("1f1f1f1f") == 0x1f1f1f1f,
                 "from_hex is not correct (1F1F1F1F)");

   static_assert(from_hex<11>("1f1f1f1f1f") == 0x1f1f1f1f1f,
                 "from_hex is not correct (1F1F1F1F)");

   static_assert(from_hex<17>("123456789abc0000") == 0x123456789abc0000,
                 "from_hex is not correct (123456789abc0000)");
}