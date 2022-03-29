#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::descriptors
{
#pragma pack(push, 1)
   struct camera_descriptor final
   {
      constexpr camera_descriptor()
      {

      }

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
      fixed_buffer<math::rational<int32_t>, 4> position;

      /*
       Spawn up
       */
      fixed_buffer<math::rational<int32_t>, 4> up;

      /*
       Spawn look at.
       */
      fixed_buffer<math::rational<int32_t>, 4> look;

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