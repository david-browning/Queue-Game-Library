#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Descriptors/qgl_vector_descriptor.h"

namespace qgl::descriptors
{
#pragma pack(push, 1)
   struct camera_descriptor final
   {
      constexpr camera_descriptor()
      {

      }

      constexpr camera_descriptor(const camera_descriptor&) = default;

      constexpr camera_descriptor(camera_descriptor&&) noexcept = default;

      ~camera_descriptor() noexcept = default;

      friend void swap(camera_descriptor& l, camera_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.far_plane, r.far_plane);
         swap(l.fov, r.fov);
         swap(l.near_plane, r.near_plane);
         swap(l.look, r.look);
         swap(l.up, r.up);
         swap(l.position, r.position);
      }

      camera_descriptor& operator=(camera_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Spawn location
       */
      vector_descriptor position;

      /*
       Spawn up
       */
      vector_descriptor up;

      /*
       Spawn look at.
       */
      vector_descriptor look;

      /*
       Field of view.
       */
      math::rational<int32_t> fov{ 1, 1 };

      /*
       How close things can be to be rendered. Anything less than this
       is not rendered.
       */
      math::rational<int32_t> near_plane{ 1, 1 };

      /*
       How far away things can be rendered. Anything greater than this is not
       rendered.
       */
      math::rational<int32_t> far_plane{ 10000 , 1 };
   };
#pragma pack(pop)
}