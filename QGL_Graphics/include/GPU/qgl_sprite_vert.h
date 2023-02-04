#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::gpu
{
   /*
    Stores a Position and UV coordinate. While the Z position can be anything,  
    it should be 0.
    This is final so that there is no vtable pointer.
    */
   class sprite_vert final
   {
      public:
      sprite_vert() :
         position(0.0f, 0.0f, 0.0f, 0.0f),
         uv(0.0f, 0.0f)
      {

      }

      sprite_vert(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR uvpos)
      {
         DirectX::XMStoreFloat4(&position, pos);
         DirectX::XMStoreFloat2(&uv, uvpos);
      }

      sprite_vert(const DirectX::XMFLOAT4& pos, const DirectX::XMFLOAT2& uvpos) :
         position(pos),
         uv(uvpos)
      {

      }

      sprite_vert(const sprite_vert&) = default;

      sprite_vert(sprite_vert&&) noexcept = default;

      ~sprite_vert() noexcept = default;

      friend void swap(sprite_vert& l, sprite_vert& r) noexcept
      {
         using std::swap;
         swap(l.position, r.position);
         swap(l.uv, r.uv);
      }

      sprite_vert& operator=(sprite_vert r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Position of the vertex. The Z position should remain 0.
       */
      DirectX::XMFLOAT4 position;

      /*
       Position of the UV coordinate.
       */
      DirectX::XMFLOAT2 uv;
   };
}