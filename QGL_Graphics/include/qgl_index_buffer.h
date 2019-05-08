#pragma once
#include "qgl_graphics_include.h"
#include "qgl_igpu_buffer.h"

namespace qgl::graphics
{
   class low::copy_command_list;

   template<typename index_t>
   class index_buffer : public low::igpu_buffer<D3D12_SUBRESOURCE_DATA, D3D12_INDEX_BUFFER_VIEW, d3d_resource>
   {
      friend class low::copy_command_list;

      //Index size cannot only be 1, 2, or 4 bytes.
      static_assert(std::is_integral<index_t>::value && (
         sizeof(index_t) == sizeof(uint8_t) ||
         sizeof(index_t) == sizeof(uint16_t) ||
         sizeof(index_t) == sizeof(uint32_t)));

      public:
      using ResourceDescriptionT = D3D12_SUBRESOURCE_DATA;
      using ViewDescriptionT = D3D12_INDEX_BUFFER_VIEW;

      index_buffer(winrt::com_ptr<d3d_device>& dev_p,
                   std::vector<index_t>& indices) :
         m_indices(indices),
         m_indexDataSize(sizeof(index_t) * indices.size()),
         m_viewDesc(),
         m_desc(),
         igpu_buffer(dev_p)
      {
         p_allocate();
      }

      index_buffer(winrt::com_ptr<d3d_device>& dev_p,
                   std::vector<index_t>&& indices) :
         m_indices(indices),
         m_indexDataSize(sizeof(index_t) * indices.size()),
         m_viewDesc(),
         m_desc(),
         igpu_buffer(dev_p)
      {
         p_allocate();
      }

      index_buffer(const index_buffer& r) = delete;

      index_buffer(index_buffer&& r) = delete;

      virtual ~index_buffer()
      {
      }

      virtual ResourceDescriptionT description() const
      {
         return m_desc;
      }

      virtual ViewDescriptionT view() const
      {
         return m_viewDesc;
      }

      /*
       Returns the DXGI_FORMAT that this index buffer uses.
       */
      constexpr DXGI_FORMAT index_format() const
      {
         switch (sizeof(index_t))
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
      /*
       Creates the committed resource for the index buffer. This does not actually populate the
       buffer.
       */
      virtual void p_allocate()
      {
         m_desc.pData = m_indices.data();
         m_desc.RowPitch = m_indexDataSize;
         m_desc.SlicePitch = m_desc.RowPitch;

         //Create the heap for the resource.
         winrt::check_hresult(m_dev_p->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(m_indexDataSize),
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(m_resource.put())));

         m_viewDesc.BufferLocation = m_resource->GetGPUVirtualAddress();
         m_viewDesc.Format = index_format();
         m_viewDesc.SizeInBytes = m_indexDataSize;

      }

      std::vector<index_t> m_indices;
      size_t m_indexDataSize;
      ResourceDescriptionT m_desc;
      ViewDescriptionT m_viewDesc;
   };
}