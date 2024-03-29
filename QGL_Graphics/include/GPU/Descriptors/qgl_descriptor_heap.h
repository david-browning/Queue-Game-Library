#pragma once
#include "include/qgl_graphics_include.h"
#include "include/qgl_graphics_device.h"
#include "include/GPU/Buffers/qgl_igpu_buffer.h"
#include "include/GPU/Buffers/qgl_const_buffer.h"

namespace qgl::graphics::gpu
{
   /*
    Heap used to manage descriptors. Call table() to create a descriptor table
    that can be bound to a root signature.
    A descriptor heap can contain any number of resources that are the same
    type.
    Supported types are render target views, samplers, shader resource views,
    constant buffer view, and unordered access views.
    Set the Flag to D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE so a shader can
     see the heap. For DSV and RTV heaps, set this to
     D3D12_DESCRIPTOR_HEAP_FLAG_NONE.
    The heap type is ID3D12DescriptorHeap. Calling get() returns a pointer to
    that. The heap description is D3D12_DESCRIPTOR_HEAP_DESC.
    */
   template<
      D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapT,
      D3D12_DESCRIPTOR_HEAP_FLAGS Flag>
   class descriptor_heap
   {
      public:
      using dev_ptr = typename pptr<i3d_device>;
      using depth_stencil_type = typename igpu_buffer<
         CD3DX12_RESOURCE_DESC,
         D3D12_DEPTH_STENCIL_VIEW_DESC,
         igpu_resource>;
      using render_target_type = typename igpu_buffer<
         DXGI_SWAP_CHAIN_DESC1,
         D3D12_RENDER_TARGET_VIEW_DESC,
         i3d_render_target>;
      using sampler_type = typename igpu_buffer<
         D3D12_SAMPLER_DESC,
         nullptr_t,
         igpu_resource>;
      using texture_type = typename igpu_buffer<D3D12_RESOURCE_DESC,
         D3D12_SHADER_RESOURCE_VIEW_DESC, igpu_resource>;

      /*
       numEntries: Number of entries in the descriptor heap. For a DSV or RTV
        heap, set this to the number of render targets.
       nodeMask: GPU mask where to put the descriptor heap.
       */
      descriptor_heap(i3d_device* dev_p,
                      size_t numEntries,
                      gpu_idx_t nodeMask = 0) :
         m_heapSize(numEntries)
      {
         p_allocate(dev_p, nodeMask);
      }

      descriptor_heap(const descriptor_heap& r) = delete;

      descriptor_heap(descriptor_heap&&) = default;

      virtual ~descriptor_heap() noexcept = default;

      /*
       Returns a const pointer to a D3D12 Descriptor heap that can be bound to
       a command list.
       */
      const ID3D12DescriptorHeap* get() const noexcept
      {
         return m_heap_p.get();
      }

      /*
       Returns a pointer to a D3D12 Descriptor heap that can be bound to a
       command list.
       */
      ID3D12DescriptorHeap* get() noexcept
      {
         return m_heap_p.get();
      }

      /*
       Adds a shader resource view at the idx'th position in the descriptor
       heap.
       */
      void insert(graphics_device* dev_p,
                  size_t idx,
                  texture_type& textureBuffer)
      {
         auto handle = at_cpu(idx);
         dev_p->dev_3d()->CreateShaderResourceView(
            textureBuffer.get(), textureBuffer.view(), handle);
      }

      /*
       Adds a sampler view at the idx'th position in the descriptor heap.
       */
      void insert(graphics_device* dev_p,
                  size_t idx,
                  sampler_type& samplerBuffer)
      {
         auto handle = at_cpu(idx);
         dev_p->dev_3d()->CreateSampler(
            samplerBuffer.description(), handle);
      }

      /*
       Adds a depth stencil view at the idx'th position in the descriptor heap.
       */
      void insert(graphics_device* dev_p,
                  size_t idx,
                  depth_stencil_type& depthStencil)
      {
         auto handle = at_cpu(idx);
         dev_p->dev_3d()->CreateDepthStencilView(
            depthStencil.get(), depthStencil.view(), handle);
      }

      /*
       Adds a render target view at the idx'th position in the descriptor heap.
       */
      void insert(graphics_device* dev_p,
                  size_t idx,
                  render_target_type& renderTarget)
      {
         auto handle = at_cpu(idx);
         dev_p->dev_3d()->CreateRenderTargetView(
            renderTarget.get(), renderTarget.view(), handle);
      }

      /*
       Adds a shader resource view at the idx'th position in the descriptor
       heap.
       */
      template<typename T>
      void insert(graphics_device* dev_p,
                  size_t idx,
                  const const_buffer<T>& constBuffer)
      {
         auto handle = at_cpu(idx);
         dev_p->dev_3d()->CreateConstantBufferView(
            constBuffer.view(), handle);
      }

      /*
       Returns a CPU handle to the idx'th item in the heap.
       Throws std::out_of_range if the index is greater than the number of
       heap entries.
       */
      CD3DX12_CPU_DESCRIPTOR_HANDLE at_cpu(size_t idx) const
      {
         if (idx >= 0 && idx < m_heapSize)
         {
            auto ret = m_cpuStartHandle;
            return ret.Offset(static_cast<INT>(idx), static_cast<UINT>(m_descriptorSize));
         }
         else
         {
            throw std::out_of_range("The index is out of range.");
         }
      }

      /*
       Returns a GPU handle to the idx'th item in the heap.
       Throws std::out_of_range if the index is greater than the number of
       heap entries.
       */
      CD3DX12_GPU_DESCRIPTOR_HANDLE at_gpu(size_t idx) const
      {
         if (idx >= 0 && idx < m_heapSize)
         {
            auto ret = m_gpuStartHandle;
            return ret.Offset(static_cast<INT>(idx), static_cast<UINT>(m_descriptorSize));
         }
         else
         {
            throw std::out_of_range("The index is out of range.");
         }
      }

      private:
      void p_allocate(i3d_device* dev_p, gpu_idx_t nodeMask)
      {
         //Create the description
         D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
         heapDesc.Flags = Flag;
         heapDesc.NodeMask = nodeMask;
         heapDesc.NumDescriptors = static_cast<UINT>(m_heapSize);
         heapDesc.Type = DescriptorHeapT;

         m_descriptorSize =
            dev_p->GetDescriptorHandleIncrementSize(DescriptorHeapT);

         //Create a heap.
         check_result(dev_p->CreateDescriptorHeap(
            &heapDesc,
            IID_PPV_ARGS(m_heap_p.put())));
         name_d3d(m_heap_p.get(), L"Descriptor Heap");

         //Set the descriptor handles.
         m_gpuStartHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
            m_heap_p->GetGPUDescriptorHandleForHeapStart());
         m_cpuStartHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
            m_heap_p->GetCPUDescriptorHandleForHeapStart());
      }

      /*
       GPU handle to the beginning of the descriptor heap.
       */
      CD3DX12_GPU_DESCRIPTOR_HANDLE m_gpuStartHandle;

      /*
       CPU handle to the beginning of the descriptor heap.
       */
      CD3DX12_CPU_DESCRIPTOR_HANDLE m_cpuStartHandle;

      /*
       Size of a descriptor. Use this to get addresses of descriptors.
       */
      size_t m_descriptorSize;

      pptr<ID3D12DescriptorHeap> m_heap_p;

      size_t m_heapSize;
   };
}
