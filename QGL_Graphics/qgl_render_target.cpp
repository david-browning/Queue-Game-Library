#include "pch.h"
#include "include/GPU/Render/qgl_render_target.h"
#include "include/Interfaces/qgl_igraphics_device.h"
#include "include/GPU/Descriptors/qgl_rtv_descriptor_heap.h"
#include <winrt/Windows.Graphics.Display.h>
using namespace winrt::Windows::Graphics;

namespace qgl::graphics::gpu::render
{
   struct render_target::impl
   {
      public:
      impl(static_ptr_ref<graphics::igraphics_device> dev,
           D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle,
           d3d_render_target** resource,
           size_t frameIndex) :
         CPUHandle(cpuHandle),
         RenderTargetsAquired(false)
      {
         auto swapChain = dev->swap_chain();

         winrt::check_hresult(swapChain->GetDesc1(&SwapChainDesc));
         ViewDesc.Format = SwapChainDesc.Format;
         ViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
         ViewDesc.Texture2D.MipSlice = 0;
         ViewDesc.Texture2D.PlaneSlice = 0;

         auto displayProps = Display::DisplayInformation::GetForCurrentView();
         construct(dev,
                   frameIndex,
                   resource,
                   displayProps.RawDpiX(),
                   displayProps.RawDpiY());
      }

      impl(const impl&) = delete;

      impl(impl&&) = default;

      ~impl()
      {

      }

      ViewDescriptionT ViewDesc;
      ResourceDescriptionT SwapChainDesc;
      D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle;
      bool RenderTargetsAquired;

      std::vector<D3D12_RECT> Rectangles;
      winrt::com_ptr<d3d_wrapped_render_target> WrappedRenderTarget_p;
      winrt::com_ptr<d2d_render_target> D2DTarget_p;

      private:
      void construct(static_ptr_ref<igraphics_device> gdev,
                     size_t frameIndex,
                     d3d_render_target** resource,
                     float dpiX,
                     float dpiY)
      {
         winrt::check_hresult(gdev->swap_chain()->GetBuffer(
            static_cast<UINT>(frameIndex),
            IID_PPV_ARGS(resource)));

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
            *resource,
            &d3d11Flags,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT,
            IID_PPV_ARGS(WrappedRenderTarget_p.put())));

         // Create a render target for D2D to draw directly to this back buffer.
         winrt::com_ptr<IDXGISurface> const surface =
            WrappedRenderTarget_p.as<IDXGISurface>();

         winrt::check_hresult(gdev->d2d1_context()->CreateBitmapFromDxgiSurface(
            surface.get(),
            &bitmapProperties,
            D2DTarget_p.put()));

         #ifdef DEBUG
         std::wstringstream nameStream;
         nameStream << L"Render Target " << frameIndex;
         name_d3d(*resource, nameStream.str().c_str());
         #endif
      }
   };

   render_target::render_target(
      static_ptr_ref<graphics::igraphics_device> dev,
      static_ptr_ref<rtv_descriptor_heap> rtvHeap,
      size_t frameIndex)
   {
      auto cpuHandle = rtvHeap->at_cpu(frameIndex);
      m_impl_p = new impl(dev,
                          cpuHandle,
                          put(),
                          frameIndex);
      rtvHeap->insert(dev, frameIndex, this);
   }

   render_target::render_target(render_target&& r) :
      igpu_buffer(std::move(r))
   {
      delete m_impl_p;
      m_impl_p = r.m_impl_p;
      r.m_impl_p = nullptr;
   }

   render_target::~render_target()
   {
      delete m_impl_p;
      m_impl_p = nullptr;
   }

   const render_target::ResourceDescriptionT* render_target::description() const
   {
      return &m_impl_p->SwapChainDesc;
   }

   const render_target::ViewDescriptionT* render_target::view() const
   {
      return &m_impl_p->ViewDesc;
   }

   DXGI_FORMAT render_target::format() const noexcept
   {
      return m_impl_p->SwapChainDesc.Format;
   }

   void render_target::rectangles(const D3D12_RECT* rects,
                                  size_t rectCount)
   {
      m_impl_p->Rectangles.resize(rectCount);
      for (size_t i = 0; i < rectCount; i++)
      {
         m_impl_p->Rectangles[i] = rects[i];
      }
   }

   size_t render_target::rectangle_count() const noexcept
   {
      return m_impl_p->Rectangles.size();
   }

   const D3D12_RECT* render_target::rectangles() const noexcept
   {
      return m_impl_p->Rectangles.data();
   }

   D3D12_CPU_DESCRIPTOR_HANDLE render_target::where() const
   {
      return m_impl_p->CPUHandle;
   }

   const d2d_render_target* render_target::d2d_target() const noexcept
   {
      return m_impl_p->D2DTarget_p.get();
   }

   void render_target::acquire_resources(static_ptr_ref<d3d11_device> dev_p)
   {
      d3d_wrapped_render_target* targets2D[] =
      {
         m_impl_p->WrappedRenderTarget_p.get()
      };

      dev_p->AcquireWrappedResources(targets2D, 1);
      m_impl_p->RenderTargetsAquired = true;
   }

   void render_target::release_resources(static_ptr_ref<d3d11_device> dev_p)
   {
      d3d_wrapped_render_target* targets2D[] =
      {
         m_impl_p->WrappedRenderTarget_p.get()
      };

      dev_p->ReleaseWrappedResources(targets2D, 1);
      m_impl_p->RenderTargetsAquired = false;
   }

   void render_target::dispose(static_ptr_ref<d3d11_device> dev_p)
   {
      if (m_impl_p->RenderTargetsAquired)
      {
         release_resources(dev_p);
      }

      nullify();
      m_impl_p->D2DTarget_p = nullptr;
      m_impl_p->WrappedRenderTarget_p = nullptr;
   }
}