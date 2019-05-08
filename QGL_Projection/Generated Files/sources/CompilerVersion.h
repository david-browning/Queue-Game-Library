#pragma once

#include "CompilerVersion.g.h"

namespace winrt::QGL_Projection::implementation
{
    struct CompilerVersion : CompilerVersionT<CompilerVersion>
    {
        CompilerVersion() = delete;
        CompilerVersion(uint32_t val);

        uint32_t Value();
        hstring ValueName();
        hstring ToString();

        static QGL_Projection::CompilerVersion Version_0_1();
        static QGL_Projection::CompilerVersion Version_0_2();
        static Windows::Foundation::Collections::IVector<QGL_Projection::CompilerVersion> CompilerVersions();
        static Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> ObservableCompilerVersions();
    };
}

namespace winrt::QGL_Projection::factory_implementation
{
    struct CompilerVersion : CompilerVersionT<CompilerVersion, implementation::CompilerVersion>
    {
    };
}
