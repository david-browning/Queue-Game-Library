#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::gpu
{
   /*
    Stores a position, color, normal, and UV coordinate.
    This is final so that there is no vtable pointer.
    */
   class geom3d_vert final
   {
      public:
      geom3d_vert() :
         position(0.0f, 0.0f, 0.0f),
         color(1.0f, 0.0f, 0.0f, 1.0f),
         normal(0.0f, 0.0f, -1.0f),
         uv(0.0f, 0.0f)
      {

      }

      geom3d_vert(DirectX::FXMVECTOR pos,
                  DirectX::FXMVECTOR norm,
                  DirectX::FXMVECTOR col,
                  DirectX::GXMVECTOR u)
      {
         DirectX::XMStoreFloat3(&position, pos);
         DirectX::XMStoreFloat3(&normal, norm);
         DirectX::XMStoreFloat4(&color, col);
         DirectX::XMStoreFloat2(&uv, u);
      }

      geom3d_vert(const DirectX::XMFLOAT3& pos,
                  const DirectX::XMFLOAT3& norm,
                  const DirectX::XMFLOAT4& col,
                  const DirectX::XMFLOAT2& u) :
         position(pos),
         normal(norm),
         color(col),
         uv(u)
      {

      }

      geom3d_vert(const geom3d_vert&) = default;

      geom3d_vert(geom3d_vert&&) noexcept = default;

      ~geom3d_vert() noexcept = default;

      friend void swap(geom3d_vert& l, geom3d_vert& r) noexcept
      {
         using std::swap;
         swap(l.position, r.position);
         swap(l.color, r.color);
         swap(l.uv, r.uv);
         swap(l.normal, r.normal);
      }

      geom3d_vert& operator=(geom3d_vert r)
      {
         swap(*this, r);
         return *this;
      }

      /*
       Position of the vertex. The Z position should remain 0.
       */
      DirectX::XMFLOAT3 position;

      /*
       The vertex normal.
       */
      DirectX::XMFLOAT3 normal;

      /*
       Color of the vertex.
       */
      DirectX::XMFLOAT4 color;

      /*
       UV coordinates for texture mapping.
       */
      DirectX::XMFLOAT2 uv;
   };
}