#pragma once
#include "CompilerVersion.g.h"

namespace winrt::QGL_Projection::implementation
{
    struct CompilerVersion : CompilerVersionT<CompilerVersion>
    {
        CompilerVersion() = default;

        CompilerVersion(uint32_t val, hstring const& str);
        uint32_t Value();
        hstring ToString();
    };
}
namespace winrt::QGL_Projection::factory_implementation
{
    struct CompilerVersion : CompilerVersionT<CompilerVersion, implementation::CompilerVersion>
    {
    };
}
