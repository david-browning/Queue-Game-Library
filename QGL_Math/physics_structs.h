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
       Rigid body rotation, stored in a quaternion.
       */
      DirectX::XMVECTOR Rotation;

      DirectX::XMMATRIX XM_CALLCONV InertiaTensor();
   };

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

   void XM_CALLCONV integrate(DirectX::FXMVECTOR force,
                              DirectX::FXMVECTOR impact,
                              const rb_properties& props,
                              float dt,
                              rb_state& state)
   {
      using namespace DirectX;
      XMVECTOR acceleration = force / props.Mass;
      XMVECTOR armVector = impact - state.Position;
      XMVECTOR torque = XMVector3Cross(armVector, force);


      state.LastAcceleration = acceleration;
      state.Velocity += state.LastAcceleration * dt;
      state.Position += state.Velocity * dt;

      //XMVECTOR angularAcceleration = torque / state.InertiaTensor();

   }
}