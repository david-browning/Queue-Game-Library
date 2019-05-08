#include "pch.h"
#include "include/qgl_render_target.h"
#include "include/qgl_rtv_descriptor_heap.h"
#include <winrt/Windows.Graphics.Display.h>

qgl::graphics::render_target::render_target(const graphics_device& dev,
                                            size_t frameIndex,
                                            const rtv_descriptor_heap& rtvHeap) :
   m_frameIndex(frameIndex),
   m_swapChainDesc(),
   m_handle(),
   m_viewDesc(),
   m_rectangles(nullptr),
   m_wrappedRenderTarget(nullptr),
   m_d2dRenderTarget_p(nullptr),
   m_acquired(false),
   igpu_buffer(dev.com_d3d12_device())
{
   auto& swapChain = dev.com_swap_chain();

   m_handle = rtvHeap.at_cpu(frameIndex);
   winrt::check_hresult(swapChain->GetDesc1(&m_swapChainDesc));
   m_viewDesc.Format = m_swapChainDesc.Format;
   m_viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
   m_viewDesc.Texture2D.MipSlice = 0;
   m_viewDesc.Texture2D.PlaneSlice = 0;

   auto displayProps = winrt::Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
   p_allocate(dev.com_d3d11on12_device(),
              swapChain,
              dev.com_d2d1_context(),
              displayProps.RawDpiX(),
              displayProps.RawDpiY());
}

qgl::graphics::render_target::render_target(render_target&& r) :
   m_viewDesc(std::move(r.m_viewDesc)),
   m_swapChainDesc(std::move(r.m_swapChainDesc)),
   m_frameIndex(r.m_frameIndex),
   m_handle(std::move(r.m_handle)),
   m_rectangles(std::move(r.m_rectangles)),
   m_wrappedRenderTarget(std::move(r.m_wrappedRenderTarget)),
   m_d2dRenderTarget_p(std::move(r.m_d2dRenderTarget_p)),
   m_acquired(r.m_acquired),
   igpu_buffer(std::move(r))
{
}

qgl::graphics::render_target::~render_target()
{
}

void qgl::graphics::render_target::rectangles(std::initializer_list<D3D12_RECT>& rects)
{
   if (!m_rectangles)
   {
      m_rectangles = std::shared_ptr<std::vector<D3D12_RECT>>();
   }

   m_rectangles->resize(rects.size());
   size_t i = 0;
   for (auto& rect : rects)
   {
      m_rectangles->at(i) = rect;
      i++;
   }
}

void qgl::graphics::render_target::acquire_resources(const winrt::com_ptr<d3d11_device>& dev_p)
{
   d3d_wrapped_render_target* targets2D[] = { m_wrappedRenderTarget.get() };
   dev_p->AcquireWrappedResources(targets2D, 1);
   m_acquired = true;
}

void qgl::graphics::render_target::release_resources(const winrt::com_ptr<d3d11_device>& dev_p)
{
   d3d_wrapped_render_target* targets2D[] = { m_wrappedRenderTarget.get() };
   dev_p->ReleaseWrappedResources(targets2D, 1);
   m_acquired = false;
}

void qgl::graphics::render_target::p_release(const winrt::com_ptr<d3d11_device>& dev_p)
{
   if (m_acquired)
   {
      release_resources(dev_p);
   }

   m_resource = nullptr;
   m_d2dRenderTarget_p = nullptr;
   m_wrappedRenderTarget = nullptr;
}

void qgl::graphics::render_target::p_allocate(const winrt::com_ptr<d3d11_device>& d3d11on12Device,
                                              const winrt::com_ptr<d3d_swap_chain>& swapChain_p,
                                              const winrt::com_ptr<d2d_context>& d2dContext_p,
                                              float dpiX,
                                              float dpiY)
{
   winrt::check_hresult(swapChain_p->GetBuffer(static_cast<UINT>(m_frameIndex),
                                               IID_PPV_ARGS(m_resource.put())));

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
   winrt::check_hresult(d3d11on12Device->CreateWrappedResource(
      get(),
      &d3d11Flags,
      D3D12_RESOURCE_STATE_RENDER_TARGET,
      D3D12_RESOURCE_STATE_PRESENT,
      IID_PPV_ARGS(m_wrappedRenderTarget.put())));

   // Create a render target for D2D to draw directly to this back buffer.
   winrt::com_ptr<IDXGISurface> const surface{ m_wrappedRenderTarget.as<IDXGISurface>() };
   winrt::check_hresult(d2dContext_p->CreateBitmapFromDxgiSurface(
      surface.get(),
      &bitmapProperties,
      m_d2dRenderTarget_p.put()));

   #ifdef DEBUG
   std::wstringstream nameStream;
   nameStream << L"Render Target " << m_frameIndex;
   name_d3d(m_resource.get(), nameStream.str().c_str());

   #endif
}
