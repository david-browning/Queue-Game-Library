#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Render/qgl_irender_target.h"
#include "include/GPU/Buffers/qgl_igpu_buffer.h"
#include "include/GPU/Memory/qgl_committed_allocator.h"
#include "include/Descriptors/qgl_depth_stencil_descriptor.h"
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
      igpu_resource>
   {
      public:
      using ResourceDescriptionT = CD3DX12_RESOURCE_DESC;
      using ViewDescriptionT = D3D12_DEPTH_STENCIL_VIEW_DESC;

      /*
       Constructs a depth stencil texture. Once constructed, this inserts
       itself into the DSV heap.
       This does not own the graphics device or allocator pointers.
       Do not free those items, or let them go out of scope, before destroying
       this.
       */
      depth_stencil(const descriptors::depth_stencil_descriptor& buffer,
                    committed_allocator* allocator_p,
                    graphics_device* dev_p,
                    dsv_descriptor_heap& dsvHeap,
                    size_t frameIndex) :
         m_allocator_p(allocator_p),
         m_buffer(buffer),
         m_dev_p(dev_p),
         m_frameIndex(frameIndex)
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
      depth_stencil(depth_stencil&& x) noexcept :
         irender_target(std::move(x)),
         igpu_buffer(std::move(x)),
         m_allocator_p(x.m_allocator_p),
         m_alloc_h(x.m_alloc_h),
         m_dev_p(std::move(x.m_dev_p)),
         m_depthDesc(std::move(x.m_depthDesc)),
         m_rects(std::move(x.m_rects)),
         m_frameIndex(x.m_frameIndex),
         m_size(x.m_size),
         m_desc(std::move(x.m_desc)),
         m_viewDesc(std::move(x.m_viewDesc)),
         m_clearValue(std::move(x.m_clearValue)),
         m_cpuHandle(std::move(x.m_cpuHandle)),
         m_buffer(std::move(x.m_buffer))
      {
         x.m_allocator_p = nullptr;
         x.m_dev_p = nullptr;
         x.m_frameIndex = static_cast<size_t>(-1);
         x.m_size = static_cast<size_t>(-1);
      }

      /*
       Destructor.
       */
      virtual ~depth_stencil() noexcept
      {
         release();
         m_allocator_p = nullptr;
         m_dev_p = nullptr;
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

      D3D12_DEPTH_STENCIL_DESC depth_desc() const noexcept
      {
         return m_depthDesc;
      }

      virtual size_t size() const noexcept
      {
         return m_size;
      }

      virtual gpu_alloc_handle alloc_handle() const noexcept
      {
         return m_alloc_h;
      }

      virtual const igpu_resource* get() const
      {
         return m_allocator_p->resource(m_alloc_h);
      }

      virtual igpu_resource* get()
      {
         return m_allocator_p->resource(m_alloc_h);
      }

      /*
       format of the depth stencil.
       */
      virtual DXGI_FORMAT format() const noexcept
      {
         return static_cast<DXGI_FORMAT>(m_buffer.format);
      }

      virtual D3D12_CPU_DESCRIPTOR_HANDLE where() const noexcept
      {
         return m_cpuHandle;
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

      void release()
      {
         if (m_allocator_p)
         {
            m_allocator_p->free(m_alloc_h);
         }
      }

      void acquire()
      {
         m_viewDesc.Format = format();
         m_viewDesc.Flags = D3D12_DSV_FLAG_NONE;
         m_viewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

         m_desc = CD3DX12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_TEXTURE2D,
            0,
            helpers::dip_to_pixels(m_dev_p->wnd()->width(),
                                   m_dev_p->wnd()->dpi_x()),
            helpers::dip_to_pixels(m_dev_p->wnd()->height(),
                                   m_dev_p->wnd()->dpi_y()),
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
         auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

         m_depthDesc.StencilWriteMask = m_buffer.stencil_write_mask;
         m_depthDesc.StencilReadMask = m_buffer.stencil_read_mask;
         m_depthDesc.StencilEnable = m_buffer.stencil_enabled();
         m_depthDesc.DepthWriteMask =
            static_cast<D3D12_DEPTH_WRITE_MASK>(m_buffer.write_mask);
         m_depthDesc.DepthFunc =
            static_cast<D3D12_COMPARISON_FUNC>(m_buffer.depth_func);
         m_depthDesc.DepthEnable = m_buffer.depth_enabled();

         m_depthDesc.BackFace.StencilFunc =
            static_cast<D3D12_COMPARISON_FUNC>(m_buffer.back_face_op.func);
         m_depthDesc.BackFace.StencilDepthFailOp =
            static_cast<D3D12_STENCIL_OP>(m_buffer.back_face_op.depth_fail_op);
         m_depthDesc.BackFace.StencilFailOp =
            static_cast<D3D12_STENCIL_OP>(m_buffer.back_face_op.fail_op);
         m_depthDesc.BackFace.StencilPassOp =
            static_cast<D3D12_STENCIL_OP>(m_buffer.back_face_op.pass_op);

         m_depthDesc.FrontFace.StencilFunc =
            static_cast<D3D12_COMPARISON_FUNC>(m_buffer.front_face_op.func);
         m_depthDesc.FrontFace.StencilDepthFailOp =
            static_cast<D3D12_STENCIL_OP>(m_buffer.front_face_op.depth_fail_op);
         m_depthDesc.FrontFace.StencilFailOp =
            static_cast<D3D12_STENCIL_OP>(m_buffer.front_face_op.fail_op);
         m_depthDesc.FrontFace.StencilPassOp =
            static_cast<D3D12_STENCIL_OP>(m_buffer.front_face_op.pass_op);

         m_alloc_h = m_allocator_p->alloc(
            m_desc, D3D12_RESOURCE_STATE_DEPTH_WRITE);

         // texture width * texture height * 4 bytes per pixel.
         m_size = m_desc.Width * m_desc.Height * 4;
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
      committed_allocator* m_allocator_p = nullptr;
      gpu_alloc_handle m_alloc_h = static_cast<gpu_alloc_handle>(-1);
      graphics_device* m_dev_p = nullptr;
      D3D12_DEPTH_STENCIL_DESC m_depthDesc;

      std::vector<D3D12_RECT> m_rects;
      size_t m_frameIndex = static_cast<size_t>(-1);
      size_t m_size = static_cast<size_t>(-1);
      ResourceDescriptionT m_desc;
      ViewDescriptionT m_viewDesc;
      D3D12_CLEAR_VALUE m_clearValue;
      D3D12_CPU_DESCRIPTOR_HANDLE m_cpuHandle;
      descriptors::depth_stencil_descriptor m_buffer;
   };
}