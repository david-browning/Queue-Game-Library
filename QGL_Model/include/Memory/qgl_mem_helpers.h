#pragma once
#include "include/qgl_model_include.h"
#include "include/Impl/qgl_mem_helpers_impl.h"
#include "include/Memory/qgl_mem_traits.h"

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
   constexpr typename qgl::mem::traits::make_type<(n - 1) * 4>::type from_hex(
      const char s[n])
   {
      return impl::from_hex<n>(s, 0, 0);
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