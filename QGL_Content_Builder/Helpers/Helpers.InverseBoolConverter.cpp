#include "pch.h"
#include "Helpers.InverseBoolConverter.h"
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml;

namespace winrt::QGL_Content_Builder::Helpers::implementation
{
    IInspectable InverseBoolConverter::Convert(IInspectable const& value, 
                                               Interop::TypeName const&,
                                               IInspectable const&, 
                                               hstring const&)
    {
       bool b = winrt::unbox_value<bool>(value);
       return winrt::box_value(!b);
    }

    IInspectable InverseBoolConverter::ConvertBack(IInspectable const& value, 
                                                   Interop::TypeName const&, 
                                                   IInspectable const&, 
                                                   hstring const&)
    {
       bool b = winrt::unbox_value<bool>(value);
       return winrt::box_value(!b);
    }
}
