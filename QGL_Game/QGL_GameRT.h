#pragma once
#include "QGL_GameRT.g.h"

namespace winrt::QGL_Game::implementation
{
    struct QGL_GameRT : QGL_GameRTT<QGL_GameRT>
    {
        QGL_GameRT() = default;

    };
}
namespace winrt::QGL_Game::factory_implementation
{
    struct QGL_GameRT : QGL_GameRTT<QGL_GameRT, implementation::QGL_GameRT>
    {
    };
}
