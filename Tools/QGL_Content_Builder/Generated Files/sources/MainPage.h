#pragma once

#include "MainPage.g.h"

namespace winrt::QGL_Content_Builder::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage() = default;

        int32_t MyProperty();
        void MyProperty(int32_t value);
    };
}

namespace winrt::QGL_Content_Builder::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
