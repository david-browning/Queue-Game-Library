/* File Information

   Name:					annotations.h
   Description:
   Notes:

   Date:					5/24/19
   Copyright:			Copyright David Browning 2012
   Author:				David Browning
*/

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <queue>
#include <forward_list>
#include <functional>
#include <vector>
#include <iomanip>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <sstream>
#include <atomic>

static constexpr double PI_D = 3.14159265358979323846;
static constexpr double PI_F = 3.14159265358979323846f;

#if !defined(NDEBUG) || defined(_DEBUG) || defined(DEBUG)

#ifndef _DEBUG
#define _DEBUG
#endif

#ifndef DEBUG
#define DEBUG
#endif

#endif

#ifndef EXTERN_C
#define EXTERN_C (extern "C")
#endif

typedef void* handle_t;

#ifdef _WIN32

typedef void* file_handle_t;
const file_handle_t NO_FILE = NULL;

#ifdef UNICODE
typedef wchar_t sys_char;
#else
typedef char sys_char;
#endif

//The target is exported when compiling a library.
#define LIB_EXPORT __declspec(dllexport)
#define LIB_IMPORT __declspec(dllimport)

#define FUNC_NO_RETURN __declspec(noreturn)

//Use when the function is small and not likely to be called often, 
//such as a function that handles an error condition.
#define FUNC_NO_INLINE __declspec(noinline)

#define MIC_FUNC __declspec(target(mic))

static const std::string LIB_EXPORT_STR = "__declspec(dllexport)";

inline std::string HrToString(unsigned long hr)
{
   char s_str[64] = {};
   sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<unsigned long>(hr));
   return std::string(s_str);
}

#else

typedef int file_handle_t;
const file_handle_t NO_FILE = -1;
typedef char sys_char;

//The target is exported when compiling a library.
#define LIB_EXPORT __attribute__((__visibility__("default")))
#define LIB_IMPORT LIB_EXPORT
#define MIC_FUN __attribute__(mic)

static const std::string LIB_EXPORT_STR = "__attribute__((__visibility__(\"default\")))";

#endif

typedef std::basic_string<sys_char> sys_str;

#define _DEV_SET_
#define _DEV_DO_NOT_SET_

//Indicates a design by contract function or parameter. The design by contract target does no 
//error checking on its parameters. It is the caller's responsibility to make sure the passed
//parameters obey the contract.
#define _DBC_

//The variable is an input parameter.
#define _IN_

//The variable is an input parameter. It can be a null pointer.
#define _IN_CAN_BE_NULL_

//The variable is an optional input parameter.
#define _IN_OPT_

//The variable is a size_t used as an input parameter.
#define _IN_SIZE_
//The variable is  asize_t array used as an input parameter.
#define _IN_SIZE_ARR_

//The variable is an enumeration used as an input parameter.
#define _IN_ENUM_
//The variable is an enumeration array used as an input parameter.
#define _IN_ENUM_ARR_
//The variable is an enumeration array with a zero at the end and is used as an input parameter.
#define _IN_ENUM_ARR_Z_

//The variable is an array used as an input parameter.
#define _IN_ARR_
//The variable is a zero terminated array used as an input parameter.
#define _IN_ARR_Z_

//The variable is a bool used as an input parameter.
#define _IN_BOOL_
//The variable is an array of bools used as an input parameter.
#define _IN_BOOL_ARR_
//The variable is a zero terminated bool array used as an input parameter.
#define _IN_BOOL_ARR_Z_

//The variable is a byte used as an input parameter.
#define _IN_BYTE_
//The variable is an array of bytes used as an input parameter.
#define _IN_BYTE_ARR_
//The variable is a zero terminated byte array used as an input parameter.
#define _IN_BYTE_ARR_Z_

//The variable is a char used as an input parameter.
#define _IN_CHAR_
//The variable is an array of characters used as an input parameter.
#define _IN_CHAR_ARR_
//The variable is a zero terminated char array used as an input parameter.
#define _IN_CHAR_ARR_Z_

//The variable is a short used as an input parameter.
#define _IN_SHORT_
//The variable is an array of shorts used as an input parameter.
#define _IN_SHORT_ARR_
//The variable is a zero terminated short array used as an input parameter.
#define _IN_SHORT_ARR_Z_

//The variable is a wchar_t used as an input parameter.
#define _IN_WCHAR_
//The variable is an array of wide characters used as an input parameter.
#define _IN_WCHAR_ARR_
//The variable is a zero terminated wchar_t array used as an input parameter.
#define _IN_WCHAR_ARR_Z_

//The variable is a wide character string used as an input parameter.
#define _IN_WSTR_
//The variable is a zero terminated wide character string used as an input parameter.
#define _IN_WSTR_Z_
//The variable is an array of zero terminated wide character strings used as an input parameter.
#define _IN_WSTR_Z_POINTER
//The variable is an array of wide character strings used as an input parameter.
#define _IN_WSTR_ARR_
//The variable is a zero terminated wchar_t* array used as an input parameter.
#define _IN_WSTR_ARR_Z_

//The variable is an int used as an input parameter.
#define _IN_INT_
//The variable is an array of ints used as an input parameter.
#define _IN_INT_ARR_
//The variable is a zero terminated int array used as an input parameter.
#define _IN_INT_ARR_Z_

