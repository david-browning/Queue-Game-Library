#pragma once
#include <QGLModel.h>
#include <QGLMath.h>
#include <QGLGraphics.h>

#include <Xinput.h>
#include <winrt/Windows.Gaming.Input.h>
#include <winrt/Windows.UI.Xaml.Controls.h>

#ifdef QGL_INPUT_EXPORTS
#define QGL_INPUT_API __declspec(dllexport)
#define QGL_INPUT_TEMPLATE
#else
#define QGL_INPUT_API __declspec(dllimport)
#define QGL_INPUT_TEMPLATE
#endif