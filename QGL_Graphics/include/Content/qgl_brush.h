#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Descriptors/qgl_brush_descriptor.h"
#include "include/Helpers/qgl_dx_constructors.h"

namespace qgl::graphics
{
   class brush
   {
      public:
      brush(descriptors::brush_descriptor&& desc,
            context_2d* devContext_p) :
         m_desc(std::forward<descriptors::brush_descriptor>(desc))
      {
         make_brush(devContext_p);
      }

      /*
       Returns a const pointer to the D2D brush.
       */
      const ID2D1Brush* get() const noexcept
      {
         return m_brush.get();
      }

      /*
       Returns a pointer to the D2D brush.
       */
      ID2D1Brush* get() noexcept
      {
         return m_brush.get();
      }

      private:
      void make_brush(context_2d* devContext_p)
      {
         switch (m_desc.style)
         {
            case brush_styles::solid:
            {
               winrt::check_hresult(devContext_p->CreateSolidColorBrush(
                  to_d2d_color(m_desc.stops.at(0).color),
                  reinterpret_cast<ID2D1SolidColorBrush**>(m_brush.put())));
               break;
            }
            case brush_styles::gradient_linear:
            {
               auto stops = to_d2d_stop(m_desc.stops);

               winrt::com_ptr<ID2D1GradientStopCollection> stopCollection;
               winrt::check_hresult(devContext_p->CreateGradientStopCollection(
                  stops.data(),
                  static_cast<UINT32>(m_desc.stop_count),
                  stopCollection.put()));

               auto linearProps =
                  reinterpret_cast<D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES*>(
                     m_desc.properties.data());

               winrt::check_hresult(devContext_p->CreateLinearGradientBrush(
                  *linearProps,
                  stopCollection.get(),
                  reinterpret_cast<ID2D1LinearGradientBrush**>(m_brush.put())));
               break;
            }
            case brush_styles::gradient_radial:
            {
               auto stops = to_d2d_stop(m_desc.stops);

               winrt::com_ptr<ID2D1GradientStopCollection> stopCollection;
               winrt::check_hresult(
                  devContext_p->CreateGradientStopCollection(
                     stops.data(),
                     static_cast<UINT>(m_desc.stop_count),
                     stopCollection.put()));

               auto radialProps =
                  reinterpret_cast<D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES*>(
                     m_desc.properties.data());

               winrt::check_hresult(
                  devContext_p->CreateRadialGradientBrush(
                     *radialProps,
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

      winrt::com_ptr<ID2D1Brush> m_brush;
      descriptors::brush_descriptor m_desc;
   };
}