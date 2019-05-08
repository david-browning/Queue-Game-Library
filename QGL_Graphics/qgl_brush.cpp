#include "pch.h"
#include "include/qgl_brush.h"
#include <variant>

qgl::graphics::brush::brush(const BRUSH_BUFFER& fmtBuffer,
                            const winrt::com_ptr<d2d_context>& devContext_p,
                            const content::wcontent_item::str_t& name,
                            const content::wcontent_item::id_t& id) :
   m_brushData(fmtBuffer),
   m_devContext(devContext_p),
   content_item(name, id,
                qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                qgl::content::CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH)
{
   p_make_brush();
}

qgl::graphics::brush::brush(const brush& r) :
   m_brushData(r.m_brushData),
   m_devContext(r.m_devContext),
   content_item(r)
{
   p_make_brush();
}

qgl::graphics::brush::brush(brush && r) :
   m_brushData(std::move(r.m_brushData)),
   m_devContext(std::move(r.m_devContext)),
   content_item(std::move(r))
{
   p_make_brush();

   //Destruct the other's brush. 
   r.m_brush = nullptr;
}

void qgl::graphics::brush::p_make_brush()
{
   switch (m_brushData.style())
   {
      case TEXT_BRUSH_STYLE::TEXT_BRUSH_STYLE_SOLID:
      {
         winrt::check_hresult(
            m_devContext->CreateSolidColorBrush(
               m_brushData.stop(0).color,
               reinterpret_cast<ID2D1SolidColorBrush**>(m_brush.put())));
         break;
      }
      case TEXT_BRUSH_STYLE::TEXT_BRUSH_STYLE_GRADIENT_LINEAR:
      {
         winrt::com_ptr<ID2D1GradientStopCollection> stopCollection;
         winrt::check_hresult(
            m_devContext->CreateGradientStopCollection(m_brushData.stops(),
                                                       static_cast<UINT>(m_brushData.count()),
                                                       stopCollection.put()));
         winrt::check_hresult(
            m_devContext->CreateLinearGradientBrush(
               m_brushData.linear_properties(),
               stopCollection.get(),
               reinterpret_cast<ID2D1LinearGradientBrush**>(m_brush.put())));
         break;
      }
      case TEXT_BRUSH_STYLE::TEXT_BRUSH_STYLE_GRADIENT_RADIAL:
      {
         winrt::com_ptr<ID2D1GradientStopCollection> stopCollection;
         winrt::check_hresult(
            m_devContext->CreateGradientStopCollection(m_brushData.stops(),
                                                       static_cast<UINT>(m_brushData.count()),
                                                       stopCollection.put()));

         winrt::check_hresult(
            m_devContext->CreateRadialGradientBrush(
               m_brushData.radial_properties(),
               stopCollection.get(),
               reinterpret_cast<ID2D1RadialGradientBrush**>(m_brush.put())));

         break;
      }
      default:
      {
         throw std::bad_variant_access();
      }
   }
}


