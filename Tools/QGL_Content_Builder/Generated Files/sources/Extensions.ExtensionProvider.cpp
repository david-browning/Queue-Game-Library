#include "pch.h"
#include "Extensions.ExtensionProvider.h"

namespace winrt::QGL_Content_Builder::Extensions::implementation
{
    bool ExtensionProvider::CheckSupport(QGL_Projection::ContentInfo const& info)
    {
        throw hresult_not_implemented();
    }

    hstring ExtensionProvider::GetExtensionForLoader(QGL_Projection::ContentInfo const& info)
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> ExtensionProvider::Extensions()
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> ExtensionProvider::ObservableResourceTypes()
    {
        throw hresult_not_implemented();
    }

    winrt::event_token ExtensionProvider::PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        throw hresult_not_implemented();
    }

    void ExtensionProvider::PropertyChanged(winrt::event_token const& token) noexcept
    {
        throw hresult_not_implemented();
    }
}
