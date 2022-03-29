#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Render/qgl_irender_target.h"
#include "include/GPU/Buffers/qgl_igpu_buffer.h"
#include "include/GPU/Memory/qgl_committed_allocator.h"

#include "include/Content/Content-Descriptors/qgl_depth_stencil_descriptor.h"
#include "include/GPU/Descriptors/qgl_dsv_descriptor_heap.h"


namespace qgl::graphics::gpu
{
   /*
    Depth-stencil testing is used by the output merger to determine if a pixel
    should be drawn or not.
    Depth data is used to determine which pixels lie closest to the camera.
    Stencil data is used to mask which pixels can be updated.
    */
   class depth_stencil : public irender_target,
                         public igpu_buffer<CD3DX12_RESOURCE_DESC,
                           D3D12_DEPTH_STENCIL_VIEW_DESC,
                           gpu_resource>
   {
      public:
      using ResourceDescriptionT = CD3DX12_RESOURCE_DESC;
      using ViewDescriptionT = D3D12_DEPTH_STENCIL_VIEW_DESC;

      /*
       Constructs a depth stencil texture. Once constructed, this inserts
       itself into the DSV heap.
       */
      depth_stencil(descriptors::depth_stencil_descriptor&& buffer,
                    committed_allocator_ptr&& allocator,
                    graphics_device_ptr&& dev_p,
                    dsv_descriptor_heap& dsvHeap,
                    size_t frameIndex) :
         m_allocator_p(std::forward<committed_allocator_ptr>(allocator)),
         m_buffer(std::forward<descriptors::depth_stencil_descriptor>(buffer)),
         m_dev_p(std::forward<graphics_device_ptr>(dev_p))
      {
         m_cpuHandle = dsvHeap.at_cpu(frameIndex);
         acquire();
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
      virtual ~depth_stencil() noexcept
      {
         release();
      }

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

      virtual size_t size() const noexcept
      {
         return m_size;
      }

      virtual gpu_alloc_handle alloc_handle() const noexcept
      {
         return m_resHndl;
      }

      virtual const gpu_resource* get() const
      {
         return m_allocator_p->resource(m_resHndl);
      }

      virtual gpu_resource* get()
      {
         return m_allocator_p->resource(m_resHndl);
      }

      /*
       format of the depth stencil.
       */
      virtual DXGI_FORMAT format() const noexcept
      {
         return static_cast<DXGI_FORMAT>(m_buffer.format);
      }

      virtual D3D12_CPU_DESCRIPTOR_HANDLE where() const noexcept;

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

      void release()
      {
         m_allocator_p->free(m_resHndl);
      }

      void acquire()
      {
         m_viewDesc.Format = format();
         m_viewDesc.Flags = D3D12_DSV_FLAG_NONE;
         m_viewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

         m_desc = CD3DX12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_TEXTURE2D,
            0,
            m_dev_p->wnd()->width<UINT>(),
            m_dev_p->wnd()->height<UINT>(),
            1,
            1,
            format(),
            1,
            0,
            D3D12_TEXTURE_LAYOUT_UNKNOWN,
            D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL |
            D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);

         m_clearValue.Format = format();
         m_clearValue.DepthStencil.Depth = depth();
         m_clearValue.DepthStencil.Stencil = stencil();
         D3D12_DEPTH_STENCIL_DESC m_depthDesc;
         m_depthDesc = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
         auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

         m_resHndl = m_allocator_p->alloc(heapProps, D3D12_HEAP_FLAG_NONE,
            m_desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &m_clearValue);

         // texture width * texture height * 4 bytes per pixel.
         m_size = m_desc.Width * m_desc.Height * 4;
      }

      private:
      committed_allocator_ptr m_allocator_p;
      gpu_alloc_handle m_resHndl;
      graphics_device_ptr m_dev_p;

      size_t m_size;
      ResourceDescriptionT m_desc;
      ViewDescriptionT m_viewDesc;
      D3D12_CLEAR_VALUE m_clearValue;
      D3D12_CPU_DESCRIPTOR_HANDLE m_cpuHandle;
      descriptors::depth_stencil_descriptor m_buffer;
   };
}