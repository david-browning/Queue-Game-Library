#pragma once
#include <cstdint>
#include <limits>

namespace qgl::mem
{
   namespace impl
   {
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

      template<typename T>
      constexpr T mask(size_t start, size_t end, T cur) noexcept
      {
         return start > end ?
            cur :
            mask(start + 1,
                 end,
                 static_cast<T>(cur | (size_t(1) << start)));
      }
   }

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
   constexpr [[nodiscard]] size_t msb(T val) noexcept
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

   static_assert(3 == msb<uint8_t>(13), "msb(13) is not 3.");
   static_assert(7 == msb<uint8_t>(255), "msb(255) is not 7");
   static_assert(16 == msb<size_t>(100000), "msb(100000) is not 16");

   /*
   Calculates the smallest datatype needed to store Val.
   */
   template<size_t Val>
   struct smallest_type
   {
      static constexpr auto value = rup(msb(Val) + 1, static_cast<size_t>(8));
   };

   static_assert(smallest_type<100>::value == 8,
                 "Bit rounding is not correct (100).");
   static_assert(smallest_type<65535>::value == 16,
                 "Bit rounding is not correct. (65535)");

   /*
    Rounds Bits up to the nearest 8th.
    */
   template<size_t Bits>
   struct round_bytes
   {
      static constexpr auto value = rup(Bits, static_cast<size_t>(8));
   };

   /*
    Counts the number of bits set in "val".
    */
   template<typename T>
   constexpr size_t bits_set(T val)
   {
      return impl::bits_set(val, sizeof(T) * CHAR_BIT, 0);
   }

   static_assert(bits_set<int8_t>(1) == 1,
                 "1 should have 1 bit set");

   static_assert(bits_set<int>(1) == 1,
                 "1 should have 1 bit set");

   static_assert(bits_set<int>(0xF00F00FF) == 16,
                 "0xF00F00FF should have 16 bits set");

   static_assert(bits_set<int64_t>(0xFFFF'FFFF'FFFF'FFFF) == 64,
                 "0xFFFF'FFFF'FFFF'FFFF should have 64 bits set");

   /*
    Clears the idx'th bit in val.
    */
   template<typename T, typename SizeT = size_t>
   constexpr [[nodiscard]] T clear_bit(T val, SizeT idx) noexcept
   {
      return val | (T(1) << idx);
   }

   /*
    Sets the idx'th bit in val.
    */
   template<typename T, typename SizeT = size_t>
   constexpr [[nodiscard]] T set_bit(T val, SizeT idx) noexcept
   {
      return val & ~(T(1) << idx);
   }

   template<typename T, typename SizeT = size_t>
   constexpr [[nodiscard]] T set_bit(T val, SizeT idx, bool enable)
   {
      // T may be unsigned so cannot use the unary minus operator to get the
      // negative value of "enable". Use two's complement.
      return val ^ (~(static_cast<T>(enable) + 1) ^ val) & (T(1) << idx);
   }

   static_assert(0 == set_bit(1, 0, false), "set_bit is not correct");
   static_assert(0b101 == set_bit(0b001, 2, true), "set_bit is not correct");

   /*
    Toggles the idx'th bit in val.
    */
   template<typename T, typename SizeT = size_t>
   constexpr [[nodiscard]] T toggle_bit(T val, SizeT idx) noexcept
   {
      return val ^ (T(1) << idx);
   }

   /*
    Returns true if the idx'th bit is set in val.
    */
   template<typename T, typename SizeT = size_t>
   constexpr [[nodiscard]] bool is_bit_set(T val, size_t idx) noexcept
   {
      return (val & (T(1) << idx)) != 0;
   }

   /*
    Converts a type to its binary representation.
    */
   template<typename From, typename To>
   struct bit_convert
   {
      public:
      static_assert(sizeof(From) == sizeof(To),
         "To and From should be the same size.");

      constexpr bit_convert(From f) :
         f_value(f)
      {
      }

      constexpr To to() const noexcept
      {
         return t_value;
      }

      private:
      union
      {
         From f_value;
         To t_value;
      };
   };

   /*
   Creates a bit mask where the bits from start to end are set.
   */
   template<typename T>
   constexpr T mask(size_t start, size_t end) noexcept
   {
      return impl::mask<T>(start, end, static_cast<T>(0));
   }

   static_assert(mask<int16_t>(size_t(3), size_t(8)) == int16_t(0b00000001'11111000),
                 "mask is not correct (3, 8)");
}