#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics
{
   class graphics_device;
   struct GRAPHICS_CONFIG_BUFFER;

   class LIB_EXPORT graphic_options
   {
      public:
      graphic_options(const graphics_device& dev);

      graphic_options(const graphic_options& r);

      graphic_options(graphic_options&& r);

      virtual ~graphic_options() = default;

      /*
       Returns true if stereo is supported.
       TODO: https://docs.microsoft.com/en-us/windows/desktop/direct3ddxgi/stereo-rendering-stereo-status-notifying
       */
      inline bool support_3d() const
      {
         return m_3d;
      }

      /*
       Returns true if Vsync can be turned off.
       */
      inline bool support_tearing() const
      {
         return m_tearing;
      }

      inline bool support_hdr() const
      {
         return m_hdr;
      }

      /*
       Returns true if the graphics config is supported.
       When compiled in debug mode, this throws an exception if the graphics config has an
       unsupported value set.
       */
      bool valid(const GRAPHICS_CONFIG_BUFFER& config) const;

      friend void swap(graphic_options& first, graphic_options& second) noexcept
      {
         using std::swap;
         swap(first.m_desc, second.m_desc);
         swap(first.m_modes, second.m_modes);
         swap(first.m_3d, second.m_3d);
         swap(first.m_tearing, second.m_tearing);
         swap(first.m_hdr, second.m_hdr);
      }

      graphic_options& operator=(graphic_options r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:
      /*
       Populates the member variables.
       */
      void p_populate(const graphics_device& dev);

      #pragma warning(push)
      #pragma warning(disable: 4251)
      std::vector<DXGI_MODE_DESC1> m_modes;
      #pragma warning(pop)

      DXGI_OUTPUT_DESC1 m_desc;
      bool m_3d;
      bool m_tearing;
      bool m_hdr;
   };
}
