//
// pch.h
//

#pragma once

#include <collection.h>
#include <ppltasks.h>
#include "CppUnitTest.h"
#include "UnitTestApp.xaml.h"
#include <QGLContent.h>

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
   template<>
   inline std::wstring ToString<std::byte>(const std::byte& b)
   {
      std::wstringstream ret;
      ret << std::hex << (uint8_t)b;
      return ret.str();
   }
}