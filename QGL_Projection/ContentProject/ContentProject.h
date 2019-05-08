#pragma once

#include "ContentProject.g.h"

namespace winrt::QGL_Projection::implementation
{
    struct ContentProject : ContentProjectT<ContentProject>
    {
        ContentProject();

        QGL_Projection::ContentInfo ContentInfo();
        void ContentInfo(QGL_Projection::ContentInfo const& value);
        
        Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> ObservableEntries();
        void ObservableEntries(Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> const& value);

        Windows::Foundation::IAsyncOperation<bool> LoadFromFileAsync(Windows::Storage::StorageFile const f,
                                                            Windows::Foundation::Collections::IVector<ResourceType> const& supported);
        Windows::Foundation::IAsyncOperation<bool> SaveToFileAsync(Windows::Storage::StorageFile const f);

        winrt::event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

       private:
       QGL_Projection::ContentInfo m_info;
       Windows::Foundation::Collections::IObservableVector<IInspectable> m_observableEntries;
       winrt::event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
    };
}

namespace winrt::QGL_Projection::factory_implementation
{
    struct ContentProject : ContentProjectT<ContentProject, implementation::ContentProject>
    {
    };
}
