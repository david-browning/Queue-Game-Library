#include "pch.h"
#include "ResourceType.h"

namespace winrt::QGL_Projection::implementation
{
    ResourceType::ResourceType(uint16_t enumValue, hstring const& name)
    {
        throw hresult_not_implemented();
    }
    uint16_t ResourceType::Value()
    {
        throw hresult_not_implemented();
    }
    Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> ResourceType::SupportedLoaders()
    {
        throw hresult_not_implemented();
    }
    hstring ResourceType::ToString()
    {
        throw hresult_not_implemented();
    }
}
