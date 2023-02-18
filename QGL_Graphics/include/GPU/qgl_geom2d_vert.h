#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::gpu
{
   /*
    Stores a position, color, and UV coordinate. While the Z position can be 
    anything, it should be 0.
    This is final so that there is no vtable pointer.
    */
   class geom2d_vert final
   {
      public:
      geom2d_vert() :
         position(0.0f, 0.0f),
         color(1.0f, 0.0f, 0.0f),
         uv(0.0f, 0.0f)
      {

      }

      geom2d_vert(DirectX::FXMVECTOR pos, 
                  DirectX::FXMVECTOR col,
                  DirectX::FXMVECTOR u)
      {
         DirectX::XMStoreFloat2(&position, pos);
         DirectX::XMStoreFloat3(&color, col);
         DirectX::XMStoreFloat2(&uv, u);
      }

      geom2d_vert(const DirectX::XMFLOAT2& pos, 
                  const DirectX::XMFLOAT3& col,
                  const DirectX::XMFLOAT2& u) :
         position(pos),
         color(col),
         uv(u)
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
         swap(l.uv, r.uv);
      }

      geom2d_vert& operator=(geom2d_vert r)
      {
         swap(*this, r);
         return *this;
      }
      
      /*
       Color of the vertex.
       */
      DirectX::XMFLOAT3 color;

      /*
       Position of the vertex. 
       */
      DirectX::XMFLOAT2 position;

      /*
       UV coordinates for texture mapping.
       */
      DirectX::XMFLOAT2 uv;
   };
}