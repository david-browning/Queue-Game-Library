/* File Information

   Name:					annotations.h
   Description:
   Notes:

   Date:					8/17/13
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


//The variable is an output parameter.
#define _OUT_

//The variable is an output parameter. It can be a null pointer.
#define _OUT_CAN_BE_NULL_

//The variable is an optional output parameter.
#define _OUT_OPTIONAL_

//The variable is a size_t used as an output parameter.
#define _OUT_SIZE_
//The variable is  asize_t array used as an output parameter.
#define _OUT_SIZE_ARR_

//The variable is an enumeration used as an output parameter.
#define _OUT_ENUM_
//The variable is an enumeration array used as an output parameter.
#define _OUT_ENUM_ARR_
//The variable is an enumeration array with a zero at the end and is used as an output parameter.
#define _OUT_ENUM_ARR_Z_

//The variable is an array used as an output parameter.
#define _OUT_ARR_
//The variable is a zero terminated array used as an output parameter.
#define _OUT_ARR_Z_

//The variable is a bool used as an output parameter.
#define _OUT_BOOL_
//The variable is an array of bools used as an output parameter.
#define _OUT_BOOL_ARR_
//The variable is a zero terminated bool array used as an output parameter.
#define _OUT_BOOL_ARR_Z_

//The variable is a byte used as an output parameter.
#define _OUT_BYTE_
//The variable is an array of bytes used as an output parameter.
#define _OUT_BYTE_ARR_
//The variable is a zero terminated byte array used as an output parameter.
#define _OUT_BYTE_ARR_Z_

//The variable is a char used as an output parameter.
#define _OUT_CHAR_
//The variable is an array of characters used as an output parameter.
#define _OUT_CHAR_ARR_
//The variable is a zero terminated char array used as an output parameter.
#define _OUT_CHAR_ARR_Z_

//The variable is a short used as an output parameter.
#define _OUT_SHORT_
//The variable is an array of shorts used as an output parameter.
#define _OUT_SHORT_ARR_
//The variable is a zero terminated short array used as an output parameter.
#define _OUT_SHORT_ARR_Z_

//The variable is a wchar_t used as an output parameter.
#define _OUT_WCHAR_
//The variable is an array of wide characters used as an output parameter.
#define _OUT_WCHAR_ARR_
//The variable is a zero terminated wchar_t array used as an output parameter.
#define _OUT_WCHAR_ARR_Z_

//The variable is a wide character string used as an output parameter.
#define _OUT_WSTR_
//The variable is a zero terminated wide character string used as an output parameter.
#define _OUT_WSTR_Z_
//The variable is a pointer to wide character zero terminated string. 
#define _OUT_WSTR_Z_POINTER_
//The variable is an array of wide character strings used as an output parameter.
#define _OUT_WSTR_ARR_
//The variable is a zero terminated wchar_t* array used as an output parameter.
#define _OUT_WSTR_ARR_Z_

//The variable is an int used as an output parameter.
#define _OUT_INT_
//The variable is an array of ints used as an output parameter.
#define _OUT_INT_ARR_
//The variable is a zero terminated int array used as an output parameter.
#define _OUT_INT_ARR_Z_

//The variable is a float used as an output parameter.
#define _OUT_FLOAT_
//The variable is an array of floats used as an output parameter.
#define _OUT_FLOAT_ARR_
//The variable is a zero terminated float array used as an output parameter.
#define _OUT_FLOAT_ARR_Z_

//The variable is a long used as an output parameter.
#define _OUT_LONG_
//The variable is an array of longs used as an output parameter.
#define _OUT_LONG_ARR_
//The variable is a zero terminated long array used as an output parameter.
#define _OUT_LONG_ARR_Z_

//The variable is a double used as an output parameter.
#define _OUT_DOUBLE_
//The variable is an array of doubles used as an output parameter.
#define _OUT_DOUBLE_ARR_
//The variable is a zero terminated double array used as an output parameter.
#define _OUT_DOUBLE_ARR_Z_

//The variable is a string used as an output parameter.
#define _OUT_STR_
//The variable is a zero terminated string used as an output parameter.
#define _OUT_STR_Z_
//The variable is an array of strings used as an output parameter.
#define _OUT_STR_ARR_
//The variable is a zero terminated string array used as an output parameter.
#define _OUT_STR_ARR_Z_

//The variable is a pointer to a variable used as an output parameter.
#define _OUT_POINTER_
//The variable is an array of pointers used as an output parameter.
#define _OUT_POINTER_ARR_
//The variable is a zero terminated pointer array used as an output parameter.
#define _OUT_POINTER_ARR_Z_

//The variable is a callback used as an output parameter.
#define _OUT_CALLBACK_
//The variable is an array of callbacks used as an output parameter.
#define _OUT_CALLBACK_ARR_
//The variable is a zero terminated function pointer array used as an output parameter.
#define _OUT_CALLBACK_ARR_Z_

//The variable is a function used as an output parameter.
#define _OUT_FUNCTION_
//The variable is an array of functions used as an output parameter.
#define _OUT_FUNCTION_ARR_
//The variable is a zero terminated function pointer array used as an output parameter.
#define _OUT_FUNCTION_ARR_Z_

#define _OUT_HANDLE_

#define _OUT_OPT_


//The variable is an input and output parameter.
#define _INOUT_

//The variable is an input and output parameter. It can be a null pointer.
#define _INOUT_CAN_BE_NULL_

//The variable is an optional input and output parameter.
#define _INOUT_OPTIONAL_

//The variable is a size_t used as an input and output parameter.
#define _INOUT_SIZE_
//The variable is  asize_t array used as an input and output parameter.
#define _INOUT_SIZE_ARR_

//The variable is an enumeration used as an input and output parameter.
#define _INOUT_ENUM_
//The variable is an enumeration array used as an input and output parameter.
#define _INOUT_ENUM_ARR_
//The variable is an enumeration array with a zero at the end and is used as an input and output parameter.
#define _INOUT_ENUM_ARR_Z_

//The variable is an array used as an input and output parameter.
#define _INOUT_ARR_
//The variable is a zero terminated array used as an input and output parameter.
#define _INOUT_ARR_Z_

//The variable is a bool used as an input and output parameter.
#define _INOUT_BOOL_
//The variable is an array of bools used as an input and output parameter.
#define _INOUT_BOOL_ARR_
//The variable is a zero terminated bool array used as an input and output parameter.
#define _INOUT_BOOL_ARR_Z_

//The variable is a byte used as an input and output parameter.
#define _INOUT_BYTE_
//The variable is an array of bytes used as an input and output parameter.
#define _INOUT_BYTE_ARR_
//The variable is a zero terminated byte array used as an input and output parameter.
#define _INOUT_BYTE_ARR_Z_

//The variable is a char used as an input and output parameter.
#define _INOUT_CHAR_
//The variable is an array of characters used as an input and output parameter.
#define _INOUT_CHAR_ARR_
//The variable is a zero terminated char array used as an input and output parameter.
#define _INOUT_CHAR_ARR_Z_

//The variable is a short used as an input and output parameter.
#define _INOUT_SHORT_
//The variable is an array of shorts used as an input and output parameter.
#define _INOUT_SHORT_ARR_
//The variable is a zero terminated short array used as an input and output parameter.
#define _INOUT_SHORT_ARR_Z_

//The variable is a wchar_t used as an input and output parameter.
#define _INOUT_WCHAR_
//The variable is an array of wide characters used as an input and output parameter.
#define _INOUT_WCHAR_ARR_
//The variable is a zero terminated wchar_t array used as an input and output parameter.
#define _INOUT_WCHAR_ARR_Z_

//The variable is a wide character string used as an input and output parameter.
#define _INOUT_WSTR_
//The variable is a zero terminated wide character string used as an input and output parameter.
#define _INOUT_WSTR_Z_
//The variable is an array of zero terminated wide character strings used as an input and output parameter.
#define _INOUT_WSTR_Z_POINTER
//The variable is an array of wide character strings used as an input and output parameter.
#define _INOUT_WSTR_ARR_
//The variable is a zero terminated wchar_t* array used as an input and output parameter.
#define _INOUT_WSTR_ARR_Z_

//The variable is an int used as an input and output parameter.
#define _INOUT_INT_
//The variable is an array of ints used as an input and output parameter.
#define _INOUT_INT_ARR_
//The variable is a zero terminated int array used as an input and output parameter.
#define _INOUT_INT_ARR_Z_

//The variable is a float used as an input and output parameter.
#define _INOUT_FLOAT_
//The variable is an array of floats used as an input and output parameter.
#define _INOUT_FLOAT_ARR_
//The variable is a zero terminated float array used as an input and output parameter.
#define _INOUT_FLOAT_ARR_Z_

//The variable is a long used as an input and output parameter.
#define _INOUT_LONG_
//The variable is an array of longs used as an input and output parameter.
#define _INOUT_LONG_ARR_
//The variable is a zero terminated long array used as an input and output parameter.
#define _INOUT_LONG_ARR_Z_

//The variable is a double used as an input and output parameter.
#define _INOUT_DOUBLE_
//The variable is an array of doubles used as an input and output parameter.
#define _INOUT_DOUBLE_ARR_
//The variable is a zero terminated double array used as an input and output parameter.
#define _INOUT_DOUBLE_ARR_Z_

//The variable is a string used as an input and output parameter.
#define _INOUT_STR_
//The variable is a zero terminated string used as an input and output parameter.
#define _INOUT_STR_Z_
//The variable is an array of strings used as an input and output parameter.
#define _INOUT_STR_ARR_
//The variable is a zero terminated string array used as an input and output parameter.
#define _INOUT_STR_ARR_Z_

//The variable is a pointer to a variable used as an input and output parameter.
#define _INOUT_POINTER_
//The variable is an array of pointers used as an input and output parameter.
#define _INOUT_POINTER_ARR_
//The variable is a zero terminated pointer array used as an input and output parameter.
#define _INOUT_POINTER_ARR_Z_

//The variable is a callback used as an input and output parameter.
#define _INOUT_CALLBACK_
//The variable is an array of callbacks used as an input and output parameter.
#define _INOUT_CALLBACK_ARR_
//The variable is a zero terminated function pointer array used as an input and output parameter.
#define _INOUT_CALLBACK_ARR_Z_

//The variable is a function used as an input and output parameter.
#define _INOUT_FUNCTION_
//The variable is an array of functions used as an input and output parameter.
#define _INOUT_FUNCTION_ARR_
//The variable is a zero terminated function pointer array used as an input and output parameter.
#define _INOUT_FUNCTION_ARR_Z_

typedef int64_t result_t;
typedef uint32_t facility_result_t;
typedef uint32_t code_result_t;

inline constexpr result_t MakeResult(const bool failed,
                                     const facility_result_t facility,
                                     const code_result_t code) noexcept
{
    return (failed ? 0x8000000000000000 : 0) |
        (((uint64_t)facility & 0x7FFFFFFF) << (sizeof(code_result_t) * uint32_t(8))) |
        code;
}

//Returns true if the result indicates a failure.
inline constexpr bool ResultFailed(const result_t r) noexcept
{
    return r < 0;
}

//Returns the error code from the result.
inline code_result_t GetCodeFromResult(const result_t r) noexcept
{
    return (code_result_t)(r & 0x00000000FFFFFFFF);
}

//Returns the facility code from the result.
inline facility_result_t GetFacilityFromResult(const result_t r) noexcept
{
    return (facility_result_t)((r >> sizeof(code_result_t) * 8) & 0x00007FFFFFFF);
}

static constexpr facility_result_t FACILITY_NONE = 0x0000;

static constexpr facility_result_t FACILITY_DEVICE = 0x0001;
static constexpr facility_result_t FACILITY_KEYBOARD = 0x0003;

static constexpr facility_result_t FACILITY_CPU = 0x0002;
static constexpr facility_result_t FACILITY_MEMORY = 0x0004;
static constexpr facility_result_t FACILITY_DISK = 0x0006;
static constexpr facility_result_t FACILITY_NETWORK = 0x0008;
static constexpr facility_result_t FACILITY_FILE = 0x0010;

static constexpr facility_result_t FACILITY_PROCESS = 0x0020;
static constexpr facility_result_t FACILITY_LIBRARY = 0x0040;
static constexpr facility_result_t FACILITY_THREAD = 0x0080;
static constexpr facility_result_t FACILITY_INTERRUPT = 0x0090;
static constexpr facility_result_t FACILITY_KERNEL = 0x00A0;

static constexpr code_result_t CODE_NONE = 0x0000;
static constexpr code_result_t CODE_UNSUPPORTED = 0x0001;
static constexpr code_result_t CODE_UNKNOWN = 0x0002;
static constexpr code_result_t CODE_OUT_OF_MEM = 0x0003;
static constexpr code_result_t CODE_NOT_EQUAL = 0x0004;
static constexpr code_result_t CODE_NOT_FOUND = 0x0005;
static constexpr code_result_t CODE_INVALID = 0x0006;
static constexpr code_result_t CODE_FULL = 0x0007;
static constexpr code_result_t CODE_READ_ERROR = 0x0008;
static constexpr code_result_t CODE_WRITE_ERROR = 0x0009;
static constexpr code_result_t CODE_NOT_AVAILABLE = 0x000A;
static constexpr code_result_t CODE_NOT_ENOUGH = 0x000B;
static constexpr code_result_t CODE_TEST_FAILED = 0x000C;
static constexpr code_result_t CODE_RESET_FAILED = 0x000D;
static constexpr code_result_t CODE_TIME_OUT = 0x000E;
static constexpr code_result_t CODE_NOT_IMPLEMENTED = 0x000F;
static constexpr code_result_t CODE_NOT_INITIALIZED = 0x0010;
static constexpr code_result_t CODE_OUT_OF_BOUNDS = 0x0011;

static constexpr result_t RES_SUCCESS = MakeResult(false, FACILITY_NONE, CODE_NONE);

//This is not an error code. It states that the action cannot be completed right now.
static constexpr result_t RES_NOT_NOW = MakeResult(false, FACILITY_NONE, CODE_INVALID);
static constexpr result_t RES_NOT_IMPLEMENTED = MakeResult(true, FACILITY_NONE, CODE_NOT_IMPLEMENTED);

static constexpr result_t RES_UNSUPPORTED = MakeResult(true, FACILITY_NONE, CODE_UNSUPPORTED);
static constexpr result_t RES_OUT_OF_MEMORY = MakeResult(true, FACILITY_MEMORY, CODE_OUT_OF_MEM);
static constexpr result_t RES_NOT_FOUND = MakeResult(true, FACILITY_MEMORY, CODE_NOT_FOUND);
static constexpr result_t RES_INVALID_ARG = MakeResult(true, FACILITY_LIBRARY, CODE_UNSUPPORTED);
static constexpr result_t RES_TOO_SMALL = MakeResult(true, FACILITY_MEMORY, CODE_NOT_ENOUGH);

static constexpr result_t RES_READ_ERROR = MakeResult(true, FACILITY_FILE, CODE_READ_ERROR);
static constexpr result_t RES_WRITE_ERROR = MakeResult(true, FACILITY_FILE, CODE_WRITE_ERROR);
static constexpr result_t RES_FILE_NOT_FOUND = MakeResult(true, FACILITY_FILE, CODE_NOT_FOUND);
static constexpr result_t RES_FILE_SEEK_INVALID = MakeResult(true, FACILITY_FILE, CODE_OUT_OF_BOUNDS);

static constexpr result_t RES_ARRAY_FULL = MakeResult(true, FACILITY_MEMORY, CODE_FULL);
static constexpr result_t RES_NOT_AVAILABLE = MakeResult(true, FACILITY_MEMORY, CODE_NOT_AVAILABLE);

static constexpr result_t RES_PAGE_NOT_ALLOCATED = MakeResult(true, FACILITY_MEMORY, CODE_NOT_INITIALIZED);

static constexpr result_t RES_UNKNOWN_LIB_ERROR = MakeResult(true, FACILITY_LIBRARY, CODE_UNKNOWN);
static constexpr result_t RES_UNKNOWN_DEVICE_ERROR = MakeResult(true, FACILITY_DEVICE, CODE_UNKNOWN);
static constexpr result_t RES_UNKNOWN_NETWORK_ERROR = MakeResult(true, FACILITY_NETWORK, CODE_UNKNOWN);
static constexpr result_t RES_UNKNOWN_FILE_ERROR = MakeResult(true, FACILITY_FILE, CODE_UNKNOWN);
static constexpr result_t RES_UNKNOWN_THREAD_ERROR = MakeResult(true, FACILITY_THREAD, CODE_UNKNOWN);

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
