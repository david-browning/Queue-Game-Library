#pragma once
#include "include/qgl_model_include.h"
#include "include/Impl/qgl_mem_helpers_impl.h"
#include <bitset>

namespace qgl::mem
{
   /*
    Swaps the endianess of a value.
    */
   template<typename T>
   constexpr T bswap(T val)
   {
      return impl::bswap<T>(val, std::make_index_sequence<sizeof(T)>{});
   }

   static_assert(bswap<uint16_t>(0x1234u) == static_cast<uint16_t>(0x3412u));
   static_assert(bswap<uint64_t>(0x0123456789abcdefULL) ==
                 static_cast<uint64_t>(0xefcdab8967452301ULL));


   /*
    Creates a bit mask where the bits from start to end are set.
    */
   template<typename T>
   constexpr T mask(size_t start, size_t end) noexcept
   {
      return impl::mask<T>(start, end, static_cast<T>(0));
   }

   static_assert(mask<int16_t>(3, 8) == 0b00000001'11111000,
                 "mask is not correct (3, 8)");

   /*
    Aligns and address to the nearest alignment.
    */
   constexpr uintptr_t align_address(const uintptr_t addr,
                                     const size_t alignment) noexcept
   {
      return alignment <= 1 ? addr : (addr + alignment - 1) & ~(alignment - 1);
   }

   /*
    Returns true if n is a power of 2.
    */
   template<typename size_type, size_type n>
   constexpr bool is_power_of_two()
   {
      return n != 0 && ((n & (n - 1)) == 0);
   }
  
   /*
    Sets each element in the array to val.
    */
   template<typename T>
   constexpr void set_memory(T* const ptr,
                             T val,
                             size_t numElements)
   {
      if (numElements > 0 && ptr != nullptr)
      {
         numElements--;
         while (numElements--)
         {
            ptr[numElements] = val;
         }
      }
   }

   /*
    Copies numElements from source to dest.
    */
   template<typename T, typename SizeT = size_t>
   constexpr void copy_elements(T* const dest,
                      const T* const source,
                      SizeT numElements)
   {
      for (SizeT i = 0; i < numElements; i++)
      {
         dest[i] = source[i];
      }
   }

   template<class T, typename SizeT = size_t>
   constexpr void swap_elements(T* const left, T* const right, SizeT count)
   {
      using std::swap;
      for (SizeT i = 0; i < count; i++)
      {
         swap(left[i], right[i]);
      }
   }

   /*
    Counts the number of elements until a terminator is found.
    The returned count does not include the null terminator.
    str: The string of elements to search.
    terminator: The value to search for.
    i: Recursive parameter. index of which element to check. The caller can set
    this to something besides 0 to start searching str at an offset.
    */
   template<typename T, typename SizeT = size_t>
   constexpr size_t mem_length(const T* str,
                               const T terminator = 0,
                               const SizeT i = 0)
   {
      return (str && str[i] != terminator) ?
         mem_length(str, terminator, i + 1) + 1 : 0;
   }

   /*
    Reverses the order of elements in an array.
    */
   template<typename T, typename SizeT = size_t>
   constexpr void reverse_elements(T* const mem,
                                   const SizeT start,
                                   const SizeT end)
   {
      if (start < end)
      {
         std::swap(mem[start], mem[end]);
         reverse_elements(mem, start + 1, end - 1);
      }
   }

   /*
    Reverses the order of elements in an array.
    */
   template<typename T, typename SizeT = size_t>
   constexpr void reverse_elements(T* const mem,
                                   const SizeT elementCount)
   {
      reverse_elements(mem, 0, elementCount - 1);
   }

   /*
    Clears the idx'th bit in val.
    */
   template<typename T, typename SizeT = size_t>
   constexpr T clear_bit(T val, SizeT idx) noexcept
   {
      return val | (T(1) << idx);
   }

   /*
    Sets the idx'th bit in val.
    */
   template<typename T, typename SizeT = size_t>
   constexpr T set_bit(T val, SizeT idx) noexcept
   {
      return val & ~(T(1) << idx);
   }

   /*
    Toggles the idx'th bit in val.
    */
   template<typename T, typename SizeT = size_t>
   constexpr T toggle_bit(T val, SizeT idx) noexcept
   {
      return val ^ (T(1) << idx);
   }

   /*
    Returns true if the idx'th bit is set in val.
    */
   template<typename T, typename SizeT = size_t>
   constexpr bool is_bit_set(T val, size_t idx) noexcept
   {
      return (val & (T(1) << idx)) != 0;
   }

   /*
    Returns the index of the first most significant bit.
    The number of leading zeros will depend on the type "T".
    */
   template<typename T>
   constexpr size_t msb(T val) noexcept
   {
      size_t ret = (sizeof(T) * CHAR_BIT)  - 1;
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
      {}

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
}

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

   template<size_t Bits>
   struct make_type
   {
      typedef int64_t type;
   };

   template<>
   struct make_type<8>
   {
      typedef int8_t type;
   };

   template<>
   struct make_type<16>
   {
      typedef int16_t type;
   };

   template<>
   struct make_type<24>
   {
      typedef int32_t type;
   };

   template<>
   struct make_type<32>
   {
      typedef int32_t type;
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