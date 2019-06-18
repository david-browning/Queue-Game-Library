/* File Information

   Name:					annotations.h
   Description:
   Notes:

   Date:					5/24/19
   Copyright:			Copyright David Browning 2012
   Author:				David Browning
*/
#pragma once


#ifdef _WIN32

//The target is exported when compiling a library.
#define LIB_EXPORT __declspec(dllexport)
#define LIB_IMPORT __declspec(dllimport)

#define FUNC_NO_RETURN __declspec(noreturn)

//Use when the function is small and not likely to be called often, 
//such as a function that handles an error condition.
#define FUNC_NO_INLINE __declspec(noinline)

#define MIC_FUNC __declspec(target(mic))

#else

//The target is exported when compiling a library.
#define LIB_EXPORT __attribute__((__visibility__("default")))
#define LIB_IMPORT LIB_EXPORT
#define MIC_FUN __attribute__(mic)

#endif


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
