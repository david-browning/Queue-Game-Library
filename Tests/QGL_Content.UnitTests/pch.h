//
// pch.h
//

#pragma once

#include <collection.h>
#include <ppltasks.h>
#include <QGLContent.h>

#include "UnitTestApp.xaml.h"
#include "CppUnitTest.h"

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
   template<> static std::wstring ToString<qgl::content::CONTENT_LOADER_IDS>
      (const qgl::content::CONTENT_LOADER_IDS& t)
   {
      RETURN_WIDE_STRING(t);
   }

   template<> static std::wstring ToString<qgl::content::RESOURCE_TYPES>
      (const qgl::content::RESOURCE_TYPES& t)
   {
      RETURN_WIDE_STRING(t);
   }
}
