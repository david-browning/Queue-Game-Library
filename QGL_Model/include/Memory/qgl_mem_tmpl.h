#pragma once
#include <stdint.h>

inline constexpr uintptr_t MemAlignAddress(const uintptr_t addr,
                                           const size_t alignment) noexcept
{
   return (addr + alignment - 1) & ~(alignment - 1);
}

template<typename T>
void MemoryReplace(T* ptr,
                   const T toReplace,
                   const T with,
                   size_t numElements)
{
   if (numElements > 0 && ptr != nullptr)
   {
      for (int i = 0; i < numElements; i++)
      {
         if (ptr[i] == toReplace)
         {
            ptr[i] = with;
         }
      }
   }
}

template<typename T>
void MemorySet(T* const ptr,
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
Copies the "numElements" number of elements of type "T" from "src" to "dest".
The number of bytes copied is numElements * sizeof(T)
Does nothing if either pointer is nullptr.
*/
template<typename T>
void MemoryCopy(T* const dest,
                const T* src,
                size_t numElements)
{
   if (numElements != 0 && dest != nullptr && src != nullptr)
   {
      for (size_t i = 0; i < numElements; i++)
      {
         dest[i] = src[i];
      }
   }
}

/*
Copies memory from dest to src until terminator is found in src.
*/
template<typename T>
void MemoryCopyUntil(T* dest,
                     const T* src,
                     T terminator)
{
   if (dest == nullptr || src == nullptr)
   {
      return;
   }

   int i = 0;
   while (src[i] != terminator)
   {
      dest[i] = src[i];
      i++;
   }
}

/*
 Searches an array of "numElements" of type T until it finds an element 
 equal to "val".
 Returns a pointer to the first occurrence of "val" in the block of memory 
 pointed to by "ptr".
 Returns nullptr if the value is not found.
 */
template<typename T>
T* MemoryFind(const T* ptr,
              T val,
              size_t numElements)
{
   for (size_t i = 0; i < numElements; i++)
   {
      if (ptr[i] == val)
      {
         return ptr + i;
      }
   }

   return nullptr;
}

template<typename T>
size_t MemoryIndex(const T* const ptr,
                   T val,
                   size_t numElements)
{
   for (size_t i = 0; i < numElements; i++)
   {
      if (ptr[i] == val)
      {
         return i;
      }
   }

   return static_cast<size_t>(-1);
}

/*
 Compares each element in p1 to p2 until an element is not equal.
 Returns 0 if the blocks of memory each pointer points to are equal.
 Returns -1 if the p1 element is less than p2's element.
 Returns 1 if the p1 element is greater than p2's element.
 Returns -2 is one of any of the arguments are invalid.
 */
template<typename T>
int MemoryCompare(const T* p1,
                  const T* p2,
                  const size_t numElements)
{
   if (p1 == nullptr || p2 == nullptr || numElements == 0)
   {
      return -2;
   }

   for (size_t i = 0; i < numElements; i++)
   {
      if (p1[i] < p2[i])
      {
         return -1;
      }
      else if (p1[i] > p2[i])
      {
         return 1;
      }
   }

   return 0;
}

/*
 Counts the number of elements until a terminator is found.
 The returned count does not include the null terminator.
 str: The string of elements to search.
 terminator: The value to search for.
 i: Recursive parameter. Index of which element to check. The caller can set
 this to something besides 0 to start searching str at an offset.
 */
template<typename T>
constexpr size_t MemoryLength(const T* str,
                              const T terminator = 0,
                              const size_t i = 0)
{
   return (str && str[i] != terminator) ?
      MemoryLength(str, terminator, i + 1) + 1 : 0;
}

template<typename T>
constexpr void mem_reverse(T* const mem,
                           const size_t start,
                           const size_t end)
{
   if (start < end)
   {
      std::swap(mem[start], mem[end]);
      mem_reverse(mem, start + 1, end - 1);
   }
}

template<typename T>
constexpr void mem_reverse(T* const mem,
                           const size_t elementCount)
{
   mem_reverse(mem, 0, elementCount - 1);
}

template<typename T>
size_t BitCount(T n)
{
   std::bitset<sizeof(T) * CHAR_BIT> b(n);
   return b.count();
}