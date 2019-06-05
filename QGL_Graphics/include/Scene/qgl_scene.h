#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics
{
   /*
    A scene is a collection of lights, renderable objects, and the render 
    components that render the objects. The scene organizes renderable objects
    by their render component to avoid switching pipeline states. A scene is 
    rendered by passing the command lists to a render engine. The command lists
    should be mostly composed of command bundles, provided by a render 
    component.
    The render engine executes the command lists.
    */
   class QGL_GRAPHICS_API scene
   {
      public:

      const d3d_command_list** command_lists() const;

      void update()
      {
         //Foreach Root signature
            //Foreach PSO
               //Foreach model
                  //Update model using render component.
      }

      private:
      struct impl;
      impl* m_impl_p = nullptr;
   };
}