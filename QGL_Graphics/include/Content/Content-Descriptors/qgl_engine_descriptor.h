#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::descriptors
{
#pragma pack(push, 1)
   struct engine_descriptor final
   {
      constexpr engine_descriptor()
      {
      }

      friend void swap(engine_descriptor& l, engine_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.width, r.width);
         swap(l.height, r.height);
         swap(l.refresh, r.refresh);
         swap(l.buffers, r.buffers);
         swap(l.fullscreen, r.fullscreen);
         swap(l.tearing, r.tearing);
         swap(l.high_resolution, r.high_resolution);
         swap(l.stereo, r.stereo);
         swap(l.interlacing, r.interlacing);
         swap(l.pref_adapter_id, r.pref_adapter_id);
      }

      engine_descriptor& operator=(engine_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      uint32_t pref_adapter_id = 0;

      /*
       The horizontal resolution in pixels.
       */
      uint32_t width = 800;

      /*
       The vertical resolution in pixels.
       */
      uint32_t height = 600;

      /*
       Monitor refresh rate in hertz.
       */
      uint32_t refresh = 30;

      /*
       Number of render target back buffers the engine will use.
       */
      uint8_t buffers = 3;

      /*
       True if the engine should be in full screen.
       */
      uint8_t fullscreen = 0;

      /*
       True if the engine should allow an uncapped render rate.
       */
      uint8_t tearing = 1;

      /*
       True if the engine uses high resolution rendering.
       */
      uint8_t high_resolution = 1;

      uint8_t stereo = 0;

      uint8_t interlacing = 0;
   };
#pragma pack(pop)
}