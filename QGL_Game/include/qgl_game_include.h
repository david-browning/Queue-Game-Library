#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winrt/base.h>

#include <QGLModel.h>
#include <QGLMath.h>
#include <QGLPhysics.h>
#include <QGLGraphics.h>
#include <QGLInput.h>


#ifdef QGL_GAME_EXPORTS
#define QGL_GAME_API __declspec(dllexport)
#define QGL_GAME_TEMPLATE
#else
#define QGL_GAME_API __declspec(dllimport)
#define QGL_GAME_TEMPLATE extern
#endif