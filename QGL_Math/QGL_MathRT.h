#pragma once
#include "QGL_MathRT.g.h"

namespace winrt::QGL_Math::implementation
{
    struct QGL_MathRT : QGL_MathRTT<QGL_MathRT>
    {
        QGL_MathRT() = default;
    };
}
namespace winrt::QGL_Math::factory_implementation
{
    struct QGL_MathRT : QGL_MathRTT<QGL_MathRT, implementation::QGL_MathRT>
    {
    };
}
