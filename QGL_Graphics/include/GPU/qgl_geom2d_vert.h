#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::gpu
{
   /*
    Stores a position and color. While the Z position can be anything,
    it should be 0.
    This is final so that there is no vtable pointer.
    */
   class geom2d_vert final
   {
      public:
      geom2d_vert() :
         position(0.0f, 0.0f, 0.0f, 0.0f),
         color(0.0f, 0.0f, 0.0f, 0.0f)
      {

      }

      geom2d_vert(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR col)
      {
         DirectX::XMStoreFloat4(&position, pos);
         DirectX::XMStoreFloat4(&color, col);
      }

      geom2d_vert(const DirectX::XMFLOAT4& pos, const DirectX::XMFLOAT4& col) :
         position(pos),
         color(col)
      {

      }

      geom2d_vert(const geom2d_vert&) = default;

      geom2d_vert(geom2d_vert&&) noexcept = default;

      ~geom2d_vert() noexcept = default;

      friend void swap(geom2d_vert& l, geom2d_vert& r) noexcept
      {
         using std::swap;
         swap(l.position, r.position);
         swap(l.color, r.color);
      }

      geom2d_vert& operator=(geom2d_vert r)
      {
         swap(*this, r);
         return *this;
      }

      /*
       Position of the vertex. The Z position should remain 0.
       */
      DirectX::XMFLOAT4 position;

      /*
       Color of the vertex.
       */
      DirectX::XMFLOAT4 color;
   };
}