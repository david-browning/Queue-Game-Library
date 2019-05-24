#pragma once

#include <QGLModel.h>
#include <winrt/base.h>
#include <filesystem>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Foundation.h>

#ifdef QGL_CONTENT_EXPORTS
#define QGL_CONTENT_API __declspec(dllexport)
#else
#define QGL_CONTENT_API __declspec(dllimport)
#endif