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
         m_frmIdx(frameIndex),
         m_state(D3D12_RESOURCE_STATE_PRESENT)
      {
         m_cpuHandle = rtvHeap.at_cpu(frameIndex);
         create();
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
         m_cpuHandle(std::move(x.m_cpuHandle)),
         m_swapChainDesc(std::move(x.m_swapChainDesc)),
         m_viewDesc(std::move(x.m_viewDesc)),
         m_dev_p(x.m_dev_p),
         m_renderTarget_p(std::move(x.m_renderTarget_p)),
         m_rects(std::move(x.m_rects)),
         m_frmIdx(x.m_frmIdx),
         irender_target(x),
         igpu_buffer(x)
      {
         x.m_dev_p = nullptr;
         x.m_renderTarget_p = nullptr;
      }

      /*
       Destructor.
       */
      virtual ~render_target()
      {
         m_dev_p = nullptr;
         m_renderTarget_p = nullptr;
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
         return m_renderTarget_p.get();
      }

      virtual i3d_render_target* get()
      {
         return m_renderTarget_p.get();
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

      const D3D12_RECT* rectangles() const noexcept
      {
         if (rectangle_count())
         {
            return m_rects.data();
         }
         return nullptr;
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


      virtual D3D12_RESOURCE_STATES state() const noexcept
      {
         return m_state;
      }

      virtual void state(D3D12_RESOURCE_STATES s) noexcept
      {
         m_state = s;
      }

      private:
      void create()
      {
         check_result(m_dev_p->swp_chn()->GetDesc1(&m_swapChainDesc));
         m_viewDesc.Format = m_swapChainDesc.Format;
         m_viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
         m_viewDesc.Texture2D.MipSlice = 0;
         m_viewDesc.Texture2D.PlaneSlice = 0;

         // Create the render target.
         check_result(m_dev_p->swp_chn()->GetBuffer(
            static_cast<UINT>(m_frmIdx),
            IID_PPV_ARGS(m_renderTarget_p.put())));

#ifdef DEBUG
         std::wstringstream stream;
         stream << L"Render Target " << m_frmIdx;
         name_d3d(m_renderTarget_p.get(), stream.str().c_str());
#endif
      }
      
      D3D12_RESOURCE_STATES m_state;
      D3D12_CPU_DESCRIPTOR_HANDLE m_cpuHandle;
      ResourceDescriptionT m_swapChainDesc;
      ViewDescriptionT m_viewDesc;

      graphics_device* m_dev_p = nullptr;
      pptr<i3d_render_target> m_renderTarget_p = nullptr;

      std::vector<D3D12_RECT> m_rects;
      size_t m_frmIdx;
   };
}