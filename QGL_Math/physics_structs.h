#pragma once
#include "include/qgl_math_includes.h"

namespace qgl::math
{
   /*
    Constant properties for a rigid body. The members here are not measured in
    any particular units.
    Commonly, the mass-units should be kilograms.
    The coefficient of restitution is unit-less, but must be between 0 and 1.0
    inclusive.
    */
   struct rb_properties
   {
      /*
       Mass of the rigid body, measured in mass-units.
       */
      float Mass;

      /*
       Coefficient of restitution. This is unit-less and must be between 0 and
       1.0 inclusive.
       */
      float CoR;
   };

   /*
    State information about a physical object. The members here are not
    measured in any particular units. Instead, the members reference time-steps
    and distance-units.
    Commonly, the distance-units should be meters and time-steps should be
    seconds.
    */
   struct rb_state
   {
      /*
       Integrated center of the rigid body at the current time-step.
       Measured in distance-units.
       */
      DirectX::XMVECTOR Position;

      /*
       Integrated rigid body velocity at the current time-step.
       Measured in distance-units per time-step.
       */
      DirectX::XMVECTOR Velocity;

      /*
       Integrated acceleration of the rigid body from the last time-step.
       Measured in distance-units per time-step^2
       */
      DirectX::XMVECTOR LastAcceleration;

      /*
       Integrated acceleration of the rigid body at the current time-step.
       Measured in distance-units per time-step^2
       */
      DirectX::XMVECTOR Acceleration;

      /*
       Rigid body rotation, stored in a quaternion.
       */
      DirectX::XMVECTOR Rotation;
   };
}