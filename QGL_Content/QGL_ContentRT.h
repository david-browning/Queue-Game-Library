#pragma once

#include "QGL_ContentRT.g.h"

namespace winrt::QGL_Content::implementation
{
   struct QGL_ContentRT : QGL_ContentRTT<QGL_ContentRT>
   {
      QGL_ContentRT() = default;

   };
}

namespace winrt::QGL_Content::factory_implementation
{
   struct QGL_ContentRT : QGL_ContentRTT<QGL_ContentRT, implementation::QGL_ContentRT>
   {
   };
}
