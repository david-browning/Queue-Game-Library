#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::components
{
   class render_context
   {
      public:

      private:
   };

   template<class RenderObject>
   using render_update_functor =
      typename std::function<qgl::result_t(RenderObject&, render_context&)>;

   template<class RenderObject>
   using render_comonent = typename qgl::components::component<
      RenderObject,
      render_context,
      qgl::result_t,
      render_update_functor<RenderObject>>;
}