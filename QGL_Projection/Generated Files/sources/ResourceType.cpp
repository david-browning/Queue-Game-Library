#include "pch.h"
#include "ResourceType.h"

namespace winrt::QGL_Projection::implementation
{
    ResourceType::ResourceType(uint16_t val, hstring const& name, array_view<QGL_Projection::ContentLoader const> loaders)
    {
        throw hresult_not_implemented();
    }

    uint16_t ResourceType::Value()
    {
        throw hresult_not_implemented();
    }

    hstring ResourceType::ValueName()
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::Collections::IVector<QGL_Projection::ContentLoader> ResourceType::Loaders()
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> ResourceType::ObservableLoaders()
    {
        throw hresult_not_implemented();
    }

    hstring ResourceType::ToString()
    {
        throw hresult_not_implemented();
    }
}
