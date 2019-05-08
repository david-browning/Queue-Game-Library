#pragma once
#include "ContentLoader.g.h"
#include <QGLContent.h>

namespace winrt::QGL_Projection::implementation
{
    struct ContentLoader : ContentLoaderT<ContentLoader>
    {
        ContentLoader() = delete;
        ContentLoader(uint16_t val, hstring const& name);

        uint16_t Value();
        hstring ValueName()
        {
           return ToString();
        }
        hstring ToString();

       private:
       hstring m_name;
       qgl::content::CONTENT_LOADER_IDS m_id;
    };
}

namespace winrt::QGL_Projection::factory_implementation
{
    struct ContentLoader : ContentLoaderT<ContentLoader, implementation::ContentLoader>
    {
    };
}
