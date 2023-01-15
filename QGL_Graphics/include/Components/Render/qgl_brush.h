#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Descriptors/qgl_brush_descriptor.h"
#include "include/Content/qgl_outline_brush.h"
#include "include/Helpers/qgl_dx_constructors.h"

namespace qgl::graphics
{
   class brush
   {
      public:
      brush(descriptors::brush_descriptor&& desc,
            i2d_context* devContext_p) :
         m_desc(std::forward<descriptors::brush_descriptor>(desc))
      {
         make_brush(devContext_p);
      }

      /*
       Returns a const pointer to the D2D brush.
       */
      const ID2D1Brush* get() const noexcept
      {
         return m_brush_up.get();
      }

      /*
       Returns a pointer to the D2D brush.
       */
      ID2D1Brush* get() noexcept
      {
         return m_brush_up.get();
      }

      private:
      void make_brush(i2d_context* devContext_p)
      {
         switch (m_desc.style)
         {
            case brush_styles::solid:
            {
               check_result(devContext_p->CreateSolidColorBrush(
                  to_d2d_color(m_desc.stops.at(0).color),
                  reinterpret_cast<ID2D1SolidColorBrush**>(m_brush_up.put())));
               break;
            }
            case brush_styles::gradient_linear:
            {
               auto stops = to_d2d_stop(m_desc.stops);

               pptr<ID2D1GradientStopCollection> stopCollection;
               check_result(devContext_p->CreateGradientStopCollection(
                  stops.data(),
                  static_cast<UINT32>(m_desc.stop_count),
                  stopCollection.put()));

               auto linearProps =
                  reinterpret_cast<D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES*>(
                     m_desc.properties.data());

               check_result(devContext_p->CreateLinearGradientBrush(
                  *linearProps,
                  stopCollection.get(),
                  reinterpret_cast<ID2D1LinearGradientBrush**>(m_brush_up.put())));
               break;
            }
            case brush_styles::gradient_radial:
            {
               auto stops = to_d2d_stop(m_desc.stops);

               pptr<ID2D1GradientStopCollection> stopCollection;
               check_result(
                  devContext_p->CreateGradientStopCollection(
                     stops.data(),
                     static_cast<UINT>(m_desc.stop_count),
                     stopCollection.put()));

               auto radialProps =
                  reinterpret_cast<D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES*>(
                     m_desc.properties.data());

               check_result(
                  devContext_p->CreateRadialGradientBrush(
                     *radialProps,
                     stopCollection.get(),
                     reinterpret_cast<ID2D1RadialGradientBrush**>(m_brush_up.put())));
               break;
            }
            default:
            {
               throw std::bad_variant_access();
            }
         }
      }

      pptr<ID2D1Brush> m_brush_up;
      descriptors::brush_descriptor m_desc;
   };
}