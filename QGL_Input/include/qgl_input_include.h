#pragma once
#include <cstdint>

#ifdef QGL_INPUT_EXPORTS
#define QGL_INPUT_API __declspec(dllexport)
#define QGL_INPUT_TEMPLATE
#else
#define QGL_INPUT_API __declspec(dllimport)
#define QGL_INPUT_TEMPLATE
#endif