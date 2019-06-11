#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/igpu_buffer.h"
#include "include/Interfaces/qgl_igraphics_device.h"

namespace qgl::graphics::gpu::buffers
{
   template<typename IndexT>
   class index_buffer : public igpu_buffer<D3D12_SUBRESOURCE_DATA,
      D3D12_INDEX_BUFFER_VIEW,
      d3d_resource>
   {
      //Index size cannot only be 1, 2, or 4 bytes.
      static_assert(std::is_integral<IndexT>::value && (
         sizeof(IndexT) == sizeof(uint8_t) ||
         sizeof(IndexT) == sizeof(uint16_t) ||
         sizeof(IndexT) == sizeof(uint32_t)));

      public:
      using ResourceDescriptionT = D3D12_SUBRESOURCE_DATA;
      using ViewDescriptionT = D3D12_INDEX_BUFFER_VIEW;

      index_buffer(graphics::igraphics_device* gdev,
                   const IndexT* indicies,
                   size_t numIndicies) :
         m_viewDesc(),
         m_desc(),
         m_indexDataSize(sizeof(IndexT) * numIndicies)
      {
         m_indices.resize(numIndicies);
         MemoryCopy(m_indices.data(), indicies, numIndicies);

         construct(gdev);
      }

      index_buffer(graphics::igraphics_device* gdev,
                   const std::vector<IndexT>& indicies) :
         m_viewDesc(),
         m_desc(),
         m_indexDataSize(sizeof(IndexT) * indicies.size()),
         m_indices(m_indices)
      {
         construct(gdev);
      }

      /*
       Index buffers should be unique.
       */
      index_buffer(const index_buffer&) = delete;

      /*
       Move constructor.
       */
      index_buffer(index_buffer&&) = default;

      virtual ~index_buffer() noexcept = default;

      /*
       Returns a description of the resource.
       */
      virtual const ResourceDescriptionT* description() const
      {
         return &m_desc;
      }

      /*
       Returns a view pointer that can be bound to the shader pipeline.
       */
      virtual const ViewDescriptionT* view() const
      {
         return &m_viewDesc;
      }

      /*
       Returns the DXGI_FORMAT that this index buffer uses.
       */
      constexpr DXGI_FORMAT index_format() const noexcept
      {
         switch (sizeof(IndexT))
         {
            case (sizeof(uint8_t)):
            {
               return DXGI_FORMAT_R8_UINT;
            }
            case (sizeof(uint16_t)):
            {
               return DXGI_FORMAT_R16_UINT;
            }
            case (sizeof(uint32_t)):
            {
               return DXGI_FORMAT_R32_UINT;
            }
         }
      }

      private:
      void construct(graphics::igraphics_device* gdev)
      {
         m_desc.pData = m_indices.data();
         m_desc.RowPitch = m_indexDataSize;
         m_desc.SlicePitch = m_desc.RowPitch;

         //Create the heap for the resource.
         winrt::check_hresult(gdev->d3d12_device()->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(m_indexDataSize),
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(put())));

         m_viewDesc.BufferLocation = get()->GetGPUVirtualAddress();
         m_viewDesc.Format = index_format();
         m_viewDesc.SizeInBytes = m_indexDataSize;
      }

      ResourceDescriptionT m_desc;
      ViewDescriptionT m_viewDesc;
      std::vector<IndexT> m_indices;
      size_t m_indexDataSize;
   };
}