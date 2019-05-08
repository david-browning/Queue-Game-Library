#pragma once

#include "ResourceType.g.h"

namespace winrt::QGL_Projection::implementation
{
    struct ResourceType : ResourceTypeT<ResourceType>
    {
        ResourceType() = delete;
        ResourceType(uint16_t val, hstring const& name, array_view<QGL_Projection::ContentLoader const> loaders);

        uint16_t Value();
        hstring ValueName();
        Windows::Foundation::Collections::IVector<QGL_Projection::ContentLoader> Loaders();
        Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> ObservableLoaders();
        hstring ToString();
    };
}

namespace winrt::QGL_Projection::factory_implementation
{
    struct ResourceType : ResourceTypeT<ResourceType, implementation::ResourceType>
    {
    };
}
