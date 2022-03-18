#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::descriptors
{
#pragma pack(push, 1)
   struct engine_descriptor final
   {
      constexpr engine_descriptor() :
         flags(0),
         pref_adapter_id(0),
         width(640),
         height(480),
         refresh(60),
         buffers(3),
         fullscreen(false),
         tearing(false),
         high_resolution(true),
         stereo(false),
         hdr(false),
         interlacing(true)
      {
      }

      engine_descriptor(const engine_descriptor&) = default;

      engine_descriptor(engine_descriptor&&) = default;

      ~engine_descriptor() noexcept = default;

      friend void swap(engine_descriptor& l, engine_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.flags, r.flags);
         swap(l.width, r.width);
         swap(l.height, r.height);
         swap(l.refresh, r.refresh);
         swap(l.buffers, r.buffers);
         swap(l.fullscreen, r.fullscreen);
         swap(l.tearing, r.tearing);
         swap(l.high_resolution, r.high_resolution);
         swap(l.stereo, r.stereo);
         swap(l.hdr, r.hdr);
         swap(l.interlacing, r.interlacing);
         swap(l.pref_adapter_id, r.pref_adapter_id);
      }

      engine_descriptor& operator=(engine_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      uint32_t flags;

      uint32_t pref_adapter_id;

      /*
       The horizontal resolution in pixels.
       */
      uint32_t width;

      /*
       The vertical resolution in pixels.
       */
      uint32_t height;

      /*
       Monitor refresh rate in hertz.
       */
      uint32_t refresh;

      /*
       Number of render target back buffers the engine will use.
       */
      uint8_t buffers;

      /*
       True if the engine should be in full screen.
       */
      uint8_t fullscreen;

      /*
       True if the engine should allow an uncapped render rate.
       */
      uint8_t tearing;

      /*
       True if the engine uses high resolution rendering.
       */
      uint8_t high_resolution;

      uint8_t stereo;

      uint8_t hdr;

      uint8_t interlacing;
   };
#pragma pack(pop)
}