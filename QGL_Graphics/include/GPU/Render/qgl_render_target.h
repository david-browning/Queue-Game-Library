#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/qgl_igpu_buffer.h"
#include "include/GPU/Render/qgl_irender_target.h"
#include "include/qgl_graphics_device.h"
#include "include/GPU/Descriptors/qgl_rtv_descriptor_heap.h"
#include "include/GPU/Memory/qgl_committed_allocator.h"

namespace qgl::graphics::gpu
{
   class render_target :
      public irender_target,
      public igpu_buffer<
      DXGI_SWAP_CHAIN_DESC1,
      D3D12_RENDER_TARGET_VIEW_DESC,
      i3d_render_target>
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
      render_target(graphics_device_ptr&& dev_sp,
                    rtv_descriptor_heap& rtvHeap,
                    size_t frameIndex) :
         m_dev_sp(std::forward<graphics_device_ptr>(dev_sp)),
         m_renderTargetsAquired(false),
         m_frmIdx(frameIndex)
      {
         m_cpuHandle = rtvHeap.at_cpu(frameIndex);
         acquire();
         rtvHeap.insert(m_dev_sp, frameIndex, *this);
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
      virtual ~render_target()
      {
         release();
      }

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

      virtual size_t size() const noexcept;

      virtual gpu_alloc_handle alloc_handle() const noexcept;

      virtual const i3d_render_target* get() const
      {
         return m_renderTarget_up.get();
      }

      virtual i3d_render_target* get()
      {
         return m_renderTarget_up.get();
      }

      /*
       Returns the render target format. This is decided by the swap chain.
       */
      virtual DXGI_FORMAT format() const noexcept
      {
         return m_swapChainDesc.Format;
      }

      virtual D3D12_CPU_DESCRIPTOR_HANDLE where() const noexcept
      {
         return m_cpuHandle;
      }

      void release()
      {
         if (m_renderTargetsAquired)
         {
            m_renderTarget_up = nullptr;
            i3d_bridge_render_target* targets2D[] =
            {
               m_wrappedRenderTarget_up.get()
            };

            m_dev_sp->dev_back_compat()->ReleaseWrappedResources(targets2D, 1);
            m_renderTargetsAquired = false;
         }
      }

      void acquire()
      {
         winrt::check_hresult(m_dev_sp->swp_chn()->GetDesc1(&m_swapChainDesc));
         m_viewDesc.Format = m_swapChainDesc.Format;
         m_viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
         m_viewDesc.Texture2D.MipSlice = 0;
         m_viewDesc.Texture2D.PlaneSlice = 0;


         // Create the render target.
         winrt::check_hresult(m_dev_sp->swp_chn()->GetBuffer(
            m_frmIdx, IID_PPV_ARGS(m_renderTarget_up.put())));

         // Create a wrapped 11On12 resource of this back buffer. Since we are 
         // rendering all D3D12 content first and then all D2D content, we specify 
         // the In resource state as RENDER_TARGET - because D3D12 will have last 
         // used it in this state - and the Out resource state as PRESENT. When 
         // ReleaseWrappedResources() is called on the 11On12 device, the resource 
         // will be transitioned to the PRESENT state.
         auto displayProps = winrt::Windows::Graphics::Display::
            DisplayInformation::GetForCurrentView();
         auto bitmapProperties = D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
            displayProps.RawDpiX(),
            displayProps.RawDpiY());

         D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };
         winrt::check_hresult(m_dev_sp->dev_back_compat()->CreateWrappedResource(
            get(),
            &d3d11Flags,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT,
            IID_PPV_ARGS(m_wrappedRenderTarget_up.put())));

         // Create a render target for 2D calls
         auto surface = m_wrappedRenderTarget_up.as<IDXGISurface>();
         winrt::check_hresult(m_dev_sp->ctx_2d()->CreateBitmapFromDxgiSurface(
            surface.get(), &bitmapProperties, m_d2dTarget_up.put()));
      }

      /*
       Acquires resources so this render target can be used.
       */
      void acquire_resources()
      {
         i3d_bridge_render_target* targets2D[] =
         {
            m_wrappedRenderTarget_up.get()
         };

         m_dev_sp->dev_back_compat()->AcquireWrappedResources(targets2D, 1);
         m_renderTargetsAquired = true;
      }

      private:
      graphics_device_ptr m_dev_sp;
      D3D12_CPU_DESCRIPTOR_HANDLE m_cpuHandle;
      bool m_renderTargetsAquired;

      ResourceDescriptionT m_swapChainDesc;
      ViewDescriptionT m_viewDesc;

      winrt::com_ptr<i3d_render_target> m_renderTarget_up;
      winrt::com_ptr<i3d_bridge_render_target> m_wrappedRenderTarget_up;
      winrt::com_ptr<i2d_render_target> m_d2dTarget_up;

      size_t m_frmIdx;
   };
}