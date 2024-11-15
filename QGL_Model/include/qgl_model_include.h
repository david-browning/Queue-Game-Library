#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <cstdint>
#include <cmath>
#include <memory>
#include <string>
#include <vector>

#if !defined(NDEBUG) || defined(_DEBUG) || defined(DEBUG)

#ifndef _DEBUG
#define _DEBUG
#endif

#ifndef DEBUG
#define DEBUG
#endif

#endif


#ifdef QGL_MODEL_EXPORTS
#define QGL_MODEL_API __declspec(dllexport)
#else
#define QGL_MODEL_API __declspec(dllimport)
#endif

#ifdef DEBUG
#define NOEXCEPT_DEBUG
#else
#define NOEXCEPT_DEBUG noexcept
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