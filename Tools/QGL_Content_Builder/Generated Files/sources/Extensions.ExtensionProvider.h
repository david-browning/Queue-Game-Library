#pragma once

#include "Extensions.ExtensionProvider.g.h"

namespace winrt::QGL_Content_Builder::Extensions::implementation
{
    struct ExtensionProvider : ExtensionProviderT<ExtensionProvider>
    {
        ExtensionProvider() = default;

        bool CheckSupport(QGL_Projection::ContentInfo const& info);
        hstring GetExtensionForLoader(QGL_Projection::ContentInfo const& info);
        Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> Extensions();
        Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> ObservableResourceTypes();
        winrt::event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;
    };
}

namespace winrt::QGL_Content_Builder::Extensions::factory_implementation
{
    struct ExtensionProvider : ExtensionProviderT<ExtensionProvider, implementation::ExtensionProvider>
    {
    };
}
