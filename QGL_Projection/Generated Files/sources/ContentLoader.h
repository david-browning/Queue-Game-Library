#pragma once

#include "ContentLoader.g.h"

namespace winrt::QGL_Projection::implementation
{
    struct ContentLoader : ContentLoaderT<ContentLoader>
    {
        ContentLoader() = delete;
        ContentLoader(uint16_t val, hstring const& name);

        uint16_t Value();
        hstring ValueName();
        hstring ToString();
    };
}

namespace winrt::QGL_Projection::factory_implementation
{
    struct ContentLoader : ContentLoaderT<ContentLoader, implementation::ContentLoader>
    {
    };
}
