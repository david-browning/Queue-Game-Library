#pragma once
#include "include/qgl_physics_include.h"
#include "include/Rigid-Bodies/qgl_rb_properties.h"

namespace qgl::physics::rigid_body
{
   #ifdef DEBUG
   class test_inertia_tensor
   {
      public:
      DirectX::XMMATRIX XM_CALLCONV operator()(rb_properties& rb)
      {

      }
   };

   #endif

    /*
    State information about a physical object. The members here are not
    measured in any particular units. Instead, the members reference time-steps
    and distance-units.
    Commonly, the distance-units should be meters and time-steps should be
    seconds.
    */
   template<class InertiaTensorFunctor>
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
       Rigid body rotation, stored in a quaternion.
       */
      DirectX::XMVECTOR Rotation;

      InertiaTensorFunctor InertiaTensor;
   };
}