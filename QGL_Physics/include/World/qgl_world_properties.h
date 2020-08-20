#pragma once
#include "include/qgl_physics_include.h"

namespace qgl::physics
{
   /*
    World properties.
    Commonly, the distance-units should be meters and time-steps should be
    seconds.
    */
   struct physical_world
   {
      /*
       Acceleration due to gravity. Measured in distance-units per
       time-step^2.
       The Y component should be negative.
       */
      DirectX::XMVECTOR Gravity;

      /*
       Measured in mass-units per distance-units^3
       */
      float AirDensity;
   };

}