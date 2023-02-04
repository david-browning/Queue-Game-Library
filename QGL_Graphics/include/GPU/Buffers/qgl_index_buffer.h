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
   class index_buffer final : public igpu_buffer<
      D3D12_SUBRESOURCE_DATA,
      D3D12_INDEX_BUFFER_VIEW,
      igpu_resource>
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

      /*
       This does not own the allocator pointer. Do not free the allocator or let
       it go out of scope before destroying this.
       */
      index_buffer(igpu_allocator* allocator_p,
                   const index_data& indicies) :
         m_indices(indicies),
         m_allocator_p(allocator_p),
         igpu_buffer()
      {
         construct();
      }

      /*
       This does not own the allocator pointer. Do not free the allocator or let
       it go out of scope before destroying this.
       */
      index_buffer(igpu_allocator* allocator_p,
                   const IndexT* const indexData,
                   size_t indexCount) :
         m_allocator_p(allocator_p),
         igpu_buffer()
      {
         m_indices.resize(indexCount);
         memcpy(m_indices.data(), indexData, sizeof(IndexT)* indexCount);
         construct();
      }

      /*
       index buffers should be unique.
       */
      index_buffer(const index_buffer&) = delete;

      /*
       Move constructor.
       */
      index_buffer(index_buffer&& x) noexcept :
         m_allocator_p(std::move(x.m_allocator_p)),
         m_indices(std::move(x.m_indices)),
         m_alloc_h(std::move(x.m_alloc_h)),
         m_viewDescription(std::move(x.m_viewDescription)),
         m_resDescription(std::move(x.m_resDescription)),
         igpu_buffer(std::move(x))
      {
         x.m_allocator_p = nullptr;
      }

      virtual ~index_buffer() noexcept
      {
         if (m_allocator_p)
         {
            m_allocator_p->free(m_alloc_h);
            m_allocator_p = nullptr;
         }
      }

      virtual const ResDescT* description() const
      {
         return &m_resDescription;
      }

      virtual const ViewDescT* view() const
      {
         return &m_viewDescription;
      }

      virtual gpu_alloc_handle alloc_handle() const
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
       Returns the size of the index buffer in bytes.
       */
      virtual size_t size() const noexcept
      {
         return sizeof(IndexT) * m_indices.size();
      }

      size_t count() const noexcept
      {
         return m_indices.size();
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

      virtual D3D12_RESOURCE_STATES state() const noexcept
      {
         return m_state;
      }

      virtual void state(D3D12_RESOURCE_STATES s) noexcept
      {
         m_state = s;
      }

      private:
      void construct()
      {
         m_resDescription.pData = m_indices.data();
         m_resDescription.RowPitch = size();
         m_resDescription.SlicePitch = m_resDescription.RowPitch;

         m_alloc_h = m_allocator_p->alloc(CD3DX12_RESOURCE_DESC::Buffer(size()));

         m_viewDescription.BufferLocation = get()->GetGPUVirtualAddress();
         m_viewDescription.Format = index_format();
         m_viewDescription.SizeInBytes = size();
      }

      index_data m_indices;
      ViewDescT m_viewDescription;
      ResDescT m_resDescription;
      igpu_allocator* m_allocator_p = nullptr;
      gpu_alloc_handle m_alloc_h = static_cast<gpu_alloc_handle>(-1);
      D3D12_RESOURCE_STATES m_state = D3D12_RESOURCE_STATE_COPY_DEST;
   };
}