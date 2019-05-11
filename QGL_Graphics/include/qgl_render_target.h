#pragma once
#include "qgl_graphics_include.h"
#include "qgl_igpu_buffer.h"
#include "qgl_d3d_device.h"

namespace qgl::graphics
{
   class rtv_descriptor_heap;
   class frame;
   namespace low
   {
      class graphics_command_list;
   }

   class LIB_EXPORT render_target : public low::igpu_buffer<DXGI_SWAP_CHAIN_DESC1, D3D12_RENDER_TARGET_VIEW_DESC, d3d_render_target>
   {
      /*
       frameIndex is the frame from swap chain that this attaches to.
       Ex: A swap chain may have 3 frames, where the frame indices are from 0-2.
       TODO: Is there any reason why multiple things cannot bind to the same frame?
       */
      public:
      using ResourceDescriptionT = DXGI_SWAP_CHAIN_DESC1;
      using ViewDescriptionT = D3D12_RENDER_TARGET_VIEW_DESC;

      render_target(const graphics_device& dev,
                    size_t frameIndex,
                    const rtv_descriptor_heap& rtvHeap);

      render_target(const render_target& r) = delete;

      render_target(render_target&& r);

      virtual ~render_target();

      virtual ResourceDescriptionT description() const
      {
         return m_swapChainDesc;
      }

      virtual ViewDescriptionT view() const
      {
         return m_viewDesc;
      }

      DXGI_FORMAT format() const
      {
         return m_swapChainDesc.Format;
      }

      D3D12_CPU_DESCRIPTOR_HANDLE where() const
      {
         return m_handle;
      }

      const std::vector<D3D12_RECT>* rectangles() const
      {
         return m_rectangles.get();
      }

      void rectangles(std::initializer_list<D3D12_RECT>& rects);

      inline const d2d_render_target* d2d_target() const
      {
         return m_d2dRenderTarget_p.get();
      }

      inline d2d_render_target* d2d_target()
      {
         return m_d2dRenderTarget_p.get();
      }

      void acquire_resources(const winrt::com_ptr<d3d11_device>& dev_p);

      void release_resources(const winrt::com_ptr<d3d11_device>& dev_p);

      friend void swap(render_target& first, render_target& second) noexcept
      {
         using std::swap;
         swap(first.m_viewDesc, second.m_viewDesc);
         swap(first.m_swapChainDesc, second.m_swapChainDesc);
         swap(first.m_frameIndex, second.m_frameIndex);
         swap(first.m_handle, second.m_handle);
         swap(first.m_rectangles, second.m_rectangles);
         swap(first.m_wrappedRenderTarget, second.m_wrappedRenderTarget);
         swap(first.m_d2dRenderTarget_p, second.m_d2dRenderTarget_p);
         swap(static_cast<low::igpu_buffer<DXGI_SWAP_CHAIN_DESC1, D3D12_RENDER_TARGET_VIEW_DESC, d3d_render_target>&>(first),
              static_cast<low::igpu_buffer<DXGI_SWAP_CHAIN_DESC1, D3D12_RENDER_TARGET_VIEW_DESC, d3d_render_target>&>(second));

      }

      render_target& operator=(render_target r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:
      friend class graphics_command_list;
      friend class frame;

      void p_release(const winrt::com_ptr<d3d11_device>& dev_p);

      void p_allocate(const winrt::com_ptr<d3d11_device>& d3d11on12Device,
                      const winrt::com_ptr<d3d_swap_chain>& swapChain_p,
                      const winrt::com_ptr<d2d_context>& d2dContext_p,
                      float dpiX,
                      float dpiY);

      /*
       This gets populated by the constructor.
       */
      ViewDescriptionT m_viewDesc;
      ResourceDescriptionT m_swapChainDesc;

      size_t m_frameIndex;
      D3D12_CPU_DESCRIPTOR_HANDLE m_handle;

      #pragma warning(push)
      #pragma warning(disable: 4251)
      std::shared_ptr<std::vector<D3D12_RECT>> m_rectangles;
      winrt::com_ptr<d3d_wrapped_render_target> m_wrappedRenderTarget;
      winrt::com_ptr<d2d_render_target> m_d2dRenderTarget_p;
      #pragma warning(pop)

      bool m_acquired;
   };
}