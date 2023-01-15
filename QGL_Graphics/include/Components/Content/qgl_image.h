#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Components/Content/qgl_texture.h"
#include "include/GPU/Render/qgl_render_target.h"
#include "include/GPU/Memory/qgl_igpu_allocator.h"

namespace qgl::components
{
   static constexpr guid IMAGE_GUID{ "A5E4623918E546EE85FBCB162ECEA60B" };

   class image : public game_component<image>
   {
      public:
      image(const texture&,
            const game_update_functor<image>& updateFunctor) :
         component(IMAGE_GUID, updateFunctor)
      {

      }

      image(const image&) = delete;

      image(image&& x) noexcept :
         m_img_p(std::move(x.m_img_p)),
         component(std::move(x))
      {
         x.m_img_p = nullptr;
      }

      virtual ~image() noexcept
      {
         m_img_p = nullptr;
      }

      ID2D1Image* get() noexcept
      {
         return m_img_p.get();
      }

      const ID2D1Image* get() const noexcept
      {
         return m_img_p.get();
      }

      private:
      pptr<ID2D1Image> m_img_p;
   };
}