//
// pch.h
//

#pragma once

#include <collection.h>
#include <ppltasks.h>
#include "CppUnitTest.h"
#include "UnitTestApp.xaml.h"
#include <QGLModel.h>

template<typename T>
static std::wstring TemplateToString(const T& x)
{
   std::wstringstream ret;
   ret << x;
   return ret.str();
}

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
   template<>
   static std::wstring ToString<qgl::guid>(const qgl::guid& g)
   {
      return g.str<wchar_t>();
   }

   template<>
   static std::wstring ToString<uint16_t>(const uint16_t& i)
   {
      return TemplateToString(i);
   }

   template<>
   static std::wstring ToString<qgl::hw_types>(const qgl::hw_types& i)
   {
      std::wstringstream ret;
      ret << std::hex << (uint32_t)i;
      return ret.str();
   }
}