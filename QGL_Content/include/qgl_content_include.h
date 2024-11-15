#pragma once
#include <Unknwn.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <future>

#ifdef QGL_CONTENT_EXPORTS
#define QGL_CONTENT_API __declspec(dllexport)
#define QGL_CONTENT_TEMPLATE
#else
#define QGL_CONTENT_API __declspec(dllimport)
#define QGL_CONTENT_TEMPLATE extern
#endif

namespace qgl::content
{
   using file_buffer_t = typename std::vector<std::byte>;

   /*
    A promise used when a file operation will read a buffer.
    */
   using read_promise = typename std::promise<file_buffer_t>;

   /*
    A promise used when a file operation will return the number of bytes 
    promised.
    */
   using count_promise = typename std::promise<size_t>;

   enum class file_open_modes
   {
      read,
      write,
      readwrite,
   };
}