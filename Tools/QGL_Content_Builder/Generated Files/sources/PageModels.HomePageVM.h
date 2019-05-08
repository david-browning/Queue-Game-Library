#pragma once

#include "PageModels.HomePageVM.g.h"

namespace winrt::QGL_Content_Builder::PageModels::implementation
{
    struct HomePageVM : HomePageVMT<HomePageVM>
    {
        HomePageVM() = delete;
        HomePageVM(QGL_Content_Builder::Extensions::ExtensionProvider const& extensionProvider);

        bool IsProjectLoading();
        bool IsProjectSaving();
        Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> RecentFiles();
        void ClearRecentFiles();
        Windows::Foundation::IAsyncAction PopulateRecentFiles();
        winrt::event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;
    };
}

namespace winrt::QGL_Content_Builder::PageModels::factory_implementation
{
    struct HomePageVM : HomePageVMT<HomePageVM, implementation::HomePageVM>
    {
    };
}
