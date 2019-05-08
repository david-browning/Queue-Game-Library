#pragma once

#include "Helpers.InverseBoolConverter.g.h"

namespace winrt::QGL_Content_Builder::Helpers::implementation
{
    struct InverseBoolConverter : InverseBoolConverterT<InverseBoolConverter>
    {
        InverseBoolConverter() = default;

        Windows::Foundation::IInspectable Convert(Windows::Foundation::IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const& targetType, Windows::Foundation::IInspectable const& parameter, hstring const& language);
        Windows::Foundation::IInspectable ConvertBack(Windows::Foundation::IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const& targetType, Windows::Foundation::IInspectable const& parameter, hstring const& language);
    };
}

namespace winrt::QGL_Content_Builder::Helpers::factory_implementation
{
    struct InverseBoolConverter : InverseBoolConverterT<InverseBoolConverter, implementation::InverseBoolConverter>
    {
    };
}
