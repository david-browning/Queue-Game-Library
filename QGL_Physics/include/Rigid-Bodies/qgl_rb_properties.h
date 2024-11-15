#pragma once
#include "include/qgl_physics_include.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>

namespace qgl::physics::rigid_body
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

      DirectX::BoundingBox RestingAABB;

   };
}