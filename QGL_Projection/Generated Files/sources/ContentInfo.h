#pragma once

#include "ContentInfo.g.h"

namespace winrt::QGL_Projection::implementation
{
    struct ContentInfo : ContentInfoT<ContentInfo>
    {
        ContentInfo() = default;

        bool ContentVisible();
        void ContentVisible(bool value);
        bool ContentObeyPhysics();
        void ContentObeyPhysics(bool value);
        hstring ContentName();
        void ContentName(hstring const& value);
        QGL_Projection::ResourceType ResourceType();
        void ResourceType(QGL_Projection::ResourceType const& value);
        QGL_Projection::ContentLoader ContentLoader();
        void ContentLoader(QGL_Projection::ContentLoader const& value);
        QGL_Projection::CompilerVersion CompilerVersion();
        void CompilerVersion(QGL_Projection::CompilerVersion const& value);
        winrt::guid Guid();
        void Guid(winrt::guid const& g);
        winrt::event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

        static void WriteContentInfoToBuffer(QGL_Projection::ContentInfo const& info, Windows::Storage::Streams::DataWriter const& r);
        static QGL_Projection::ContentInfo ReadContentInfoFromBuffer(Windows::Storage::Streams::DataReader const& r, Windows::Foundation::Collections::IVector<QGL_Projection::ResourceType> const& supportedResourceTypes);
    };
}

namespace winrt::QGL_Projection::factory_implementation
{
    struct ContentInfo : ContentInfoT<ContentInfo, implementation::ContentInfo>
    {
    };
}
