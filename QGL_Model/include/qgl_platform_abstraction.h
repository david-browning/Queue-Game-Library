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
#include <system_error>

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

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>
#include <DirectXMath.h>

typedef HANDLE thread_handle_t;

typedef HANDLE file_handle_t;
static const file_handle_t NO_FILE = INVALID_HANDLE_VALUE;

#ifdef UNICODE
typedef wchar_t sys_char;
#else
typedef char sys_char;
#endif

inline std::string HrToString(HRESULT hr)
{
   char s_str[64] = {};
   sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<unsigned long>(hr));
   return std::string(s_str);
}

#else

typedef void* thread_handle_t;

typedef int file_handle_t;
static constexpr file_handle_t NO_FILE = -1;
typedef char sys_char;

#endif

typedef std::basic_string<sys_char> sys_str;
