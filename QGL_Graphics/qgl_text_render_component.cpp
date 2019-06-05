#include "pch.h"
#include "include/Components/qgl_text_render_component.h"

namespace qgl::graphics::components
{
   struct text_render_component::impl
   {
      impl(content::brush* brush_p, content::text_format* fmt_p) :
         Brush_p(brush_p),
         Format_p(fmt_p)
      {

      }

      impl(const impl&) = default;

      impl(impl&&) = default;

      ~impl() noexcept
      {

      }

      void update(text_render_component::uc_t* context,
                  text_render_component::u_t* obj)
      {
         context->DrawText(obj->c_str(),
                           static_cast<UINT>(obj->length()),
                           Format_p->get(),
                           *obj->layout(),
                           Brush_p->get());
      }

      content::brush* Brush_p;
      content::text_format* Format_p;
   };


   text_render_component::text_render_component(
      content::brush* brush_p,
      content::text_format* fmt_p,
      const GUID* g) :
      m_impl_p(new impl(brush_p, fmt_p)),
      icomponent(g)
   {
   }

   text_render_component::text_render_component(
      const text_render_component& r) :
      icomponent(r)
   {
      delete m_impl_p;
      m_impl_p = new impl(*r.m_impl_p);
   }

   text_render_component::text_render_component(
      text_render_component&& r) :
      icomponent(std::move(r))
   {
      delete m_impl_p;
      m_impl_p = r.m_impl_p;
      r.m_impl_p = nullptr;
   }

   text_render_component::~text_render_component() noexcept
   {
      delete m_impl_p;
      m_impl_p = nullptr;
   }

   void text_render_component::update(uc_t* context,
                                      u_t* obj)
   {
      m_impl_p->update(context, obj);
   }

}