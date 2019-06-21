#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content::buffers
{
   struct QGL_GRAPHICS_API MULTISAMPLE_DESC_BUFFER final
   {
      public:
      MULTISAMPLE_DESC_BUFFER(uint16_t count = 1,
                              uint16_t quality = 0);

      MULTISAMPLE_DESC_BUFFER(const MULTISAMPLE_DESC_BUFFER&) = default;

      MULTISAMPLE_DESC_BUFFER(MULTISAMPLE_DESC_BUFFER&&) = default;

      ~MULTISAMPLE_DESC_BUFFER() noexcept = default;

      inline friend void swap(MULTISAMPLE_DESC_BUFFER& l,
                              MULTISAMPLE_DESC_BUFFER& r) noexcept
      {
         using std::swap;
         swap(l.Count, r.Count);
         swap(l.Quality, r.Quality);
         swap(l.Reserved1, r.Reserved1);
      }

      inline MULTISAMPLE_DESC_BUFFER& operator=(
         MULTISAMPLE_DESC_BUFFER r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      inline friend bool operator==(const MULTISAMPLE_DESC_BUFFER& l,
                                    const MULTISAMPLE_DESC_BUFFER& r) noexcept
      {
         return l.Count == r.Count &&
            l.Quality == r.Quality;
      }

      /*
       The number of multisamples per pixel.
       Default count is 1.
       */
      uint16_t Count;

      /*
       The image quality level. 
       Default is 0.
       */
      uint16_t Quality;
      uint32_t Reserved1;
   };

   static_assert(sizeof(MULTISAMPLE_DESC_BUFFER) == 8,
                 "MULTISAMPLE_DESC_BUFFER should be 8 bytes.");
}