#pragma once
#include "include/qgl_game_include.h"
#include "include/Components/qgl_texture.h"

namespace qgl::components
{
   static constexpr guid IMAGE_GUID{ "A5E4623918E546EE85FBCB162ECEA60B" };

   class image : public game_component<image>
   {
      public:
      image(const texture&,
            const game_update_functor<image>& updateFunctor);

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