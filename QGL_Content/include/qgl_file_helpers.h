#pragma once
#include "include/qgl_content_include.h"
#include <QGLPlatform.h>
#include <winrt/Windows.Storage.h>

namespace qgl::content
{
   /*
    Returns true if a file or folder contains the given attribute.
    The path must be absolute.
    Attribute: Can be any attribute described here:
    https://docs.microsoft.com/en-us/windows/desktop/FileIO/file-attribute-constants
    */
   template<DWORD Attribute>
   inline bool attribute_exists(const sys_str& absPath) noexcept
   {
      auto attr = GetFileAttributes(absPath.c_str());
      if (attr != INVALID_FILE_ATTRIBUTES)
      {
         return (attr & Attribute) == Attribute;
      }

      return false;
   }

   /*
    Returns true if "absPath" points to a file that exists.
    */
   inline bool file_exists(const sys_str& absPath)
   {
      auto attr = GetFileAttributes(absPath.c_str());
      return attr != INVALID_FILE_ATTRIBUTES &&
         !(attr & FILE_ATTRIBUTE_DIRECTORY);
   }

   /*
    Returns true if "absPath" points to a directory that exists.
    */
   inline bool dir_exists(const sys_str& absPath)
   {
      return attribute_exists<FILE_ATTRIBUTE_DIRECTORY>(absPath);
   }

   inline sys_str installed_path() noexcept
   {
      auto localFolder{ winrt::Windows::Storage::ApplicationData::Current().LocalFolder() };
      return sys_str{ localFolder.Path() };
   }

   inline void delete_file(const sys_str& absPath)
   {
      DeleteFileFromAppW(absPath.c_str());
   }
}