#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/igpu_buffer.h"
#include "include/Content/Content-Buffers/qgl_depth_stencil_buffer.h"

namespace qgl::graphics::content
{
   class dsv_descriptor_heap;
   class window;

   class QGL_GRAPHICS_API depth_stencil :
      public gpu::buffers::igpu_buffer<CD3DX12_RESOURCE_DESC,
      D3D12_DEPTH_STENCIL_VIEW_DESC,
      d3d_resource>
   {
      public:
      using ResourceDescriptionT = CD3DX12_RESOURCE_DESC;
      using ViewDescriptionT = D3D12_DEPTH_STENCIL_VIEW_DESC;

      depth_stencil(const content::buffers::DEPTH_STENCIL_BUFFER* buffer,
                    d3d_device* dev_p,
                    const window* wnd,
                    size_t frameIndex,
                    const dsv_descriptor_heap& dsvHeap);

      depth_stencil(const depth_stencil&);

      depth_stencil(depth_stencil&&);

      virtual ~depth_stencil() noexcept;

      virtual const ResourceDescriptionT* description() const;

      virtual const ViewDescriptionT* view() const;

      DXGI_FORMAT format() const noexcept;

      const D3D12_DEPTH_STENCIL_DESC* depth_state() const noexcept;

      float depth() const noexcept;

      uint8_t stencil() const noexcept;

      D3D12_CPU_DESCRIPTOR_HANDLE where() const noexcept;

      const D3D12_RECT* rectangles() const noexcept;

      void rectangles(const D3D12_RECT* rects, size_t numRects);

      private:
      struct impl;
      impl* m_impl_p;

      ResourceDescriptionT m_desc;
      ViewDescriptionT m_viewDesc;
      D3D12_CPU_DESCRIPTOR_HANDLE m_handle;
      D3D12_CLEAR_VALUE m_clearValue;
      D3D12_DEPTH_STENCIL_DESC m_depthState;
      std::vector<D3D12_RECT> m_rectangles;
      content::buffers::DEPTH_STENCIL_BUFFER m_buffer;
      size_t m_width;
      size_t m_height;
      size_t m_frameIndex;
   };
}