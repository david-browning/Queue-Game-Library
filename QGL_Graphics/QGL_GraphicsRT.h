#pragma once

#include "QGL_GraphicsRT.g.h"

namespace winrt::QGL_Graphics::implementation
{
    struct QGL_GraphicsRT : QGL_GraphicsRTT<QGL_GraphicsRT>
    {
        QGL_GraphicsRT() = default;

    };
}

namespace winrt::QGL_Graphics::factory_implementation
{
    struct QGL_GraphicsRT : QGL_GraphicsRTT<QGL_GraphicsRT, implementation::QGL_GraphicsRT>
    {
    };
}
