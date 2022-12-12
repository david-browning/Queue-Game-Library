#pragma once
#include "include/qgl_model_include.h"

namespace qgl::mem::traits
{
   /*
   Returns val rounded to the nearest.
   Returns val if it is already rounded to the nearest.
   Zero cannot be rounded up.
   */
   template<typename T>
   constexpr T rup(T val, T nearest) noexcept
   {
      T remainder = val % nearest;
      return remainder == 0 ? val : val + nearest - remainder;
   }

   /*
    Returns the index of the first most significant bit.
    The number of leading zeros will depend on the type "T".
    */
   template<typename T>
   constexpr size_t msb(T val) noexcept
   {
      size_t ret = (sizeof(T) * CHAR_BIT) - 1;
      auto mask = static_cast<T>(1) << ret;
      while (ret > 0 && (val & mask) == 0)
      {
         ret--;
         mask = static_cast<T>(1) << ret;
      }

      return ret;
   }

   /*
   Calculates the smallest datatype needed to store Val.
   */
   template<size_t Val>
   struct bit_round
   {
      static constexpr auto value = rup(msb(Val) + 1, static_cast<size_t>(8));
   };

   static_assert(bit_round<100>::value == 8,
              "Bit rounding is not correct (100).");
   static_assert(bit_round<65535>::value == 16,
                 "Bit rounding is not correct. (65535)");

   /*
    Rounds Bits up to the nearest 8th.
    */
   template<size_t Bits>
   struct bit_size
   {
      static constexpr auto value = rup(Bits, static_cast<size_t>(8));
   };

   /*
    Default make_type for signed type.
    */
   template<size_t Bits, bool Usgnd = false>
   struct make_type
   {
      typedef int64_t type;
   };

   template<>
   struct make_type<8, false>
   {
      typedef int8_t type;
   };

   template<>
   struct make_type<16, false>
   {
      typedef int16_t type;
   };

   template<>
   struct make_type<24, false>
   {
      typedef int32_t type;
   };

   template<>
   struct make_type<32, false>
   {
      typedef int32_t type;
   };

   /*
    Default make_type for unsigned type.
    */
   template<size_t Bits>
   struct make_type<Bits, true>
   {
      typedef uint64_t type;
   };

   template<>
   struct make_type<8, true>
   {
      typedef uint8_t type;
   };

   template<>
   struct make_type<16, true>
   {
      typedef uint16_t type;
   };

   template<>
   struct make_type<24, true>
   {
      typedef uint32_t type;
   };

   template<>
   struct make_type<32, true>
   {
      typedef uint32_t type;
   };

   static_assert(sizeof(make_type<bit_size<1>::value>::type) == 1,
              "make_type did not allocate the right size (1)");

   static_assert(sizeof(make_type<bit_size<8>::value>::type) == 1,
                 "make_type did not allocate the right size (8)");

   static_assert(sizeof(make_type<bit_size<15>::value>::type) == 2,
                 "make_type did not allocate the right size (15)");

   static_assert(sizeof(make_type<bit_size<17>::value>::type) == 4,
                 "make_type did not allocate the right size (17)");

   static_assert(sizeof(make_type<bit_size<33>::value>::type) == 8,
                 "make_type did not allocate the right size (33)");
}