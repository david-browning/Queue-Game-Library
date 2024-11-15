#pragma once

#include <collection.h>
#include <ppltasks.h>
#include "CppUnitTest.h"
#include "UnitTestApp.xaml.h"
#include <QGLModel.h>
#include <QGLStruct.h>

template<typename T>
inline std::wstring TemplateToString(const T& x)
{
   std::wstringstream ret;
   ret << x;
   return ret.str();
}

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
   template<>
   inline std::wstring ToString<qgl::guid>(const qgl::guid& g)
   {
      return g.str<wchar_t>();
   }

   template<>
   inline std::wstring ToString<qgl::hw_types>(const qgl::hw_types& i)
   {
      std::wstringstream ret;
      ret << std::hex << (uint32_t)i;
      return ret.str();
   }
}