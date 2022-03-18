#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Descriptors/qgl_depth_stencil_descriptor.h"
#include "include/GPU/Buffers/igpu_buffer.h"
#include "include/Interfaces/qgl_igraphics_device.h"
#include "include/qgl_window.h"
#include "include/GPU/Descriptors/qgl_dsv_descriptor_heap.h"

namespace qgl::graphics::gpu
{
   /*
    Depth-stencil testing is used by the output merger to determine if a pixel
    should be drawn or not.
    Depth data is used to determine which pixels lie closest to the camera.
    Stencil data is used to mask which pixels can be updated.
    */
   class depth_stencil :
      public igpu_buffer<CD3DX12_RESOURCE_DESC, D3D12_DEPTH_STENCIL_VIEW_DESC, d3d_resource>
   {
      public:
      using ResourceDescriptionT = CD3DX12_RESOURCE_DESC;
      using ViewDescriptionT = D3D12_DEPTH_STENCIL_VIEW_DESC;

      /*
       Constructs a depth stencil texture. Once constructed, this inserts
       itself into the DSV heap.
       buffer: Describes the parameters for the depth stencil texture.
       dev_p: Used to create the depth stencil texture resource.
       dsvHeap: Once constructed, this depth stencil is bound to this depth
        stencil view descriptor heap.
       wnd_p: Provide the viewport parameters for the depth stencil texture.
       frameIndex: index where to put the view descriptor in the DSV descriptor
        heap.
       */
      depth_stencil(
         const descriptors::depth_stencil_descriptor& buffer,
         const graphics_device_ptr& dev_p,
         const window& wnd,
         dsv_descriptor_heap& dsvHeap,
         size_t frameIndex) :
         m_buffer(buffer),
         m_width(wnd.width<UINT>()),
         m_height(wnd.height<UINT>()),
         m_frameIndex(frameIndex)
      {
         m_clearValue.Format = static_cast<DXGI_FORMAT>(m_buffer.format);
         m_clearValue.DepthStencil.Depth = static_cast<float>(m_buffer.depth);
         m_clearValue.DepthStencil.Stencil = m_buffer.stencil;

         m_cpuHandle = dsvHeap.at_cpu(frameIndex);

         m_viewDesc.Format = static_cast<DXGI_FORMAT>(m_buffer.format);
         m_viewDesc.Flags = D3D12_DSV_FLAG_NONE;
         m_viewDesc.ViewDimension =
            D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;

         m_depthDesc = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

         construct(dev_p->d3d12_device());

         dsvHeap.insert(dev_p, frameIndex, *this);
      }

      /*
       Do not allow copying because only one depth stencil can bind to any one
       descriptor slot.
       */
      depth_stencil(const depth_stencil&) = delete;

      /*
       Move constructor.
       */
      depth_stencil(depth_stencil&&) = default;

      /*
       Destructor.
       */
      virtual ~depth_stencil() noexcept = default;

      /*
       Describes the Z-Buffer texture.
       */
      virtual const ResourceDescriptionT* description() const
      {
         return &m_desc;
      }

      /*
       The view description that gets bound to a depth stencil view descriptor
       heap.
       */
      virtual const ViewDescriptionT* view() const
      {
         return &m_viewDesc;
      }

      /*
       format of the depth stencil.
       */
      DXGI_FORMAT format() const noexcept
      {
         return static_cast<DXGI_FORMAT>(m_buffer.format);
      }

      const D3D12_DEPTH_STENCIL_DESC& depth_desc() const noexcept
      {
         return m_depthDesc;
      }

      /*
       Value to use when clearing the depth buffer.
       */
      float depth() const noexcept
      {
         return static_cast<float>(m_buffer.depth);
      }

      /*
       Value to use when clearing the stencil buffer.
       */
      uint8_t stencil() const noexcept
      {
         return m_buffer.stencil;
      }

      /*
       Describes where, in the DSV descriptor heap, the depth stencil view is.
       */
      D3D12_CPU_DESCRIPTOR_HANDLE where() const noexcept
      {
         return m_cpuHandle;
      }

      /*
       Returns a pointer to an array of rectangles.
       */
      const D3D12_RECT* rectangles() const noexcept;

      /*
       Sets the rectangles for the depth stencil.
       */
      template<class ForwardIt>
      void rectangles(ForwardIt first, ForwardIt last);

      /*
       Returns the number of rectangles.
       */
      size_t rectangle_count() const noexcept
      {
         return m_rects.size();
      }

      private:
      void construct(graphics::d3d_device* dev_p)
      {
         m_desc = CD3DX12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_TEXTURE2D,
            0,
            static_cast<UINT>(m_width),
            static_cast<UINT>(m_height),
            1,
            1,
            static_cast<DXGI_FORMAT>(m_buffer.format),
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

      ResourceDescriptionT m_desc;
      ViewDescriptionT m_viewDesc;

      D3D12_CPU_DESCRIPTOR_HANDLE m_cpuHandle;

      D3D12_CLEAR_VALUE m_clearValue;
      D3D12_DEPTH_STENCIL_DESC m_depthDesc;

      std::vector<D3D12_RECT> m_rects;

      descriptors::depth_stencil_descriptor m_buffer;

      UINT m_width;
      UINT m_height;
      size_t m_frameIndex;
   };
}