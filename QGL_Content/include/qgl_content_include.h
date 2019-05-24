#pragma once

#include <QGLModel.h>
#include <winrt/base.h>
#include <filesystem>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Foundation.h>

#ifdef QGL_CONTENT_EXPORTS
#define QGL_CONTENT_API __declspec(dllexport)
#define QGL_CONTENT_TEMPLATE
#else
#define QGL_CONTENT_API __declspec(dllimport)
#define QGL_CONTENT_TEMPLATE extern
#endif

#ifndef WINRT_HSTRING_EXPORT
#define WINRT_HSTRING_EXPORT
struct QGL_CONTENT_API winrt::hstring;
#endif

#ifdef WIN32
using file_string = winrt::hstring;
#else
using file_string = std::string;
#endif

QGL_CONTENT_TEMPLATE template class QGL_CONTENT_API std::allocator<file_string>;
