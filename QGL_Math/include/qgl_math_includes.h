#pragma once

#include <QGLModel.h>
#include <cmath>
#include <limits>
#include <numeric>
#include <winrt/base.h>

#include <DirectXMath.h>
#include <DirectXCollision.h>

#ifdef QGL_MATH_EXPORTS
#define QGL_MATH_API __declspec(dllexport)
#define QGL_MATH_TEMPLATE
#else
#define QGL_MATH_API __declspec(dllimport)
#define QGL_MATH_TEMPLATE extern
#endif

