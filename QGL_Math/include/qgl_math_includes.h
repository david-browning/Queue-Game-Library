#pragma once
#define _USE_MATH_DEFINES

#include <cstdint>
#include <math.h>

#ifdef QGL_MATH_EXPORTS
#define QGL_MATH_API __declspec(dllexport)
#define QGL_MATH_TEMPLATE
#else
#define QGL_MATH_API __declspec(dllimport)
#define QGL_MATH_TEMPLATE extern
#endif

