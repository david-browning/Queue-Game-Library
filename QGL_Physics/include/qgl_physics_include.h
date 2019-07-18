#pragma once
#include <QGLModel.h>
#include <QGLMath.h>

#ifdef QGL_PHYSICS_EXPORTS
#define QGL_PHYSICS_API __declspec(dllexport)
#define QGL_PHYSICS_TEMPLATE
#else
#define QGL_PHYSICS_API __declspec(dllimport)
#define QGL_PHYSICS_TEMPLATE extern
#endif