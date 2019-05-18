#pragma once
#include "ResourceType.g.h"

namespace winrt::QGL_Projection::implementation
{
    struct ResourceType : ResourceTypeT<ResourceType>
    {
        ResourceType() = default;

        ResourceType(uint16_t enumValue, hstring const& name);
        uint16_t Value();
        Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> SupportedLoaders();
        hstring ToString();
    };
}
namespace winrt::QGL_Projection::factory_implementation
{
    struct ResourceType : ResourceTypeT<ResourceType, implementation::ResourceType>
    {
    };
}
