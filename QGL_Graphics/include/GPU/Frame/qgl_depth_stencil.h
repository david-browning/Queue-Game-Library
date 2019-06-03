#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/igpu_buffer.h"
#include "include/Content/Content-Buffers/qgl_depth_stencil_buffer.h"
#include "include/qgl_window.h"

namespace qgl::graphics::gpu
{
   class dsv_descriptor_heap;
}

namespace qgl::graphics::gpu::frame
{
   class QGL_GRAPHICS_API depth_stencil :
      public graphics::gpu::buffers::igpu_buffer<
         CD3DX12_RESOURCE_DESC,
         D3D12_DEPTH_STENCIL_VIEW_DESC,
         graphics::d3d_resource>
   {
      public:
      using ResourceDescriptionT = CD3DX12_RESOURCE_DESC;
      using ViewDescriptionT = D3D12_DEPTH_STENCIL_VIEW_DESC;

      depth_stencil(const content::buffers::DEPTH_STENCIL_BUFFER* buffer,
                    graphics::graphics_device* dev_p,
                    const graphics::window* wnd,
                    UINT frameIndex,
                    const graphics::gpu::dsv_descriptor_heap* dsvHeap);

      depth_stencil(const depth_stencil&) = delete;

      depth_stencil(depth_stencil&&) = delete;

      virtual ~depth_stencil() noexcept;

      virtual const ResourceDescriptionT* description() const;

      virtual const ViewDescriptionT* view() const;

      /*
       Format of the depth stencil.
       */
      DXGI_FORMAT format() const noexcept;

      const D3D12_DEPTH_STENCIL_DESC* depth_desc() const noexcept;

      float depth() const noexcept;

      uint8_t stencil() const noexcept;

      D3D12_CPU_DESCRIPTOR_HANDLE where() const noexcept;

      const D3D12_RECT* rectangles() const noexcept;

      void rectangles(const D3D12_RECT* rects, size_t numRects);

      size_t rectangle_count() const noexcept;

      private:
      void construct(graphics::d3d_device* dev_p);

      ResourceDescriptionT m_desc;
      ViewDescriptionT m_viewDesc;
      D3D12_CPU_DESCRIPTOR_HANDLE m_cpuHandle;
      D3D12_CLEAR_VALUE m_clearValue;
      D3D12_DEPTH_STENCIL_DESC m_depthDesc;
      D3D12_RECT* m_rects;
      size_t m_numRects;
      content::buffers::DEPTH_STENCIL_BUFFER m_buffer;
      UINT m_width;
      UINT m_height;
      UINT m_frameIndex;
   };
}