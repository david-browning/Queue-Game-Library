#include "pch.h"
#include "CompilerVersion.h"

namespace winrt::QGL_Projection::implementation
{
   CompilerVersion::CompilerVersion(uint32_t val, hstring const& str) :
      m_version(val),
      m_name(str)
   {

   }

   uint32_t CompilerVersion::Value()
   {
      return m_version;
   }

   hstring CompilerVersion::ToString()
   {
      return m_name;
   }

   QGL_Projection::CompilerVersion CompilerVersion::Version_0_1()
   {
      return QGL_Projection::CompilerVersion(qgl::QGL_VERSION_0_1_WIN,
                                             L"0.1");
   }

   QGL_Projection::CompilerVersion CompilerVersion::Version_0_2()
   {
      return QGL_Projection::CompilerVersion(qgl::QGL_VERSION_0_2_WIN,
                                             L"0.2");
   }

   IVector<QGL_Projection::CompilerVersion> CompilerVersion::CompilerVersions()
   {
      return winrt::single_threaded_vector<QGL_Projection::CompilerVersion>(
         {
            CompilerVersion::Version_0_1(),
            CompilerVersion::Version_0_2()
         });
   }
}
