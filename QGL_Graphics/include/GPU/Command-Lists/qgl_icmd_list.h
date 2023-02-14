#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/qgl_ipso.h"
#include "include/GPU/Root-Signature/qgl_root_signature.h"
#include "include/GPU/Descriptors/qgl_descriptor_table.h"
#include "include/GPU/Descriptors/qgl_descriptor_heap.h"
#include "include/GPU/Buffers/qgl_index_buffer.h"
#include "include/GPU/Buffers/qgl_vertex_buffer.h"
#include "include/GPU/Buffers/qgl_upload_buffer.h"

namespace qgl::graphics::gpu
{
   /*
    Override begin(), end(), root_sig(), and table().
    */
   template<D3D12_COMMAND_LIST_TYPE ListT>
   class icommand_list
   {
      public:
      using descriptor_heap_list = typename std::vector<ID3D12DescriptorHeap*>;
      /*
       Constructs a command list with the initial pipeline state.
       The command list is independent of any other command list.
       */
      icommand_list(graphics_device& dev,
                    gpu::ipso& pso,
                    gpu_idx_t nodeMask = 0,
                    const sys_char* debugName = nullptr) :
         m_pso(pso),
         m_numIndcs(0),
         m_numVerts(0)
      {
         make_allocator(dev.dev_3d(), debugName);
         make_cmd_list(dev.dev_3d(), nodeMask, debugName);
      }

      /*
       Copy constructor. This is delete to keep multiple threads from setting
       the same command list concurrently.
       */
      icommand_list(const icommand_list&) = delete;

      /*
       Move constructor.
       */
      icommand_list(icommand_list&&) noexcept = default;

      virtual ~icommand_list() noexcept = default;

      /*
       Returns a pointer to the D3D command list.
       */
      icmd_list* get() noexcept
      {
         return m_cmdList.get();
      }

      /*
       Returns a const pointer to the D3D command list.
       */
      const icmd_list* get() const noexcept
      {
         return m_cmdList.get();
      }

      /*
       Puts the command list in the recording state.
       Classes that implement this function should start by calling "reset()".
       */
      virtual void begin() = 0;

      /*
       Ends the command list so that it is ready for execution.
       */
      virtual void end() = 0;

      /*
       Clears the command list. It must be rebuilt using this class's member
       functions. This leaves the command list in the recording state.
       A typical pattern is to submit a command list and then immediately reset
       it to reuse the allocated memory for another command list.
       Do not reset until the GPU is done using the command list and allocator.
       Use fences to wait until the GPU is done with this.
       */
      void reset()
      {
         // Command list allocators can only be reset when the associated
         // command lists have finished execution on the GPU; apps should use
         // fences to determine GPU execution progress.
         check_result(m_allocator->Reset());

         /*
          Before calling Reset(), the app must make sure that the GPU is no
          longer executing any command lists which are associated with the
          allocator; otherwise, the call will fail.
          Also, note that this API is not free-threaded and therefore can't be
          called on the same allocator at the same time from multiple threads.
          */
         check_result(m_cmdList->Reset(m_allocator.get(), m_pso.get().get()));
         m_heapsList.clear();
         pendingResourceTransitions.clear();
         m_numIndcs = 0;
         m_numVerts = 0;
      }

      /*
       Sets the root signature for the bundle or command list.
       If the root signature matches the root signature of the caller, then
       bindings are inherited, otherwise the bind space is reset.
       Bundles inherit the command list's root signature bindings (the
       bindings to the various slots in the Command List example above).
       If a bundle needs to change some of the inherited root signature
       bindings, it must first set the root signature to be the same as the
       calling command list (the inherited bindings do not become stale).
       If the bundle sets the root signature to be different than the calling
       command list, that has the same effect as changing the root signature
       on the command list described above: all previous root signature
       bindings are stale and newly expected bindings must be set before
       Draw/Dispatch; otherwise, the behavior is undefined.
       If a bundle does not need to change any root signature bindings,
       it does not need to set the root signature. Keep in mind that changing
       this during a command list is expensive.
       */
      virtual void root_sig(root_signature& sig) = 0;

