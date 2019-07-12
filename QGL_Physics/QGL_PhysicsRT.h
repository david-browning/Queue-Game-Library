#pragma once
#include "QGL_PhysicsRT.g.h"

namespace winrt::QGL_Physics::implementation
{
   struct QGL_PhysicsRT : QGL_PhysicsRTT<QGL_PhysicsRT>
   {
      QGL_PhysicsRT() = default;

   };
}
namespace winrt::QGL_Physics::factory_implementation
{
   struct QGL_PhysicsRT : QGL_PhysicsRTT<QGL_PhysicsRT, implementation::QGL_PhysicsRT>
   {
   };
}
