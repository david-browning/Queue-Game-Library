#include "pch.h"
#include "include/GPU/Render/qgl_render_target.h"
#include "include/Interfaces/qgl_igraphics_device.h"
#include "include/GPU/Descriptors/qgl_rtv_descriptor_heap.h"
#include <winrt/Windows.Graphics.Display.h>
using namespace winrt::Windows::Graphics;

namespace qgl::graphics::gpu::render
{
   render_target::render_target(
      static_ptr_ref<graphics::igraphics_device> dev,
      static_ptr_ref<rtv_descriptor_heap> rtvHeap,
      size_t frameIndex) :
      m_rects(nullptr),
      m_numRects(0),
      m_acquired(false)
   {
      auto swapChain = dev->swap_chain();
      m_handle = rtvHeap->at_cpu(frameIndex);

      winrt::check_hresult(swapChain->GetDesc1(&m_swapChainDesc));
      m_viewDesc.Format = m_swapChainDesc.Format;
      m_viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
      m_viewDesc.Texture2D.MipSlice = 0;
      m_viewDesc.Texture2D.PlaneSlice = 0;

      auto displayProps = Display::DisplayInformation::GetForCurrentView();
      construct(dev,
                displayProps.RawDpiX(),
                displayProps.RawDpiY());

      rtvHeap->insert(dev, frameIndex, this);
   }

   render_target::render_target(render_target&& r) :
      m_d2dTarget_p(std::move(r.m_d2dTarget_p)),
      m_wrappedRenderTarget_p(std::move(r.m_wrappedRenderTarget_p)),
      m_numRects(r.m_numRects),
      m_handle(std::move(r.m_handle)),
      m_frameIndex(r.m_frameIndex),
      m_swapChainDesc(std::move(m_swapChainDesc)),
      m_viewDesc(std::move(m_viewDesc)),
      m_acquired(r.m_acquired)
   {
      delete[] m_rects;
      m_rects = r.m_rects;
      r.m_rects = nullptr;
   }

   render_target::~render_target()
   {
      delete[] m_rects;
   }

   const render_target::ResourceDescriptionT* render_target::description() const
   {
      return &m_swapChainDesc;
   }

   const render_target::ViewDescriptionT* render_target::view() const
   {
      return &m_viewDesc;
   }

   DXGI_FORMAT render_target::format() const noexcept
   {
      return m_swapChainDesc.Format;
   }

   void render_target::rectangles(const D3D12_RECT* rects,
                                  size_t rectCount)
   {
      delete[] m_rects;
      m_numRects = rectCount;
      m_rects = new D3D12_RECT[rectCount];

      memcpy(m_rects, rects, sizeof(D3D12_RECT) * rectCount);
   }

   size_t render_target::rectangle_count() const noexcept
   {
      return m_numRects;
   }

   D3D12_CPU_DESCRIPTOR_HANDLE render_target::where() const
   {
      return m_handle;
   }

   const D3D12_RECT* render_target::rectangles() const noexcept
   {
      return m_rects;
   }

   const d2d_render_target* render_target::d2d_target() const noexcept
   {
      return m_d2dTarget_p.get();
   }

   void render_target::acquire_resources(static_ptr_ref<d3d11_device> dev_p)
   {
      d3d_wrapped_render_target* targets2D[] =
      {
         m_wrappedRenderTarget_p.get()
      };

      dev_p->AcquireWrappedResources(targets2D, 1);
      m_acquired = true;
   }

   void render_target::release_resources(static_ptr_ref<d3d11_device> dev_p)
   {
      d3d_wrapped_render_target* targets2D[] =
      {
         m_wrappedRenderTarget_p.get()
      };

      dev_p->ReleaseWrappedResources(targets2D, 1);
      m_acquired = false;
   }

   void render_target::dispose(static_ptr_ref<d3d11_device> dev_p)
   {
      if (m_acquired)
      {
         release_resources(dev_p);
      }

      nullify();
      m_d2dTarget_p = nullptr;
      m_wrappedRenderTarget_p = nullptr;
   }

   void render_target::construct(static_ptr_ref<igraphics_device> gdev,
                                 float dpiX,
                                 float dpiY)
   {
      winrt::check_hresult(gdev->swap_chain()->GetBuffer(
         static_cast<UINT>(m_frameIndex),
         IID_PPV_ARGS(put())));

      // Create a wrapped 11On12 resource of this back buffer. Since we are 
      // rendering all D3D12 content first and then all D2D content, we specify 
      // the In resource state as RENDER_TARGET - because D3D12 will have last 
      // used it in this state - and the Out resource state as PRESENT. When 
      // ReleaseWrappedResources() is called on the 11On12 device, the resource 
      // will be transitioned to the PRESENT state.
      D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
         D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
         D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
         dpiX,
         dpiY);

      D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };
      winrt::check_hresult(gdev->d3d11on12_device()->CreateWrappedResource(
         get(),
         &d3d11Flags,
         D3D12_RESOURCE_STATE_RENDER_TARGET,
         D3D12_RESOURCE_STATE_PRESENT,
         IID_PPV_ARGS(m_wrappedRenderTarget_p.put())));

      // Create a render target for D2D to draw directly to this back buffer.
      winrt::com_ptr<IDXGISurface> const surface =
         m_wrappedRenderTarget_p.as<IDXGISurface>();

      winrt::check_hresult(gdev->d2d1_context()->CreateBitmapFromDxgiSurface(
         surface.get(),
         &bitmapProperties,
         m_d2dTarget_p.put()));

      #ifdef DEBUG
      std::wstringstream nameStream;
      nameStream << L"Render Target " << m_frameIndex;
      name_d3d(get(), nameStream.str().c_str());
      #endif
   }
}