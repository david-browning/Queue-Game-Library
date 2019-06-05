#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/qgl_wtext.h"
#include "include/Content/qgl_brush.h"
#include "include/Content/qgl_text_format.h"

namespace qgl::graphics::components
{
   /*
    Renders a wtext object.
    The "style" the text is render depends on the brush and text_format passed 
    to this's constructor. The font spacing, size, and style is stored in a 
    text_format. The font color is stored in a brush.
    Update context is a d2d_context.
    Object to update is a wtext.
    */
   class QGL_GRAPHICS_API text_render_component : 
      public icomponent<d2d_context, content::wtext>
   {
      public:
      using uc_t = d2d_context;
      using u_t = content::wtext;

      /*
       The brush and text format are not copied. Do not allow them to 
       go out of scope.
       */
      text_render_component(content::brush* brush_p,
                            content::text_format* fmt_p,
                            const GUID* g);

      /*
       Copy constructor.
       */
      text_render_component(const text_render_component& r);

      /*
       Move constructor.
       */
      text_render_component(text_render_component&& r);

      /*
       Destructor
       */
      virtual ~text_render_component() noexcept;

      /*
       Renders the wtext using the context.
       */
      virtual void update(uc_t* context, u_t* obj);

      protected:

      struct impl;
      impl* m_impl_p = nullptr;
   };
}