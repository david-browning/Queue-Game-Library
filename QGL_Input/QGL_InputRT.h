#pragma once
#include "QGL_InputRT.g.h"

namespace winrt::QGL_Input::implementation
{
    struct QGL_InputRT : QGL_InputRTT<QGL_InputRT>
    {
       QGL_InputRT() = default;
    };
}

namespace winrt::QGL_Input::factory_implementation
{
    struct QGL_InputRT : QGL_InputRTT<QGL_InputRT, implementation::QGL_InputRT>
    {
    };
}
