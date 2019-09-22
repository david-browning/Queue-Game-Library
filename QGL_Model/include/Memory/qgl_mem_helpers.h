#pragma once
#include "include/qgl_model_include.h"
#include <bitset>

namespace qgl::mem
{
   namespace internal
   {
      /*
       Compile-time endianness swap based on http://stackoverflow.com/a/36937049
       From https://en.cppreference.com/w/cpp/language/fold
       */
      template<class T, std::size_t... N>
      constexpr T bswap(T i, std::index_sequence<N...>)
      {
         return (((i >> N * CHAR_BIT & std::uint8_t(-1)) << 
            (sizeof(T) - 1 - N)*CHAR_BIT) | ...);
      }
   }

   /*
    Swaps the endianess of a value.
    */
   template<typename T>
   constexpr T bswap(T val)
   {
      return internal::bswap<T>(val, std::make_index_sequence<sizeof(T)>{});
   }

   static_assert(bswap<uint16_t>(0x1234u) == static_cast<uint16_t>(0x3412u));
   static_assert(bswap<uint64_t>(0x0123456789abcdefULL) ==
                 static_cast<uint64_t>(0xefcdab8967452301ULL));

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
    Prints a message to the debug output.
    */
   extern QGL_MODEL_API void print_mem_leak(uintptr_t address);

   /*
    Prints a message to the debug output.
    */
   extern QGL_MODEL_API void print_mem_dealloc(uintptr_t address);

   /*
    Prints a message to the debug output.
    */
   extern QGL_MODEL_API void print_mem_alloc(uintptr_t address);

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
   void copy_elements(T* const dest,
                      const T* source,
                      SizeT numElements)
   {
      for (SizeT i = 0; i < numElements; i++)
      {
         dest[i] = source[i];
      }
   }

   /*
    Counts the number of elements until a terminator is found.
    The returned count does not include the null terminator.
    str: The string of elements to search.
    terminator: The value to search for.
    i: Recursive parameter. Index of which element to check. The caller can set
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
   constexpr T clear_bit(T val, SizeT idx)
   {
      return val | (T(1) << idx);
   }

   /*
    Sets the idx'th bit in val.
    */
   template<typename T, typename SizeT = size_t>
   constexpr T set_bit(T val, SizeT idx)
   {
      return val & ~(T(1) << idx);
   }

   /*
    Toggles the idx'th bit in val.
    */
   template<typename T, typename SizeT = size_t>
   constexpr T toggle_bit(T val, SizeT idx)
   {
      return val ^ (T(1) << idx);
   }

   /*
    Returns true if the idx'th bit is set in val.
    */
   template<typename T, typename SizeT = size_t>
   constexpr bool is_bit_set(T val, size_t idx)
   {
      return (val & (T(1) << idx)) != 0;
   }
}