#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/igpu_buffer.h"

namespace qgl::graphics
{
   class igraphics_device;
}

namespace qgl::graphics::gpu
{
   class rtv_descriptor_heap;
   class graphics_command_list;
}

namespace qgl::graphics::gpu::render
{
   class QGL_GRAPHICS_API render_target : 
      public buffers::igpu_buffer<DXGI_SWAP_CHAIN_DESC1,
      D3D12_RENDER_TARGET_VIEW_DESC, 
      d3d_render_target>
   {
      public:
      using ResourceDescriptionT = DXGI_SWAP_CHAIN_DESC1;
      using ViewDescriptionT = D3D12_RENDER_TARGET_VIEW_DESC;

      /*
       frameIndex is the render from swap chain that this attaches to.
       Ex: A swap chain may have 3 frames, where the render indices are 
       from 0-2.
       TODO: Is there any reason why multiple things cannot bind to the same 
       render?
       */
      render_target(graphics::igraphics_device* dev,
                    const rtv_descriptor_heap* rtvHeap,
                    size_t frameIndex);

      /*
       Copy constructor.
       */
      render_target(const render_target&) = delete;

      /*
       Move constructor.
       */
      render_target(render_target&&);

      /*
       Destructor.
       */
      virtual ~render_target();

      /*
       Returns a pointer to the swap chain description.
       */
      virtual const ResourceDescriptionT* description() const;

      /*
       Returns a pointer to the view description so this can be bound to
       the graphics pipeline.
       */
      virtual const ViewDescriptionT* view() const;

      /*
       Returns the render target format. This is decided by the swap chain.
       */
      DXGI_FORMAT format() const noexcept;

      void rectangles(const D3D12_RECT* rects,
                      size_t rectCount);
      
      const D3D12_RECT* rectangles() const noexcept;

      /*
       Returns the number of rectangles.
       */
      size_t rectangle_count() const noexcept;

      D3D12_CPU_DESCRIPTOR_HANDLE where() const;
      
      /*
       Returns the 2D render target used for Direct2D.
       */
      const d2d_render_target* d2d_target() const noexcept;

      private:
      friend class frame;
      friend class graphics_command_list;

      void construct(d3d11_device* d3d11on12Device,
                     d3d_swap_chain* swapChain_p,
                     d2d_context* d2dContext_p,
                     float dpiX,
                     float dpiY);
            
      /*
       Acquires resources so this render target can be used.
       */
      void acquire_resources(d3d11_device* dev_p);

      /*
       Releases acquired resources so other render targets can be used.
       */
      void release_resources(d3d11_device* dev_p);

      /*
       Releases all resources so that the render can be rebuilt. This only needs
       to be called when the frames need to be resized.
       */
      void dispose(d3d11_device* dev_p);

      ViewDescriptionT m_viewDesc;
      ResourceDescriptionT m_swapChainDesc;
      size_t m_frameIndex;
      D3D12_CPU_DESCRIPTOR_HANDLE m_handle;
      bool m_acquired;

      D3D12_RECT* m_rects;
      size_t m_numRects;
      winrt::com_ptr<d3d_wrapped_render_target> m_wrappedRenderTarget_p;
      winrt::com_ptr<d2d_render_target> m_d2dTarget_p;
   };
}