      /*
       Sets a single descriptor table on the root signature.
       The root signature slot is defined by the table.
       */
      virtual void table(descriptor_table& tbl) = 0;

      /*
       Changes the pipeline state.
       Note that bundles don't inherit the pipeline state set by previous
       calls in direct command lists that are their parents.
       */
      void pso(ipso& pso)
      {
         m_pso = pso;
         m_cmdList->SetPipelineState(m_pso.get().get());
      }

      /*
       Sets the descriptor heaps. This should only be called once because the
       array of descriptor heaps is reset whenever this is called. Calling
       this method twice will cause the old array of descriptor heaps to become
       invalid.
       SetDescriptorHeaps can be called on a bundle, but the bundle descriptor
       heaps must match the calling command list descriptor heap.
       */
      template<
         D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapT,
         D3D12_DESCRIPTOR_HEAP_FLAGS Flag>
      void descriptors(
         std::initializer_list<descriptor_heap<DescriptorHeapT, Flag>*> heaps)
      {
         descriptor_heap_list toPushBack;
         for (auto& heap : heaps)
         {
            toPushBack.push_back(heap->get());
         }

         m_heapsList.emplace_back(std::move(toPushBack));
         get()->SetDescriptorHeaps(static_cast<UINT>(m_heapsList.back().size()),
                                   m_heapsList.back().data());
      }

      void descriptors(const descriptor_heap_list& heaps)
      {
         m_heapsList.push_back(heaps);
         get()->SetDescriptorHeaps(static_cast<UINT>(m_heapsList.back().size()),
                                   m_heapsList.back().data());
      }

      void descriptors(descriptor_heap_list&& heaps)
      {
         m_heapsList.emplace_back(std::forward<descriptor_heap_list&>(heaps));
         get()->SetDescriptorHeaps(static_cast<UINT>(m_heapsList.back().size()),
                                   m_heapsList.back().data());
      }

      /*
       Sets the vertex buffers. Do not allow the vertex buffers to go out of
       scope.
      */
      template<typename VertexT, D3D_PRIMITIVE_TOPOLOGY TopologyT>
      void vertices(vertex_buffer<VertexT, TopologyT>& buffer)
      {
         m_vertBuffView = *buffer.view();
         m_numVerts = buffer.count();
         m_cmdList->IASetVertexBuffers(0, 1, &m_vertBuffView);
         m_cmdList->IASetPrimitiveTopology(TopologyT);
      }

      /*
       Sets the index buffer.
       */
      template<typename IndexT>
      void indices(const index_buffer<IndexT>& buff)
      {
         m_idxBuffView = *buff.view();
         m_numIndcs = buff.count();
         m_cmdList->IASetIndexBuffer(&m_idxBuffView);
      }

      void draw(size_t instances)
      {
         get()->DrawInstanced(static_cast<UINT>(m_numVerts),
                              static_cast<UINT>(instances), 
                              0, 0);
      }

      /*
       Draws using the set index and vertex buffer.
       */
      void idraw(size_t instances)
      {
         get()->DrawIndexedInstanced(static_cast<UINT>(m_numIndcs),
                                     static_cast<UINT>(instances),
                                     0, 0, 0);
      }

      /*
       Performs an immediate resource transition from one state to another.
       Resource's current state is updated.
       Transitions can be expensive.
       */
      template<
         typename ResourceDescriptionT,
         typename ViewDescriptionT,
         typename ResourceT>
      void transition(
         igpu_buffer<ResourceDescriptionT, ViewDescriptionT, ResourceT>& resource,
         D3D12_RESOURCE_STATES newState)
      {
         if (resource.state() != newState)
         {
            transition(resource.get(), resource.state(), newState);
            resource.state(newState);
         }
      }

