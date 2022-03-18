#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cmath>

//System
#include <memory>
#include <mutex>
#include <stdexcept>
#include <atomic>
#include <system_error>

//IO and Streams
#include <iostream>
#include <fstream>
#include <sstream>

//Containers
#include <string>
#include <list>
#include <queue>
#include <forward_list>
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <variant>

//Algorithms
#include <algorithm>
#include <functional>
#include <execution>
#include <iomanip>

#if !defined(NDEBUG) || defined(_DEBUG) || defined(DEBUG)

#ifndef _DEBUG
#define _DEBUG
#endif

#ifndef DEBUG
#define DEBUG
#endif

#endif


#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define NOMINMAX 
#include <Windows.h>
#include <objbase.h>
#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.Threading.h>

#pragma region Define Handle Types
typedef HANDLE handle_t;

template<typename Handle = handle_t>
constexpr Handle INVALID_HANDLE = INVALID_HANDLE_VALUE;

typedef HANDLE thread_handle_t;

typedef HANDLE file_handle_t;
static const file_handle_t NO_FILE = INVALID_HANDLE_VALUE;
#pragma endregion

#ifdef UNICODE
typedef wchar_t sys_char;

#else
typedef char sys_char;
#endif


#else

#include <unistd.h>
#include <uuid/uuid.h>

#pragma region Define Handle Types
using handle_t = typename void*

template<typename Handle = handle_t>
constexpr Handle INVALID_HANDLE = static_cast<Handle>(-1);

typedef void* thread_handle_t;

typedef int file_handle_t;
static constexpr file_handle_t NO_FILE = -1;
#pragma endregion

typedef char sys_char;

#endif

