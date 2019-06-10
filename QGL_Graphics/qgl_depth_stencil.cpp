#include "pch.h"
#include "include/GPU/Render/qgl_depth_stencil.h"
#include "include/qgl_window.h"
#include "include/Interfaces/qgl_igraphics_device.h"
#include "include/GPU/Descriptors/qgl_dsv_descriptor_heap.h"

namespace qgl::graphics::gpu::render
{
   depth_stencil::depth_stencil(
      const content::buffers::DEPTH_STENCIL_BUFFER* buffer,
      graphics::igraphics_device* dev_p,
      const graphics::window* wnd,
      UINT frameIndex,
      const graphics::gpu::dsv_descriptor_heap* dsvHeap) :
      m_buffer(*buffer),
      m_width(wnd->width()),
      m_height(wnd->height()),
      m_frameIndex(frameIndex),
      m_rects(nullptr),
      m_numRects(0)
   {
      m_clearValue.Format = m_buffer.format();
      m_clearValue.DepthStencil.Depth = m_buffer.depth();
      m_clearValue.DepthStencil.Stencil = m_buffer.stencil();

      m_cpuHandle = dsvHeap->at_cpu(frameIndex);

      m_viewDesc.Format = m_buffer.format();
      m_viewDesc.Flags = D3D12_DSV_FLAG_NONE;
      m_viewDesc.ViewDimension =
         D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;

      m_depthDesc = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

      construct(dev_p->d3d12_device());
   }

   depth_stencil::~depth_stencil() noexcept
   {
      delete[] m_rects;
   }

   const depth_stencil::ResourceDescriptionT* depth_stencil::description() const
   {
      return &m_desc;
   }

   const depth_stencil::ViewDescriptionT* depth_stencil::view() const
   {
      return &m_viewDesc;
   }

   DXGI_FORMAT depth_stencil::format() const noexcept
   {
      return m_buffer.format();
   }

   const D3D12_DEPTH_STENCIL_DESC* depth_stencil::depth_desc() const noexcept
   {
      return &m_depthDesc;
   }

   float depth_stencil::depth() const noexcept
   {
      return m_buffer.depth();
   }

   uint8_t depth_stencil::stencil() const noexcept
   {
      return m_buffer.stencil();
   }

   D3D12_CPU_DESCRIPTOR_HANDLE depth_stencil::where() const noexcept
   {
      return m_cpuHandle;
   }

   const D3D12_RECT* depth_stencil::rectangles() const noexcept
   {
      return m_rects;
   }

   void depth_stencil::rectangles(const D3D12_RECT* rects,
                                  size_t numRects)
   {
      delete[] m_rects;
      m_numRects = numRects;
      m_rects = new D3D12_RECT[numRects];

      memcpy(m_rects, rects, sizeof(D3D12_RECT) * numRects);
   }

   size_t depth_stencil::rectangle_count() const noexcept
   {
      return m_numRects;
   }

   void depth_stencil::construct(graphics::d3d_device* dev_p)
   {
      m_desc = CD3DX12_RESOURCE_DESC(
         D3D12_RESOURCE_DIMENSION_TEXTURE2D,
         0,
         static_cast<UINT>(m_width),
         static_cast<UINT>(m_height),
         1,
         1,
         m_buffer.format(),
         1,
         0,
         D3D12_TEXTURE_LAYOUT_UNKNOWN,
         D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL |
         D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);

      auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

      winrt::check_hresult(dev_p->CreateCommittedResource(
         &heapProps,
         D3D12_HEAP_FLAG_NONE,
         &m_desc,
         D3D12_RESOURCE_STATE_DEPTH_WRITE,
         &m_clearValue,
         IID_PPV_ARGS(put())));

      #ifdef DEBUG
      std::wstringstream nameStream;
      nameStream << L"Depth Stencil " << m_frameIndex;
      graphics::name_d3d(this->get(), nameStream.str().c_str());

      #endif
   }
}