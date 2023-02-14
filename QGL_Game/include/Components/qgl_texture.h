#pragma once
#include "include/qgl_game_include.h"

namespace qgl::components
{
   static constexpr guid TEXTURE_GUID{ "DAC1DC35ADBF4CAB89DBD584D14126FA" };

   class texture : public game_component<texture>
   {
      public:
      texture(game_update_functor<texture> f);

      texture(const texture&) = delete;

      texture(texture&&) noexcept;

      virtual ~texture() noexcept;

      private:
      //qgl::graphics::gpu::texture_buffer m_txt;
   };
}