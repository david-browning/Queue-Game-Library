#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content
{
   namespace buffers
   {
      struct GRAPHICS_CONFIG_BUFFER;
   }

   class QGL_CONTENT_API graphics_config : public content_item
   {
      public:
      /*
       Constructor.
       */
      graphics_config(const buffers::GRAPHICS_CONFIG_BUFFER* b,
                      const wchar_t* name,
                      content_id id);

      /*
       Copy constructor.
       */
      graphics_config(const graphics_config&) = default;

      /*
       Move constructor.
       */
      graphics_config(graphics_config&&) = default;

      /*
       Destructor
       */
      virtual ~graphics_config() noexcept = default;

      /*
       Returns the horizontal resolution in pixels.
      */
      UINT width() const noexcept;

      /*
       Returns the vertical resolution in pixels.
       */
      UINT height() const noexcept;

      /*
       Returns the monitor refresh rate in hertz.
       */
      UINT refresh() const noexcept;

      /*
       Returns the number of render target back buffers the engine will use.
       */
      UINT buffers() const noexcept;

      /*
       Returns true if the engine should be in full screen.
       */
      bool full_screen() const noexcept;
      /*
       Returns true if the engine should allow an uncapped render rate.
       */
      bool tearing() const noexcept;

      /*
       Returns true if the engine uses high resolution rendering.
       */
      bool high_resolution() const noexcept;

      bool stereo() const noexcept;

      bool hdr() const noexcept;

      bool interlacing() const noexcept;

      UINT adapter_devID() const noexcept;

      private:
      buffers::GRAPHICS_CONFIG_BUFFER m_buffer;
   };
}