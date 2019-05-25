#pragma once

#include "include/annotations.h"
#include "include/Memory/qgl_mem.h"
#include "include/qgl_platform_abstraction.h"

#ifdef QGL_MODEL_EXPORTS
#define QGL_MODEL_API __declspec(dllexport)
#else
#define QGL_MODEL_API __declspec(dllimport)
#endif