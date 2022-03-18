#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/igpu_buffer.h"
#include "include/Interfaces/qgl_igraphics_device.h"
#include "include/GPU/Descriptors/qgl_rtv_descriptor_heap.h"

namespace qgl::graphics::gpu
{
   class render_target : public igpu_buffer<
      DXGI_SWAP_CHAIN_DESC1,
      D3D12_RENDER_TARGET_VIEW_DESC,
      d3d_render_target>
   {
      public:
      using ResourceDescriptionT = DXGI_SWAP_CHAIN_DESC1;
      using ViewDescriptionT = D3D12_RENDER_TARGET_VIEW_DESC;

      /*
       Once constructed, this inserts itself into the RTV Heap.
       frameIndex is the render from swap chain that this attaches to.
       Ex: A swap chain may have 3 frames, where the render indices are
       from 0-2.
       TODO: Is there any reason why multiple things cannot bind to the same
       render?
       */
      render_target(const graphics_device_ptr& dev,
                    rtv_descriptor_heap& rtvHeap,
                    size_t frameIndex) :
         m_renderTargetsAquired(false)
      {
         m_cpuHandle = rtvHeap.at_cpu(frameIndex);

         auto swapChain = dev->swap_chain();
         winrt::check_hresult(swapChain->GetDesc1(&m_swapChainDesc));
         m_viewDesc.Format = m_swapChainDesc.Format;
         m_viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
         m_viewDesc.Texture2D.MipSlice = 0;
         m_viewDesc.Texture2D.PlaneSlice = 0;

         auto displayProps = winrt::Windows::Graphics::Display::
            DisplayInformation::GetForCurrentView();
         construct(dev,
                   frameIndex,
                   put(),
                   displayProps.RawDpiX(),
                   displayProps.RawDpiY());

         rtvHeap.insert(dev, frameIndex, *this);
      }

      /*
       Copy constructor.
       */
      render_target(const render_target&) = delete;

      /*
       Move constructor.
       */
      render_target(render_target&&) = default;

      /*
       Destructor.
       */
      virtual ~render_target() = default;

      /*
       Returns a pointer to the swap chain description.
       */
      virtual const ResourceDescriptionT* description() const
      {
         return &m_swapChainDesc;
      }

      /*
       Returns a pointer to the view description so this can be bound to
       the graphics pipeline.
       */
      virtual const ViewDescriptionT* view() const
      {
         return &m_viewDesc;
      }

      /*
       Returns the render target format. This is decided by the swap chain.
       */
      DXGI_FORMAT format() const noexcept
      {
         return m_swapChainDesc.Format;
      }

      template<class ForwardIt>
      void rectangles(ForwardIt first, ForwardIt last)
      {
         m_rectangles.clear();
         m_rectangles.insert(m_rectangles.begin(), first, last);
      }

      const D3D12_RECT* rectangles() const noexcept
      {
         return m_rectangles.data();
      }

      /*
       Returns the number of rectangles.
       */
      size_t rectangle_count() const noexcept
      {
         return m_rectangles.size();
      }

      D3D12_CPU_DESCRIPTOR_HANDLE where() const
      {
         return m_cpuHandle;
      }

      /*
       Returns the 2D render target used for Direct2D.
       */
      const d2d_render_target* d2d_target() const noexcept
      {
         return m_d2dTarget_p.get();
      }

      /*
       Acquires resources so this render target can be used.
       */
      void acquire_resources(const winrt::com_ptr<d3d11_device>& dev_p)
      {
         d3d_wrapped_render_target* targets2D[] =
         {
            m_wrappedRenderTarget_p.get()
         };

         dev_p->AcquireWrappedResources(targets2D, 1);
         m_renderTargetsAquired = true;
      }

      /*
       Releases acquired resources so other render targets can be used.
       */
      void release_resources(const winrt::com_ptr<d3d11_device>& dev_p)
      {
         d3d_wrapped_render_target* targets2D[] =
         {
            m_wrappedRenderTarget_p.get()
         };

         dev_p->ReleaseWrappedResources(targets2D, 1);
         m_renderTargetsAquired = false;
      }

      /*
       Releases all resources so that the render can be rebuilt. This only needs
       to be called when the frames need to be resized.
       */
      void dispose(const winrt::com_ptr<d3d11_device>& dev_p)
      {
         if (m_renderTargetsAquired)
         {
            release_resources(dev_p);
         }

         nullify();
         m_d2dTarget_p = nullptr;
         m_wrappedRenderTarget_p = nullptr;
      }

      private:
      void construct(const graphics_device_ptr& gdev,
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
         winrt::check_hresult(
            gdev->d3d11on12_device()->CreateWrappedResource(
               *resource,
               &d3d11Flags,
               D3D12_RESOURCE_STATE_RENDER_TARGET,
               D3D12_RESOURCE_STATE_PRESENT,
               IID_PPV_ARGS(m_wrappedRenderTarget_p.put())));

         // Create a render target for D2D to draw directly to this back buffer.
         winrt::com_ptr<IDXGISurface> const surface =
            m_wrappedRenderTarget_p.as<IDXGISurface>();

         winrt::check_hresult(
            gdev->d2d1_context()->CreateBitmapFromDxgiSurface(
               surface.get(),
               &bitmapProperties,
               m_d2dTarget_p.put()));

#ifdef DEBUG
         std::wstringstream nameStream;
         nameStream << L"Render Target " << frameIndex;
         name_d3d(*resource, nameStream.str().c_str());
#endif
      }

      ViewDescriptionT m_viewDesc;
      ResourceDescriptionT m_swapChainDesc;
      D3D12_CPU_DESCRIPTOR_HANDLE m_cpuHandle;
      bool m_renderTargetsAquired;

      std::vector<D3D12_RECT> m_rectangles;
      winrt::com_ptr<d3d_wrapped_render_target> m_wrappedRenderTarget_p;
      winrt::com_ptr<d2d_render_target> m_d2dTarget_p;
   };
}