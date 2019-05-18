#pragma once
#include "ContentLoader.g.h"

namespace winrt::QGL_Projection::implementation
{
    struct ContentLoader : ContentLoaderT<ContentLoader>
    {
        ContentLoader() = default;

        ContentLoader(uint16_t enumValue, hstring const& name);
        uint16_t Value();
        hstring ToString();
    };
}
namespace winrt::QGL_Projection::factory_implementation
{
    struct ContentLoader : ContentLoaderT<ContentLoader, implementation::ContentLoader>
    {
    };
}
