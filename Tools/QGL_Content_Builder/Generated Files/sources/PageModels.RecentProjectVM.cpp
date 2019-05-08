#include "pch.h"
#include "PageModels.RecentProjectVM.h"

namespace winrt::QGL_Content_Builder::PageModels::implementation
{
    RecentProjectVM::RecentProjectVM(hstring const& filePath, QGL_Projection::ContentInfo const& info)
    {
        throw hresult_not_implemented();
    }

    hstring RecentProjectVM::ContentName()
    {
        throw hresult_not_implemented();
    }

    hstring RecentProjectVM::Path()
    {
        throw hresult_not_implemented();
    }

    winrt::event_token RecentProjectVM::PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        throw hresult_not_implemented();
    }

    void RecentProjectVM::PropertyChanged(winrt::event_token const& token) noexcept
    {
        throw hresult_not_implemented();
    }
}
