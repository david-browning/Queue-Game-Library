#include "pch.h"
#include "Helpers.InverseBoolConverter.h"

namespace winrt::QGL_Content_Builder::Helpers::implementation
{
    Windows::Foundation::IInspectable InverseBoolConverter::Convert(Windows::Foundation::IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const& targetType, Windows::Foundation::IInspectable const& parameter, hstring const& language)
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IInspectable InverseBoolConverter::ConvertBack(Windows::Foundation::IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const& targetType, Windows::Foundation::IInspectable const& parameter, hstring const& language)
    {
        throw hresult_not_implemented();
    }
}
