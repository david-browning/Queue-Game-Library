#pragma once
#include "CompilerVersion.g.h"

namespace winrt::QGL_Projection::implementation
{
    struct CompilerVersion : CompilerVersionT<CompilerVersion>
    {
        CompilerVersion() = delete;
        CompilerVersion(uint32_t val);

        uint32_t Value();
        hstring ToString();
        hstring ValueName()
        {
           return ToString();
        }

        static QGL_Projection::CompilerVersion Version_0_1()
        {
           return v_0_1;
        }

        static QGL_Projection::CompilerVersion Version_0_2()
        {
           return v_0_2;
        }

        static Windows::Foundation::Collections::IVector<QGL_Projection::CompilerVersion> CompilerVersions();
        static Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> ObservableCompilerVersions();

       private:
       hstring m_name;
       uint32_t m_version;

       static QGL_Projection::CompilerVersion v_0_1;
       static QGL_Projection::CompilerVersion v_0_2;
       static Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> m_ObservableAllVersions;
       static Windows::Foundation::Collections::IVector<QGL_Projection::CompilerVersion> m_allVersions;
    };
}

namespace winrt::QGL_Projection::factory_implementation
{
    struct CompilerVersion : CompilerVersionT<CompilerVersion, implementation::CompilerVersion>
    {
    };
}
