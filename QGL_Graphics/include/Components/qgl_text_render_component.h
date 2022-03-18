#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/qgl_wtext.h"
#include "include/Content/qgl_brush.h"
#include "include/Content/qgl_text_format.h"
#include "include/Interfaces/qgl_igraphics_device.h"

namespace qgl::graphics::components
{
   using text_render_updatable = static_ptr_ref<content::wtext>;
   using text_render_context = static_ptr_ref<igraphics_device>;
   namespace functors
   {
      class text_render_functor
      {
         public:
         text_render_functor(static_ptr_ref<content::brush> brush_p,
                             static_ptr_ref<content::text_format> fmt_p) :
            m_brush_p(brush_p),
            m_fmt_p(fmt_p)
         {

         }

         text_render_functor(const text_render_functor&) = default;

         text_render_functor(text_render_functor&&) = default;

         ~text_render_functor() noexcept = default;

         HRESULT operator()(text_render_updatable& txt,
                            text_render_context& dev_p)
         {
            dev_p->d2d1_context()->DrawText(txt->c_str(),
                                            static_cast<UINT32>(txt->length()),
                                            m_fmt_p->get(),
                                            txt->layout(),
                                            m_brush_p->get());
            return S_OK;
         }

         private:
         static_ptr_ref<content::brush> m_brush_p;
         static_ptr_ref<content::text_format> m_fmt_p;
      };
   }

   /*
    Renders a wtext object.
    The "style" the text is render depends on the brush and text_format passed
    to this's constructor. The font spacing, size, and style is stored in a
    text_format. The font color is stored in a brush.
    */
   class text_render_component : public component<text_render_updatable,
                                                  text_render_context,
                                                  HRESULT,
                                                  functors::text_render_functor>
   {
      public:
      text_render_component(static_ptr_ref<content::brush> brush_p,
                            static_ptr_ref<content::text_format> fmt_p) :
         component(TEXT_RENDER_COMPONENT_GUID,
                   functors::text_render_functor(brush_p, fmt_p))
      {

      }

      private:
      // {1625CE76-C3D2-4A2A-9194-35A4667E4FAE}
      static const qgl::guid TEXT_RENDER_COMPONENT_GUID =
      { 
         0x16, 0x25, 0xce, 0x76,
         0xc3, 0xd2,
         0x4a, 0x2a,
         0x91, 0x94, 0x35, 0xa4, 0x66, 0x7e, 0x4f, 0xae
      };
   };
}