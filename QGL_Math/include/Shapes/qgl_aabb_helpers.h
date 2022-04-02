#pragma once
#include "include/qgl_math_includes.h"

namespace qgl::math
{
   static constexpr size_t VERTS_PER_BOX = 8;

   /*
    Calculates an AABB from the smallest and largest distances from the
    origin.
    */
   extern "C" QGL_MATH_API void XM_CALLCONV aabb_from_minmax(
      DirectX::FXMVECTOR mins,
      DirectX::FXMVECTOR maxs,
      DirectX::BoundingBox* out_p);

   /*
    Calculates the 8 vertices in a bounding box and fills an array with
    the results.
    */
   extern "C" QGL_MATH_API void XM_CALLCONV aabb_verts(
      const DirectX::BoundingBox* aabb,
      DirectX::XMVECTOR verts[VERTS_PER_BOX]);
}
