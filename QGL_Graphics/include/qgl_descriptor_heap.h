#pragma once
#include "qgl_graphics_include.h"
#include "qgl_igpu_buffer.h"

#include "qgl_texture.h"
#include "qgl_render_target.h"
#include "qgl_sampler.h"
#include "qgl_constant_buffer.h"
#include "qgl_depth_stencil.h"

#include "qgl_descriptor_range.h"
#include "qgl_descriptor_table.h"
#include "qgl_igpu_buffer.h"
#include "qgl_d3d_conversions.h"

namespace qgl::graphics
{
   /*
    Heap used to manage descriptors. Call table() to create a descriptor table that can be bound to a
    root signature.
    A descriptor heap can contain any number of resources that are the same type.
    Supported types are render target views, samplers, shader resource views, constant buffer
    view, and unordered access views.
    The heap type is ID3D12DescriptorHeap. Calling get() and com_get returns a pointer to that.
    The heap description is D3D12_DESCRIPTOR_HEAP_DESC.
    */
   template<D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapT>
   class LIB_EXPORT descriptor_heap
   {
      public:
      descriptor_heap(winrt::com_ptr<d3d_device>& dev_p,
                      size_t numEntries,
                      D3D12_DESCRIPTOR_HEAP_FLAGS heapFlag,
                      UINT nodeMask = 0) :
         m_dev_p(dev_p),
         m_heap(numEntries),
         m_heap_p(nullptr),
         m_descriptorSize(0)

      {
         p_allocate(heapFlag, nodeMask);
      }

      descriptor_heap(const descriptor_heap& r) = delete;

      descriptor_heap(descriptor_heap&& r) :
         m_heap(r.m_heap),
         m_cpuStartHandle(r.m_cpuStartHandle),
         m_gpuStartHandle(r.m_gpuStartHandle),
         m_descriptorSize(r.m_descriptorSize),
         m_heap_p(std::move(r.m_heap_p)),
         m_dev_p(std::move(r.m_dev_p))
      {
      }

      virtual ~descriptor_heap()
      {

      }

      /*
       Returns a D3D12 Descriptor heap that can be bound to a command list.
       */
      inline const ID3D12DescriptorHeap* get() const
      {
         return m_heap_p.get();
      }

      inline ID3D12DescriptorHeap* get()
      {
         return m_heap_p.get();
      }

      /*
       Adds a shader resource view at the idx'th position in the descriptor heap.
       */
      void insert(size_t idx, texture& textureBuffer)
      {
         auto handle = at_cpu(idx);
         auto v = textureBuffer.view();
         m_dev_p->CreateShaderResourceView(textureBuffer.get(), &v, handle);
         m_heap[idx] = HEAP_ENTRY(low::RESOURCE_TYPES::RESOURCE_TYPE_TEXTURE);
      }

      /*
       Adds a shader resource view at the idx'th position in the descriptor heap.
       */
      template<typename T>
      void insert(size_t idx, const_buffer<T>& constBuffer)
      {
         auto handle = at_cpu(idx);
         auto v = constBuffer.view();
         m_dev_p->CreateConstantBufferView(&v, handle);
         m_heap[idx] = HEAP_ENTRY(low::RESOURCE_TYPES:::RESOURCE_TYPE_CONST_BUFFER);
      }

      /*
       Adds a sampler view at the idx'th position in the descriptor heap.
       */
      void insert(size_t idx, sampler& samplerBuffer)
      {
         auto handle = at_cpu(idx);
         auto d = samplerBuffer.description();
         m_dev_p->CreateSampler(&d, handle);
         m_heap[idx] = HEAP_ENTRY(low::RESOURCE_TYPES::RESOURCE_TYPE_SAMPLER);
      }

      /*
       Adds a depth stencil view at the idx'th position in the descriptor heap.
       */
      void insert(size_t idx, depth_stencil& depthStencil)
      {
         auto handle = at_cpu(idx);
         auto v = depthStencil.view();
         m_dev_p->CreateDepthStencilView(depthStencil.get(), &v, handle);
         m_heap[idx] = HEAP_ENTRY(low::RESOURCE_TYPES::RESOURCE_TYPE_DEPTH_STENCIL);
      }

      /*
       Adds a render target view at the idx'th position in the descriptor heap.
       */
      void insert(size_t idx, render_target& renderTarget)
      {
         auto handle = at_cpu(idx);
         auto v = renderTarget.view();
         m_dev_p->CreateRenderTargetView(renderTarget.get(), &v, handle);
         m_heap[idx] = HEAP_ENTRY(low::RESOURCE_TYPES::RESOURCE_TYPE_RENDER_TARGET);
      }

      /*
       Returns a CPU handle to the idx'th item in the heap.
       */
      CD3DX12_CPU_DESCRIPTOR_HANDLE at_cpu(size_t idx) const
      {
         if (idx >= 0 && idx < m_heap.size())
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
       */
      CD3DX12_GPU_DESCRIPTOR_HANDLE at_gpu(size_t idx) const
      {
         if (idx >= 0 && idx < m_heap.size())
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
      void p_allocate(D3D12_DESCRIPTOR_HEAP_FLAGS heapFlag,
                      UINT nodeMask)
      {
         //Create the description
         D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
         heapDesc.Flags = heapFlag;
         heapDesc.NodeMask = nodeMask;
         heapDesc.NumDescriptors = static_cast<UINT>(m_heap.size());
         heapDesc.Type = DescriptorHeapT;

         m_descriptorSize = m_dev_p->GetDescriptorHandleIncrementSize(DescriptorHeapT);

         //Create a heap.
         winrt::check_hresult(m_dev_p->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_heap_p.put())));
         name_d3d(m_heap_p.get(), L"Descriptor Heap");

         //Set the descriptor handles.
         m_gpuStartHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_heap_p->GetGPUDescriptorHandleForHeapStart());
         m_cpuStartHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_heap_p->GetCPUDescriptorHandleForHeapStart());
      }

      struct HEAP_ENTRY
      {
         HEAP_ENTRY() :
            Type(low::RESOURCE_TYPES::RESOURCE_TYPE_UNKNOWN)
         {
         }

         HEAP_ENTRY(low::RESOURCE_TYPES t) :
            Type(t)
         {
         }

         HEAP_ENTRY(const HEAP_ENTRY& r) :
            Type(r.Type)
         {
         }

         HEAP_ENTRY(HEAP_ENTRY&& r) :
            Type(r.Type)
         {
         }

         HEAP_ENTRY& operator=(const HEAP_ENTRY& r)
         {
            Type = r.Type;
            return *this;
         }

         low::RESOURCE_TYPES Type;
      };

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

      winrt::com_ptr<ID3D12DescriptorHeap> m_heap_p;

      winrt::com_ptr<d3d_device> m_dev_p;

      std::vector<HEAP_ENTRY> m_heap;
   };
}
