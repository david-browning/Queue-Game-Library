#pragma once
#include "qgl_graphics_include.h"
#include "qgl_wtext.h"
#include "qgl_brush.h"
#include "qgl_text_format.h"

namespace qgl::graphics
{
   /*
    Renders a wtext object.
    The "style" the text is render depends on the brush and text_format passed to this's
    constructor. The font spacing, size, and style is stored in a text_format. The font color
    is stored in a brush.
    Update context is a raw pointer to an d2d_context.
    Object to update is a wtext.
    */
   class LIB_EXPORT text_render_component : public icomponent<d2d_context*, wtext>
   {
      public:
      using uc_t = d2d_context * ;
      using u_t = wtext;

      /*
       Construct using shared pointers.
       */
      text_render_component(std::shared_ptr<brush>& brush_p,
                            std::shared_ptr<text_format>& fmt_p);

      /*
       Construct by copying the brush and formatting.
      */
      text_render_component(brush& brsh,
                            text_format& fmt);

      /*
       Construct by moving the brush and formatting.
       */
      text_render_component(brush&& brsh,
                            text_format&& fmt);

      text_render_component(const text_render_component& r) = default;

      text_render_component(text_render_component&& r) = default;

      virtual ~text_render_component() noexcept = default;

      virtual void update(const uc_t& context, u_t& obj);

      protected:
      #pragma warning(push)
      #pragma warning(disable: 4251)
      /*
       Color to render the text.
       */
      std::shared_ptr<brush> m_brush_p;

      /*
       Format, such as font style, size, weight, spacing, etc... to render the text.
       */
      std::shared_ptr<text_format> m_format_p;
      #pragma warning(pop)
   };
}