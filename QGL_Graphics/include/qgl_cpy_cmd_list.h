#pragma once
#include "qgl_graphics_include.h"
#include "qgl_icmd_list.h"
#include "qgl_descriptor_heap.h"
#include "qgl_vertex_buffer.h"
#include "qgl_index_buffer.h"
#include "qgl_constant_buffer.h"
#include "qgl_texture.h"
#include "qgl_pso.h"
#include "qgl_buffer_allocator.h"

namespace qgl::graphics::low
{
   class LIB_EXPORT copy_command_list : public low::icommand_list<D3D12_COMMAND_LIST_TYPE_DIRECT>
   {
      public:
      copy_command_list(const winrt::com_ptr<d3d_device>& dev_p,
                        pipeline_state& pipelineState_p,
                        UINT nodeMask = 0);

      copy_command_list(const copy_command_list& r) = delete;

      copy_command_list(copy_command_list&& r) = delete;

      virtual ~copy_command_list();

      virtual void root_sig(const root_signature&)
      {
         throw std::runtime_error("Copy command lists do not support root signatures.");
      }

      virtual void table(const descriptor_table&)
      {
         throw std::runtime_error("Copy command lists do not support descriptor tables.");
      }

      template<typename VertexT>
      void copy(vertex_buffer<VertexT>& toCopy)
      {
         //Verify that the buffer is in the correct state to copy data to it.
         if (toCopy.state() == D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST)
         {
            if (toCopy.state() != D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER)
            {
               //Create an upload buffer.
               auto gpuAddr = m_bufferAllocator.allocate(toCopy.m_vertexBufferSize);
               auto uploadHeap = m_bufferAllocator.buffer(gpuAddr);

               //Copy from the upload heap and then schedule a copy from the upload heap
               //to the vertex buffer's resource heap.
               UpdateSubresources<1>(m_cmdList_p.get(),
                                     toCopy.get(),
                                     uploadHeap.get(),
                                     0, 0, 1,
                                     &toCopy.description());

               //Transition the resource to the VS buffer state.
               m_pendingTransitions.push_back(
                  CD3DX12_RESOURCE_BARRIER::Transition(toCopy.get(),
                                                       toCopy.state(),
                                                       D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

               toCopy.state(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
            }
         }
         else
         {
            throw std::invalid_argument("The buffer must be in the COPY_DEST state."
                                        "vertex_buffer is in this state when initialized.");
         }
      }

      template<typename IndexT>
      void copy(index_buffer<IndexT>& toCopy)
      {
         //Verify that the buffer is in the correct state to copy data to it.
         if (toCopy.state() == D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST)
         {
         //Only copy the index buffer if it is not in a state for index buffers.
            if (toCopy.state() != D3D12_RESOURCE_STATE_INDEX_BUFFER)
            {
               //Create an upload buffer
               auto gpuAddr = m_bufferAllocator.allocate(toCopy.m_indexDataSize);
               auto uploadHeap = m_bufferAllocator.buffer(gpuAddr);

               // Copy data to the intermediate upload heap and then schedule a copy 
               // from the upload heap to the index buffer.
               UpdateSubresources<1>(m_cmdList_p.get(),
                                     toCopy.get(),
                                     uploadHeap.get(),
                                     0, 0, 1,
                                     &toCopy.description());

               //Transition the resource to the correct state.
               m_pendingTransitions.push_back(
                  CD3DX12_RESOURCE_BARRIER::Transition(toCopy.get(),
                                                       toCopy.state(),
                                                       D3D12_RESOURCE_STATE_INDEX_BUFFER));

               //Update the object's state.
               toCopy.state(D3D12_RESOURCE_STATE_INDEX_BUFFER);
            }
         }
         else
         {
            throw std::invalid_argument("The buffer must be in the COPY_DEST state."
                                        "index_buffer is in this state when initialized.");
         }
      }

      virtual void begin();

      virtual void flush();

      private:
      void p_flush_pending_transitions();

      /*
       Used to allocate upload heaps.
       */
      buffer_allocator<BUFFER_ALIGN,
         D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
         D3D12_HEAP_TYPE_UPLOAD> m_bufferAllocator;

      /*
       Used to allocate texture upload heaps.
       */
      buffer_allocator<TEXTURE_MS_ALIGN,
         D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
         D3D12_HEAP_TYPE_UPLOAD> m_textureAllocator;

      #pragma warning(push)
      #pragma warning(disable: 4251)
      std::vector<D3D12_RESOURCE_BARRIER> m_pendingTransitions;
      #pragma warning(pop)
   };
}