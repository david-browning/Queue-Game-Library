#include "pch.h"
#include "include/Content/qgl_brush.h"

using namespace qgl::content::buffers;
namespace qgl::content
{
   struct brush::impl
   {
      impl(const content::buffers::BRUSH_BUFFER* fmtBuffer,
           graphics::d2d_context* devContext_p) :
         D3DBrush(nullptr),
         Buffer(*fmtBuffer)
      {
         make_brush(devContext_p);
      }

      impl(const impl& r) = default;

      impl(impl&& r) = default;

      virtual ~impl() noexcept = default;

      void make_brush(graphics::d2d_context* devContext_p)
      {
         switch (Buffer.style())
         {
            case TEXT_BRUSH_STYLE::TEXT_BRUSH_STYLE_SOLID:
            {
               winrt::check_hresult(
                  devContext_p->CreateSolidColorBrush(
                     Buffer.stop(0)->color,
                     reinterpret_cast<ID2D1SolidColorBrush**>(
                        D3DBrush.put())));
               break;
            }
            case TEXT_BRUSH_STYLE::TEXT_BRUSH_STYLE_GRADIENT_LINEAR:
            {
               winrt::com_ptr<ID2D1GradientStopCollection> stopCollection;
               winrt::check_hresult(
                  devContext_p->CreateGradientStopCollection(
                     Buffer.stops(),
                     static_cast<UINT>(Buffer.size()),
                     stopCollection.put()));
               winrt::check_hresult(
                  devContext_p->CreateLinearGradientBrush(
                     *Buffer.linear_properties(),
                     stopCollection.get(),
                     reinterpret_cast<ID2D1LinearGradientBrush**>(
                        D3DBrush.put())));
               break;
            }
            case TEXT_BRUSH_STYLE::TEXT_BRUSH_STYLE_GRADIENT_RADIAL:
            {
               winrt::com_ptr<ID2D1GradientStopCollection> stopCollection;
               winrt::check_hresult(
                  devContext_p->CreateGradientStopCollection(
                     Buffer.stops(),
                     static_cast<UINT>(Buffer.size()),
                     stopCollection.put()));

               winrt::check_hresult(
                  devContext_p->CreateRadialGradientBrush(
                     *Buffer.radial_properties(),
                     stopCollection.get(),
                     reinterpret_cast<ID2D1RadialGradientBrush**>(
                        D3DBrush.put())));

               break;
            }
            default:
            {
               throw std::bad_variant_access();
            }
         }
      }

      winrt::com_ptr<ID2D1Brush> D3DBrush;

      buffers::BRUSH_BUFFER Buffer;
   };


   brush::brush(const buffers::BRUSH_BUFFER* fmtBuffer,
                qgl::graphics::d2d_context* devContext_p, 
                const wchar_t* name, 
                const qgl::content::content_id id) :
      content_item(name, 
                   id,
                   qgl::content::RESOURCE_TYPE_BRUSH,
                   qgl::content::CONTENT_LOADER_ID_BRUSH),
      m_impl_p(new impl(fmtBuffer, devContext_p))
   {
   }

   brush::~brush() noexcept
   {
      delete m_impl_p;
      m_impl_p = nullptr;
   }

   const ID2D1Brush* brush::get() const noexcept
   {
      return m_impl_p->D3DBrush.get();
   }

   ID2D1Brush* brush::get() noexcept
   {
      return m_impl_p->D3DBrush.get();
   }
}