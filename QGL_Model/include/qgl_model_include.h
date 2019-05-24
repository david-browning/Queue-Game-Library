#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "include/annotations.h"
#include <Windows.h>
#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>

#ifdef QGL_MODEL_EXPORTS
#define QGL_MODEL_API __declspec(dllexport)
#else
#define QGL_MODEL_API __declspec(dllimport)
#endif