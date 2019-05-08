#include "pch.h"
#include "PageModels.HomePageVM.h"

namespace winrt::QGL_Content_Builder::PageModels::implementation
{
    HomePageVM::HomePageVM(QGL_Content_Builder::Extensions::ExtensionProvider const& extensionProvider)
    {
        throw hresult_not_implemented();
    }

    bool HomePageVM::IsProjectLoading()
    {
        throw hresult_not_implemented();
    }

    bool HomePageVM::IsProjectSaving()
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> HomePageVM::RecentFiles()
    {
        throw hresult_not_implemented();
    }

    void HomePageVM::ClearRecentFiles()
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IAsyncAction HomePageVM::PopulateRecentFiles()
    {
        throw hresult_not_implemented();
    }

    winrt::event_token HomePageVM::PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        throw hresult_not_implemented();
    }

    void HomePageVM::PropertyChanged(winrt::event_token const& token) noexcept
    {
        throw hresult_not_implemented();
    }
}
