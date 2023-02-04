#pragma once
#include "include/qgl_game_include.h"
#include "include/Components/qgl_texture.h"
#include "include/Components/qgl_polygon.h"
#include "include/Components/qgl_shader.h"

namespace qgl::components
{
   static constexpr guid SPRITE_GUID{ "4FCF45F38A704C8DB2201D47BD8AF70F" };

   class sprite_context
   {
      DirectX::XMVECTOR position;
      DirectX::XMVECTOR rotation;
   };

   class sprite_instrinsics
   {
      polygon<qgl::graphics::gpu::sprite_vert> m_geom;
      shader m_shader;
      texture m_texture;
   };

   class sprite : public game_component<sprite>,
                  protected flyweight<sprite_instrinsics, sprite_context>
   {
      public:
      sprite(const sprite_instrinsics& intrinsic,
             const sprite_context& cnxt,
             game_update_functor<sprite> f) :
         flyweight(intrinsic, cnxt),
         component(SPRITE_GUID, f)
      {

      }

      sprite(const sprite_instrinsics& intrinsic,
             sprite_context&& cnxt,
             game_update_functor<sprite> f) :
         flyweight(intrinsic, std::forward<sprite_context>(cnxt)),
         component(SPRITE_GUID, f)
      {

      }

      sprite(const sprite&) = delete;

      sprite(sprite&&) noexcept = default;

      virtual ~sprite() noexcept = default;

      private:

   };
}