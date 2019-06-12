﻿#pragma once

#include "include/annotations.h"
#include "include/Memory/qgl_mem.h"
#include "include/qgl_platform_abstraction.h"

#ifdef QGL_MODEL_EXPORTS
#define QGL_MODEL_API __declspec(dllexport)
#else
#define QGL_MODEL_API __declspec(dllimport)
#endif

#ifndef QGL_CC
/*
 Use the x64 calling convention. Pass integral arguments in RCX, RDX, R8, R9.
 Do not use for functions that must cross the ABI.
 https://docs.microsoft.com/en-us/cpp/cpp/fastcall?view=vs-2019
 */
#define QGL_CC __fastcall
#endif

#ifndef QGL_VCC
/*
 Use a vector call for multiple floating point arguments or SIMD vectors.
 Useful for __m128 and __m256 values.
 https://docs.microsoft.com/en-us/cpp/cpp/vectorcall?view=vs-2019
 */
#define QGL_VCC __vectorcall
#endif

#ifndef QGL_API_CC
/*
 Default calling convention for factory methods that must cross the API.
 Does not support variadic arguments. 
 */
#define QGL_API_CC __stdcall
#endif