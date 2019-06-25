﻿#pragma once
#include <collection.h>
#include <ppltasks.h>
#include <QGLMath.h>

#include "UnitTestApp.xaml.h"
#include "CPPUnitTest.h"

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
   template<> static std::wstring ToString<qgl::math::rational<int>>
      (const qgl::math::rational<int>& r)
   {
      std::wstringstream ws;
      ws << r.numerator() << '/' << r.denominator();
      return ws.str();
   }
}