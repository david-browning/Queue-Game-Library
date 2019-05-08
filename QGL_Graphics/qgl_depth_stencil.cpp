#include "pch.h"
#include "include/qgl_depth_stencil.h"
#include "include/qgl_dsv_descriptor_heap.h"
#include "include/qgl_window.h"

qgl::graphics::depth_stencil::depth_stencil(const DEPTH_STENCIL_BUFFER& buffer, 
                                            const winrt::com_ptr<d3d_device>& dev_p,
                                            const window& wnd,
                                            size_t frameIndex,
                                            const dsv_descriptor_heap& dsvHeap) :
   m_rectangles(nullptr),
   m_desc(),
   m_clearValue(),
   m_viewDesc(),
   m_buffer(buffer),
   m_depthState(),
   m_frameIndex(frameIndex),
   igpu_buffer(dev_p)
{
   m_width = wnd.width();
   m_height = wnd.height();
   m_clearValue.Format = buffer.format();
   m_clearValue.DepthStencil.Depth = buffer.depth();
   m_clearValue.DepthStencil.Stencil = buffer.stencil();

   m_handle = dsvHeap.at_cpu(frameIndex);

   m_viewDesc.Format = buffer.format();
   m_viewDesc.Flags = D3D12_DSV_FLAG_NONE;
   m_viewDesc.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;

   m_depthState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
   //m_depthState.DepthEnable = true;
   //m_depthState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
   //m_depthState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

   p_allocate();
}

qgl::graphics::depth_stencil::depth_stencil(depth_stencil&& r) :
   m_desc(std::move(r.m_desc)),
   m_viewDesc(std::move(r.m_viewDesc)),
   m_handle(std::move(r.m_handle)),
   m_clearValue(std::move(r.m_clearValue)),
   m_depthState(std::move(r.m_depthState)),
   m_rectangles(std::move(r.m_rectangles)),
   m_buffer(std::move(r.m_buffer)),
   m_width(r.m_width),
   m_height(r.m_height),
   m_frameIndex(r.m_frameIndex),
   igpu_buffer(std::move(r))
{
}

void qgl::graphics::depth_stencil::p_allocate()
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
      D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);

   auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

   winrt::check_hresult(m_dev_p->CreateCommittedResource(
      &heapProps,
      D3D12_HEAP_FLAG_NONE,
      &m_desc,
      D3D12_RESOURCE_STATE_DEPTH_WRITE,
      &m_clearValue,
      IID_PPV_ARGS(m_resource.put())));

   #ifdef DEBUG
   std::wstringstream nameStream;
   nameStream << L"Depth Stencil " << m_frameIndex;
   name_d3d(m_resource.get(), nameStream.str().c_str());

   #endif
}