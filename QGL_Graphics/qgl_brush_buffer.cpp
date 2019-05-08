#include "pch.h"
#include "include/qgl_brush_buffer.h"
#include <variant>

qgl::graphics::BRUSH_BUFFER::BRUSH_BUFFER() :
   m_style(static_cast<uint8_t>(TEXT_BRUSH_STYLE::TEXT_BRUSH_STYLE_SOLID)),
   m_reserved1(0),
   m_stopCount(0)
{
   m_stops[0].color.r = 1.0f;
   m_stops[0].color.g = 1.0f;
   m_stops[0].color.b = 1.0f;
   m_stops[0].color.a = 1.0f;
}

qgl::graphics::BRUSH_BUFFER::BRUSH_BUFFER(const BRUSH_BUFFER& r) :
   m_style(r.m_style),
   m_reserved1(r.m_reserved1),
   m_stopCount(r.m_stopCount)
{
   MemoryCopy(m_properties, r.m_properties, BRUSH_FORMAT_BUFFER_MAX_PROPRTY_SIZE);
   MemoryCopy(m_stops, r.m_stops, BRUSH_FORMAT_BUFFER_MAX_GRADIANT_STOPS);
}

qgl::graphics::BRUSH_BUFFER::BRUSH_BUFFER(BRUSH_BUFFER&& r) :
   m_style(r.m_style),
   m_reserved1(r.m_reserved1),
   m_stopCount(r.m_stopCount)
{
   MemoryCopy(m_properties, r.m_properties, BRUSH_FORMAT_BUFFER_MAX_PROPRTY_SIZE);
   MemoryCopy(m_stops, r.m_stops, BRUSH_FORMAT_BUFFER_MAX_GRADIANT_STOPS);
}


const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES& qgl::graphics::BRUSH_BUFFER::linear_properties()
{
   if (style() == TEXT_BRUSH_STYLE::TEXT_BRUSH_STYLE_GRADIENT_LINEAR)
   {
      return *reinterpret_cast<D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES*>(&m_properties);
   }
   else
   {
      throw std::bad_variant_access();
   }
}

const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES& qgl::graphics::BRUSH_BUFFER::radial_properties()
{
   if (style() == TEXT_BRUSH_STYLE::TEXT_BRUSH_STYLE_GRADIENT_RADIAL)
   {
      return *reinterpret_cast<D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES*>(&m_properties);
   }
   else
   {
      throw std::bad_variant_access();
   }
}
