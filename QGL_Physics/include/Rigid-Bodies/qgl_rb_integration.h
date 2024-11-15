#pragma once
#include "include/qgl_physics_include.h"
#include "include/Rigid-Bodies/qgl_rb_properties.h"
#include "include/Rigid-Bodies/qgl_rb_state.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>

namespace qgl::physics::rigid_body
{
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

      XMVECTOR angularAcceleration = XMVector3Transform(torque,
                                                        state.InvInertiaTensor);

   }
}