      void transition(igpu_resource* resource,
                      D3D12_RESOURCE_STATES oldState,
                      D3D12_RESOURCE_STATES newState)
      {
         auto transition = CD3DX12_RESOURCE_BARRIER::Transition(
            resource,
            oldState,
            newState);
         get()->ResourceBarrier(1, &transition);
      }

      /*
       Queues a transition to be flushed at the end of the command list.
       Resource's current state is updated.
       The transitions are not recorded until "end()" is called.
       It is best to batch transitions at the end of the command list because
       they may cause a pipeline stall.
       */
      template<
         typename ResourceDescriptionT,
         typename ViewDescriptionT,
         typename ResourceT>
      void transition_queue(
         igpu_buffer<ResourceDescriptionT, ViewDescriptionT, ResourceT>* resource,
         D3D12_RESOURCE_STATES newState)
      {
         if (resource->state != newState)
         {
            transition_queue(resource->get(), resource->state(), newState);
            resource->state(newState);
         }
      }

      void transition_queue(igpu_resource* resource,
                            D3D12_RESOURCE_STATES oldState,
                            D3D12_RESOURCE_STATES newState)
      {
         pendingResourceTransitions.push_back(
            CD3DX12_RESOURCE_BARRIER::Transition(
               resource,
               oldState,
               newState));
      }

      template<class ResDescT, class ViewDescT, class ResourceT>
      void upload(
         const gpu::igpu_buffer<ResDescT, ViewDescT, ResourceT>& buffer,
         const gpu::upload_buffer& uBuffer)
      {
         UpdateSubresources<1>(
            (ID3D12GraphicsCommandList*)get(),
            (ID3D12Resource*)buffer.get(),
            (ID3D12Resource*)uBuffer.get(),
            0, 0, 1,
            uBuffer.description());
      }

      private:
      /*
       When creating a command list, the command list type of the allocator,
       specified by D3D12_COMMAND_LIST_TYPE, must match the type of command
       list being created.
       */
      void make_allocator(i3d_device* dev_p,
                          const sys_char* debugName)
      {
         check_result(dev_p->CreateCommandAllocator(
            ListT,
            IID_PPV_ARGS(m_allocator.put())));

#ifdef DEBUG
         if (!debugName)
         {
            name_d3d(m_allocator.get(), L"Command Allocator");
         }
         else
         {
            std::wstringstream stream;
            stream << L"Command Allocator " << debugName;
            name_d3d(m_allocator.get(), stream.str().c_str());
         }
#endif
      }

      /*
       Immediately after being created, command lists are in the recording
       state. So, close it.
       */
      void make_cmd_list(i3d_device* dev_p,
                         gpu_idx_t nodeMask,
                         const sys_char* debugName)
      {
         check_result(dev_p->CreateCommandList(
            nodeMask,
            ListT,
            m_allocator.get(),
            m_pso.get().get(),
            IID_PPV_ARGS(m_cmdList.put())));

#ifdef DEBUG
         if (!debugName)
         {
            name_d3d(m_cmdList.get(), L"Command List");
         }
         else
         {
            std::wstringstream stream;
            stream << L"Command List " << debugName;
            name_d3d(m_cmdList.get(), stream.str().c_str());
         }
#endif

         //Close command list.
         check_result(m_cmdList->Close());
      }

      std::reference_wrapper<gpu::ipso> m_pso;
      pptr<icmd_allocator> m_allocator;
      pptr<icmd_list> m_cmdList;

      /*
       Keep a list of the descriptor heaps to set. This is here so that the
       array of descriptor heap pointers does not go out of scope when
       descriptors() has finished.
       */
      std::list<descriptor_heap_list> m_heapsList;

      D3D12_VERTEX_BUFFER_VIEW m_vertBuffView;
      D3D12_INDEX_BUFFER_VIEW m_idxBuffView;
      size_t m_numVerts;
      size_t m_numIndcs;

      protected:
      /*
       List of pending resource transitions.
       These are vectors instead of queues so that we can get the raw array
       pointer and guarantee that it is contiguous.
       */
      std::vector<D3D12_RESOURCE_BARRIER> pendingResourceTransitions;
   };
}