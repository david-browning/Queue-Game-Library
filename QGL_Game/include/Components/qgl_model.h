#pragma once
#include "include/qgl_game_include.h"
#include "include/Components/qgl_shader.h"
#include "include/Components/qgl_texture.h"
#include "include/Components/qgl_mesh.h"

namespace qgl::components
{
   static constexpr guid MODEL_GUID{ "24099DFC5FCC457486141EAE0137742A" };

   class model_context
   {
      DirectX::XMVECTOR position;
      DirectX::XMVECTOR rotation;
   };

   class model_intrinsics
   {
      shader pshader;
      std::vector<texture> textures;
      model_mesh<qgl::graphics::gpu::geom3d_vert, uint16_t> mesh;
   };

   class model : public game_component<model>,
      public flyweight < model_intrinsics, model_context>
   {
      public:
      model(const model_intrinsics& intrinsic,
            const model_context& cnxt,
            game_update_functor<model> f) :
         flyweight(intrinsic, cnxt),
         component(MODEL_GUID, f)
      {

      }

      model(const model_intrinsics& intrinsic,
            model_context&& cnxt,
            game_update_functor<model> f) :
         flyweight(intrinsic, std::forward<model_context>(cnxt)),
         component(MODEL_GUID, f)
      {

      }

      model(const model&) = delete;

      model(model&&) noexcept = default;

      virtual ~model() noexcept = default;

      private:
   };
}