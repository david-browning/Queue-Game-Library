#pragma once
#include "CompilerVersion.g.h"
#include <QGLModel.h>

using namespace winrt::Windows::Foundation::Collections;

namespace winrt::QGL_Projection::implementation
{
   struct CompilerVersion : CompilerVersionT<CompilerVersion>
   {
      CompilerVersion() = default;

      CompilerVersion(uint32_t val, hstring const& str);

      uint32_t Value();

      hstring ToString();

      static QGL_Projection::CompilerVersion Version_0_1();

      static QGL_Projection::CompilerVersion Version_0_2();

      static QGL_Projection::CompilerVersion VersionLatest();
      
      static IVector<QGL_Projection::CompilerVersion> CompilerVersions();

      private:
      qgl::qgl_version_t m_version;
      winrt::hstring m_name;
   };
}
namespace winrt::QGL_Projection::factory_implementation
{
   struct CompilerVersion : CompilerVersionT<CompilerVersion,
      implementation::CompilerVersion>
   {
   };
}