//The variable is a float used as an input parameter.
#define _IN_FLOAT_
//The variable is an array of floats used as an input parameter.
#define _IN_FLOAT_ARR_
//The variable is a zero terminated float array used as an input parameter.
#define _IN_FLOAT_ARR_Z_

//The variable is a long used as an input parameter.
#define _IN_LONG_
//The variable is an array of longs used as an input parameter.
#define _IN_LONG_ARR_
//The variable is a zero terminated long array used as an input parameter.
#define _IN_LONG_ARR_Z_

//The variable is a double used as an input parameter.
#define _IN_DOUBLE_
//The variable is an array of doubles used as an input parameter.
#define _IN_DOUBLE_ARR_
//The variable is a zero terminated double array used as an input parameter.
#define _IN_DOUBLE_ARR_Z_

//The variable is a string used as an input parameter.
#define _IN_STR_
//The variable is a zero terminated string used as an input parameter.
#define _IN_STR_Z_
//The variable is an array of strings used as an input parameter.
#define _IN_STR_ARR_
//The variable is a zero terminated string array used as an input parameter.
#define _IN_STR_ARR_Z_

//The variable is a pointer to a variable used as an input parameter.
#define _IN_POINTER_
//The variable is an array of pointers used as an input parameter.
#define _IN_POINTER_ARR_
//The variable is a zero terminated pointer array used as an input parameter.
#define _IN_POINTER_ARR_Z_

//The variable is a callback used as an input parameter.
#define _IN_CALLBACK_
//The variable is an array of callbacks used as an input parameter.
#define _IN_CALLBACK_ARR_
//The variable is a zero terminated function pointer array used as an input parameter.
#define _IN_CALLBACK_ARR_Z_

//The variable is a function used as an input parameter.
#define _IN_FUNCTION_
//The variable is an array of functions used as an input parameter.
#define _IN_FUNCTION_ARR_
//The variable is a zero terminated function pointer array used as an input parameter.
#define _IN_FUNCTION_ARR_Z_

#define _IN_HANDLE_


#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4275)
#endif
class file_not_found_exception : public std::exception
{
   public:
   file_not_found_exception() :
      std::exception("The file was not found.")
   {

   }
};
#ifdef _WIN32
#pragma warning(pop)
#endif

template<typename T>
constexpr T logbn(const T base, const T n)
{
   return (n > base - 1) ? 1 + logbn(n / base, base) : 0;
}

template<typename T>
constexpr T RoundUp(_IN_ const T val, _IN_ const T nearest)
{
   T remainder = val % nearest;
   return remainder == 0 ? val : val + nearest - remainder;
}

template<typename T>
constexpr T GetMin(T value)
{
   return value;
}

template<typename T, typename... Tlist>
constexpr T GetMin(T value, Tlist... list)
{
   T min = GetMin<T>(list...);
   return value < min ? value : min;
}

inline constexpr uintptr_t MemAlignAddress(const uintptr_t addr,
                                           const size_t alignment) noexcept
{
   return (addr + alignment - 1) & ~(alignment - 1);
}

template<typename T>
void MemoryReplace(_IN_ARR_Z_ T* ptr,
                   _IN_ const T toReplace,
                   _IN_ const T with,
                   _IN_SIZE_ size_t numElements)
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
void MemorySet(_IN_POINTER_ T* const ptr,
               _IN_ T val,
               _IN_SIZE_ size_t numElements)
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
void MemoryCopy(_IN_POINTER_ T* const dest,
                _IN_POINTER_ const T* src,
                _IN_SIZE_ size_t numElements)
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
void MemoryCopyUntil(_IN_POINTER_ T* dest,
                     _IN_POINTER_ const T* src,
                     _IN_SIZE_ T terminator)
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
Searches an array of "numElements" elements of type T until it finds an element equal to "val".
Returns a pointer to the first occurrence of "val" in the block of memory pointed to by "ptr".
Returns nullptr if the value is not found.
*/
template<typename T>
T* MemoryFind(_IN_POINTER_ const T* ptr,
              _IN_ T val,
              _IN_SIZE_ size_t numElements)
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
int MemoryCompare(_IN_POINTER_ const T* p1,
                  _IN_POINTER_ const T* p2,
                  _IN_SIZE_ const size_t numElements)
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
Counts the number of elements until an element of value 0 is found.
The returned count does not include the null terminator.
*/
template<typename T>
constexpr size_t MemoryLength(_IN_ARR_Z_ const T* str,
                              _IN_OPT_ const T terminator = 0,
                              _IN_OPT_ const size_t i = 0)
{
   return (str && str[i] != terminator) ? MemoryLength(str, terminator, i + 1) + 1 : 0;
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
size_t BitCount(_IN_ T n)
{
   std::bitset<sizeof(T) * CHAR_BIT> b(n);
   return b.count();
}

inline bool approx_equal(float expected,
                         float actual,
                         float tolerance = FLT_EPSILON)
{
   return abs(expected - actual) < tolerance;
}

inline bool approx_equal(double expected,
                         double actual,
                         double tolerance = DBL_EPSILON)
{
   return abs(expected - actual) < tolerance;
}
