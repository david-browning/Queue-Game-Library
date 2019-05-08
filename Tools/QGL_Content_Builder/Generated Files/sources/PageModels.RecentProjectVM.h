#pragma once

#include "PageModels.RecentProjectVM.g.h"

namespace winrt::QGL_Content_Builder::PageModels::implementation
{
    struct RecentProjectVM : RecentProjectVMT<RecentProjectVM>
    {
        RecentProjectVM() = delete;
        RecentProjectVM(hstring const& filePath, QGL_Projection::ContentInfo const& info);

        hstring ContentName();
        hstring Path();
        winrt::event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;
    };
}

namespace winrt::QGL_Content_Builder::PageModels::factory_implementation
{
    struct RecentProjectVM : RecentProjectVMT<RecentProjectVM, implementation::RecentProjectVM>
    {
    };
}
