#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content::buffers
{
   #pragma pack(push, 1)
   struct QGL_CONTENT_API RECTANGLE_BUFFER
   {
      public:
      RECTANGLE_BUFFER();

      RECTANGLE_BUFFER(const RECTANGLE_BUFFER&) = default;

      RECTANGLE_BUFFER(RECTANGLE_BUFFER&&) = default;

      ~RECTANGLE_BUFFER() noexcept = default;

      inline friend void swap(RECTANGLE_BUFFER& l,
                              RECTANGLE_BUFFER& r) noexcept
      {
         using std::swap;
         swap(l.Flags, r.Flags);
         swap(l.Reserved1, r.Reserved1);
         swap(l.Reserved2, r.Reserved2);

         swap(l.TopLeftX, r.TopLeftX);
         swap(l.TopLeftY, r.TopLeftY);
         swap(l.TopLeftZ, r.TopLeftZ);

         swap(l.BottomRightX, r.BottomRightX);
         swap(l.BottomRightY, r.BottomRightY);
         swap(l.BottomRightZ, r.BottomRightZ);
      }

      inline RECTANGLE_BUFFER& operator=(RECTANGLE_BUFFER r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      inline friend bool operator==(const RECTANGLE_BUFFER& l,
                                    const RECTANGLE_BUFFER& r) noexcept
      {
         return l.Flags == r.Flags &&
            l.Reserved1 == r.Reserved1 &&
            l.Reserved2 == r.Reserved2 &&
            l.TopLeftX == r.TopLeftX &&
            l.TopLeftY == r.TopLeftY &&
            l.TopLeftZ == r.TopLeftZ &&
            l.BottomRightX == r.BottomRightX &&
            l.BottomRightY == r.BottomRightY &&
            l.BottomRightZ == r.BottomRightZ;
      }

      uint8_t Flags;
      uint16_t Reserved1;
      uint32_t Reserved2;

      qgl::math::rational<int32_t> TopLeftX;
      qgl::math::rational<int32_t> TopLeftY;
      qgl::math::rational<int32_t> TopLeftZ;

      qgl::math::rational<int32_t> BottomRightX;
      qgl::math::rational<int32_t> BottomRightY;
      qgl::math::rational<int32_t> BottomRightZ;
   };
   #pragma pack(pop)
}