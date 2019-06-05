#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Scene/qgl_scene.h"

namespace qgl::graphics
{
   /*
    A stage is a collection of scenes. Multiple scenes can be loaded at once.
    Stage keeps loaded scene resident on the GPU.
    The stage is submitted to the render engine.
    */
   class QGL_GRAPHICS_API stage
   {
      public:

      void set_loaded_scenes();

      private:
      struct impl;
      impl* m_impl_p = nullptr;
   };
}