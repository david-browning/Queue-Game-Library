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
      render_target(graphics_device* dev_p,
                    rtv_descriptor_heap& rtvHeap,
                    size_t frameIndex) :
         m_dev_p(dev_p),
         m_renderTargetsAquired(false),
         m_frmIdx(frameIndex)
      {
         m_cpuHandle = rtvHeap.at_cpu(frameIndex);
         acquire();
         rtvHeap.insert(m_dev_p, frameIndex, *this);
      }

      /*
       Copy constructor.
       */
      render_target(const render_target&) = delete;

      /*
       Move constructor.
       */
      render_target(render_target&& x) noexcept :
         irender_target(x),
         igpu_buffer(x),
         m_cpuHandle(std::move(x.m_cpuHandle)),
         m_renderTargetsAquired(x.m_renderTargetsAquired),
         m_swapChainDesc(std::move(x.m_swapChainDesc)),
         m_viewDesc(std::move(x.m_viewDesc)),
         m_dev_p(std::move(x.m_dev_p)),
         m_renderTarget_up(std::move(x.m_renderTarget_up)),
         m_wrappedRenderTarget_up(std::move(x.m_wrappedRenderTarget_up)),
         m_d2dTarget_up(std::move(x.m_d2dTarget_up)),
         m_rects(std::move(x.m_rects)),
         m_frmIdx(x.m_frmIdx)
      {
         x.m_dev_p = nullptr;
         x.m_renderTarget_up = nullptr;
         x.m_wrappedRenderTarget_up = nullptr;
         m_d2dTarget_up = nullptr;
         x.m_frmIdx = static_cast<size_t>(-1);
      }

      /*
       Destructor.
       */
      virtual ~render_target()
      {
         release();
         m_dev_p = nullptr;
         m_renderTarget_up = nullptr;
         m_wrappedRenderTarget_up = nullptr;
         m_d2dTarget_up = nullptr;
      }

      size_t index() const noexcept
      {
         return m_frmIdx;
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

      virtual size_t size() const noexcept
      {
         return 0;
      }

#pragma warning(push)
#pragma warning (disable: 4297)
      virtual gpu_alloc_handle alloc_handle() const noexcept
      {
         throw std::invalid_argument{ 
            "Render targets do not have an alloc handle." };
      }
#pragma warning(pop)

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
         // Guard against move also
         if (m_renderTargetsAquired && m_dev_p && m_wrappedRenderTarget_up)
         {
            m_renderTarget_up = nullptr;
            i3d_bridge_render_target* targets2D[] =
            {
               m_wrappedRenderTarget_up.get()
            };

            m_dev_p->dev_back_compat()->ReleaseWrappedResources(targets2D, 1);
            m_renderTargetsAquired = false;
         }
      }

      void acquire()
      {
         winrt::check_hresult(m_dev_p->swp_chn()->GetDesc1(&m_swapChainDesc));
         m_viewDesc.Format = m_swapChainDesc.Format;
         m_viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
         m_viewDesc.Texture2D.MipSlice = 0;
         m_viewDesc.Texture2D.PlaneSlice = 0;


         // Create the render target.
         winrt::check_hresult(m_dev_p->swp_chn()->GetBuffer(
            static_cast<UINT>(m_frmIdx), 
            IID_PPV_ARGS(m_renderTarget_up.put())));

         // Create a wrapped 11On12 resource of this back buffer. Since we are 
         // rendering all D3D12 content first and then all D2D content, we specify 
         // the In resource state as RENDER_TARGET - because D3D12 will have last 
         // used it in this state - and the Out resource state as PRESENT. When 
         // ReleaseWrappedResources() is called on the 11On12 device, the resource 
         // will be transitioned to the PRESENT state.
         auto bitmapProperties = D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
            m_dev_p->wnd()->dpi_x(),
            m_dev_p->wnd()->dpi_y());

         D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };
         winrt::check_hresult(
            m_dev_p->dev_back_compat()->CreateWrappedResource(
               get(),
               &d3d11Flags,
               D3D12_RESOURCE_STATE_RENDER_TARGET,
               D3D12_RESOURCE_STATE_PRESENT,
               IID_PPV_ARGS(m_wrappedRenderTarget_up.put())));

         // Create a render target for 2D calls
         auto surface = m_wrappedRenderTarget_up.as<IDXGISurface>();
         winrt::check_hresult(m_dev_p->ctx_2d()->CreateBitmapFromDxgiSurface(
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

         m_dev_p->dev_back_compat()->AcquireWrappedResources(targets2D, 1);
         m_renderTargetsAquired = true;
      }

      const D3D12_RECT* rectangles() const noexcept
      {
         return m_rects.data();
      }

      template<class RectIt>
      void rectangles(RectIt first, RectIt last)
      {
         m_rects.clear();
         for (auto it = first; it != last; ++it)
         {
            m_rects.push_back(*it);
         }
      }

      size_t rectangle_count() const noexcept
      {
         return m_rects.size();
      }

      private:
      D3D12_CPU_DESCRIPTOR_HANDLE m_cpuHandle;
      bool m_renderTargetsAquired;
      ResourceDescriptionT m_swapChainDesc;
      ViewDescriptionT m_viewDesc;

      graphics_device* m_dev_p = nullptr;
      winrt::com_ptr<i3d_render_target> m_renderTarget_up = nullptr;
      winrt::com_ptr<i3d_bridge_render_target> m_wrappedRenderTarget_up = nullptr;
      winrt::com_ptr<i2d_render_target> m_d2dTarget_up = nullptr;

      std::vector<D3D12_RECT> m_rects;
      size_t m_frmIdx;
   };
}