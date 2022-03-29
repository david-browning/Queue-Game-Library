#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/qgl_igpu_buffer.h"
#include "include/GPU/Memory/qgl_igpu_allocator.h"

namespace qgl::graphics::gpu
{
   /*
    Defines the index data.
    This does not upload the index buffer to the GPU. Pass this to an
    "upload_buffer" to upload the data this manages.
    */
   template<typename IndexT>
   class index_buffer : public igpu_buffer<D3D12_SUBRESOURCE_DATA,
      D3D12_INDEX_BUFFER_VIEW,
      gpu_resource>
   {
      //index size cannot only be 1, 2, or 4 bytes.
      static_assert(std::is_integral<IndexT>::value && (
         sizeof(IndexT) == sizeof(uint8_t) ||
         sizeof(IndexT) == sizeof(uint16_t) ||
         sizeof(IndexT) == sizeof(uint32_t)),
         "IndexT must be an unsigned 1, 2, or 4 byte integral.");

      public:
      using ResDescT = D3D12_SUBRESOURCE_DATA;
      using ViewDescT = D3D12_INDEX_BUFFER_VIEW;
      using index_data = typename std::vector<IndexT>;

      index_buffer(gpu_allocator_ptr&& allocator, index_data&& indicies) :
         m_indices(std::forward<index_data>(indicies)),
         m_allocator_p(std::forward<gpu_allocator_ptr>(allocator))
      {
         construct();
      }

      index_buffer(gpu_allocator_ptr&& allocator,
         const IndexT* const indexData, size_t indexCount) :
         m_allocator_p(std::forward<gpu_allocator_ptr>(allocator))
      {
         m_indices.resize(indexCount);
         memcpy(m_indices.data(), indexData, sizeof(IndexT) * indexCount);
         construct();
      }

      /*
       index buffers should be unique.
       */
      index_buffer(const index_buffer&) = delete;

      /*
       Move constructor.
       */
      index_buffer(index_buffer&&) = default;

      virtual ~index_buffer() noexcept
      {
         m_allocator_p->free(m_resHndl);
      }

      virtual const ResDescT* description() const
      {
         return &m_resDescription;
      }

      virtual const ViewDescT* view() const
      {
         return &m_viewDescription;
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
       Returns the size of the index buffer in bytes.
       */
      virtual size_t size() const noexcept
      {
         return sizeof(IndexT) * m_indices.size();
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
      void construct()
      {
         m_resDescription.pData = m_indices.data();
         m_resDescription.RowPitch = size();
         m_resDescription.SlicePitch = m_resDescription.RowPitch;

         // TODO: if allocating index buffers fails, may need to restrict this 
         // to use a committed resource allocator.
         //winrt::check_hresult(gdev->d3d12_device()->CreateCommittedResource(
         //   &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
         //   D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
         //   &CD3DX12_RESOURCE_DESC::Buffer(m_indexDataSize),
         //   D3D12_RESOURCE_STATE_COPY_DEST,
         //   nullptr,
         //   IID_PPV_ARGS(put())));

         m_resHndl = m_allocator_p->alloc(
            size(),
            0,
            CD3DX12_RESOURCE_DESC::Buffer(size()),
            D3D12_RESOURCE_STATE_COPY_DEST);

         auto res_p = m_allocator_p->resource(m_resHndl);
         m_viewDescription.BufferLocation = res_p->GetGPUVirtualAddress();
         m_viewDescription.Format = index_format();
         m_viewDescription.SizeInBytes = size();
      }

      index_data m_indices;
      gpu_allocator_ptr m_allocator_p;
      gpu_alloc_handle m_resHndl;
      ViewDescT m_viewDescription;
      ResDescT m_resDescription;
   };
}