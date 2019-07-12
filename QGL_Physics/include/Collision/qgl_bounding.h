#pragma once
#include "include/qgl_physics_include.h"

namespace qgl::physics
{
   DirectX::BoundingBox bound()
   {
      //Keep track of the minimum and maximum distance for the 6 planes.


      //For each vertex in the model:
      {
         //Calculate the plane equation for the vertex.
         //Do this 6 times: For +x, -x, +y, -y, +z, -z

      }

      return DirectX::BoundingBox();
   }
}