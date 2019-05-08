#pragma once

#include "QGL_ModelRT.g.h"

namespace winrt::QGL_Model::implementation
{
    struct QGL_ModelRT : QGL_ModelRTT<QGL_ModelRT>
    {
        QGL_ModelRT() = default;

    };
}

namespace winrt::QGL_Model::factory_implementation
{
    struct QGL_ModelRT : QGL_ModelRTT<QGL_ModelRT, implementation::QGL_ModelRT>
    {
    };
}
