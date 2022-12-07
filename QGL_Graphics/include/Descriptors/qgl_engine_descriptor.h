#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::descriptors
{
#pragma pack(push, 1)
   struct engine_descriptor final
   {
      constexpr engine_descriptor()
      {
      }

      constexpr engine_descriptor(const engine_descriptor&) = default;

      constexpr engine_descriptor(engine_descriptor&&) noexcept = default;

      ~engine_descriptor() noexcept = default;

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
         swap(l.interlacing, r.interlacing);
         swap(l.pref_adapter_id, r.pref_adapter_id);
         swap(l.text_mode, r.text_mode);
         swap(l.reserved1, r.reserved1);
         swap(l.console, r.console);
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
      qgl::graphics::physp_t width = 800;

      /*
       The vertical resolution in pixels.
       */
      qgl::graphics::physp_t height = 600;

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

      uint8_t interlacing = 0;

      /*
       Represents a method of rendering glyphs.
       You may not want to set it to any of these or the text quality may be 
       low:
       * DWRITE_RENDERING_MODE_ALIASED
       * DWRITE_RENDERING_MODE_OUTLINE
       https://docs.microsoft.com/en-us/windows/win32/api/d2d1/ne-d2d1-d2d1_text_antialias_mode
       */
      uint8_t text_mode = DWRITE_RENDERING_MODE::DWRITE_RENDERING_MODE_DEFAULT;

      /*
       True if the graphics devices should write to a console.
       */
      uint8_t console = 0;
      uint8_t reserved1 = 0;
   };
#pragma pack(pop)
}