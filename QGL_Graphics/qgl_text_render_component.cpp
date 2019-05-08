#include "pch.h"
#include "include/qgl_text_render_component.h"

qgl::graphics::text_render_component::text_render_component(std::shared_ptr<brush>& brush_p,
                                                            std::shared_ptr<text_format>& fmt_p) :
   m_brush_p(brush_p),
   m_format_p(fmt_p),
   icomponent()
{
}

qgl::graphics::text_render_component::text_render_component(brush& brsh,
                                                            text_format& fmt) :
   m_brush_p(std::make_shared<brush>(brsh)),
   m_format_p(std::make_shared<text_format>(fmt)),
   icomponent()
{
}

qgl::graphics::text_render_component::text_render_component(brush&& brsh, 
                                                            text_format&& fmt) :
   m_brush_p(std::make_shared<brush>(std::move(brsh))),
   m_format_p(std::make_shared<text_format>(std::move(fmt))),
   icomponent()
{
}

void qgl::graphics::text_render_component::update(const uc_t & context, u_t & obj)
{
   context->DrawText(obj.c_str(),
                     static_cast<UINT32>(obj.length()),
                     m_format_p->get(),
                     obj.layout(),
                     m_brush_p->get());
}