#include "pch.h"
#include "CompilerVersion.h"
#include <QGLModel.h>

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;

namespace winrt::QGL_Projection::implementation
{
   QGL_Projection::CompilerVersion CompilerVersion::v_0_1{ qgl::QGL_VERSION_0_1_WIN };
   QGL_Projection::CompilerVersion CompilerVersion::v_0_2{ qgl::QGL_VERSION_0_2_WIN };
   
   IObservableVector<IInspectable> CompilerVersion::m_ObservableAllVersions =
      winrt::single_threaded_observable_vector<IInspectable>(
         {
            CompilerVersion::v_0_1,
            CompilerVersion::v_0_2
         });

   IVector<QGL_Projection::CompilerVersion> CompilerVersion::m_allVersions =
      winrt::single_threaded_vector<QGL_Projection::CompilerVersion>(
         {
            CompilerVersion::v_0_1,
            CompilerVersion::v_0_2
         });

   CompilerVersion::CompilerVersion(uint32_t val) :
      m_version(val)
   {
      std::wstringstream nameFormatStream;
      nameFormatStream << qgl::version_major(m_version) << L'.' <<
         qgl::version_minor(m_version);
      m_name = nameFormatStream.str();
   }

   uint32_t CompilerVersion::Value()
   {
      return m_version;
   }

   hstring CompilerVersion::ToString()
   {
      return m_name;
   }

   Windows::Foundation::Collections::IVector<QGL_Projection::CompilerVersion> CompilerVersion::CompilerVersions()
   {
      return m_allVersions;
   }

   Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> CompilerVersion::ObservableCompilerVersions()
   {
      return m_ObservableAllVersions;
   }
}
