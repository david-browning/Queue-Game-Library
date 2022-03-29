#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/qgl_pso.h"
#include "include/GPU/Root-Signature/qgl_root_signature.h"
#include "include/GPU/Descriptors/qgl_descriptor_table.h"
#include "include/GPU/Descriptors/qgl_descriptor_heap.h"
#include "include/GPU/Buffers/qgl_index_buffer.h"
#include "include/GPU/Buffers/qgl_vertex_buffer.h"

namespace qgl::graphics::gpu
{
   /*
    Override begin(), root_sig(), and table().
    */
   class icommand_list
   {
      public:
      icommand_list(graphics_device_ptr& dev_p,
                    D3D12_COMMAND_LIST_TYPE listT,
                    const std::shared_ptr<gpu::pso>& pipelineState_p,
                    size_t nodeMask = 0) :
         m_pso_p(pipelineState_p)
      {
         make_allocator(dev_p->d3d12_device(), listT);
         make_cmd_list(dev_p->d3d12_device(), listT, nodeMask);
      }

      /*
       Copy constructor. This is delete to keep multiple threads from setting
       the same command list concurrently.
       */
      icommand_list(const icommand_list&) = delete;

      /*
       Move constructor.
       */
      icommand_list(icommand_list&&) = default;

      virtual ~icommand_list() noexcept = default;

      /*
       Puts the command list in the recording state.
       Classes that implement this function should start by calling "reset()".
       */
      virtual void begin() = 0;

      /*
       Closes the bundle so it can be executed.
       */
      void close()
      {
         m_cmdList->Close();
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
      void pso(const std::shared_ptr<pso>& pipeline_p)
      {
         m_pso_p = pipeline_p;
         m_cmdList->SetPipelineState(m_pso_p->get());
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
            std::initializer_list<descriptor_heap<DescriptorHeapT, Flag>> heaps)
      {
         m_heapsToSet.clear();
         m_heapsToSet.insert(m_heapsToSet.begin(), heaps.begin(), heaps.end());
         m_cmdList->SetDescriptorHeaps(static_cast<UINT>(m_heapsToSet.size()),
            m_heapsToSet.data());
      }

      template<class DescriptorIt>
      void descriptors(DescriptorIt first, DescriptorIt last)
      {
         m_heapsToSet.clear();
         m_heapsToSet.insert(m_heapsToSet.begin(), first, last);
         m_cmdList->SetDescriptorHeaps(static_cast<UINT>(m_heapsToSet.size()),
            m_heapsToSet.data());
      }

      void descriptors(const ID3D12DescriptorHeap** heaps, size_t numHeaps)
      {
         m_heapsToSet.resize(numHeaps);
         memcpy(m_heapsToSet.data(),
               heaps,
               sizeof(ID3D12DescriptorHeap*) * numHeaps);
         m_cmdList->SetDescriptorHeaps(static_cast<UINT>(m_heapsToSet.size()),
            m_heapsToSet.data());
      }

      /*
       Sets the vertex buffers. Do not allow the vertex buffers to go out of
       scope.
      */
      template<typename VertexT, D3D_PRIMITIVE_TOPOLOGY TopologyT>
      void vertex_buffers(
         std::initializer_list<vertex_buffer<VertexT, TopologyT>> buffers)
      {
         m_vertBuffView.clear();
         m_vertBuffView.insert(m_vertBuffView.begin(),
            buffers.begin(), buffers.end());
         m_cmdList->IASetVertexBuffers(
            0,
            static_cast<UINT>(m_vertBuffView.size()),
            m_vertBuffView.data());
      }

      template<class VertexIt>
      void vertex_buffers(VertexIt first, VertexIt last)
      {
         m_vertBuffView.clear();
         m_vertBuffView.insert(m_vertBuffView.begin(), first, last);
         m_cmdList->IASetVertexBuffers(
            0,
            static_cast<UINT>(m_vertBuffView.size()),
            m_vertBuffView.data());
      }

      void vertex_buffers(const D3D12_VERTEX_BUFFER_VIEW* views,
                          size_t numViews)
      {
         m_vertBuffView.resize(numViews);
         memcpy(m_vertBuffView.data(),
                views,
                sizeof(D3D12_VERTEX_BUFFER_VIEW) * numViews);

         m_cmdList->IASetVertexBuffers(
            0,
            static_cast<UINT>(m_vertBuffView.size()),
            m_vertBuffView.data());
      }

      /*
       Sets the index buffer.
       */
      template<typename IndexT>
      void index(const index_buffer<IndexT>& buff)
      {
         index(buff.view());
      }

      void index(const D3D12_INDEX_BUFFER_VIEW& buff_p)
      {
         m_idxBuffView = buff_p;
         m_cmdList->IASetIndexBuffer(&m_idxBuffView);
      }

      /*
       Sets the topology of the vertex buffers.
       */
      void topology(D3D_PRIMITIVE_TOPOLOGY topo) noexcept
      {
         m_cmdList->IASetPrimitiveTopology(topo);
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
            igpu_buffer<ResourceDescriptionT, ViewDescriptionT, ResourceT>* resource,
            D3D12_RESOURCE_STATES newState)
      {
         if (resource->state != newState)
         {
            transition(resource->get(), resource->state(), newState);
            resource->state(newState);
         }
      }

      void transition(gpu_resource* resource,
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

      void transition_queue(gpu_resource* resource,
                            D3D12_RESOURCE_STATES oldState,
                            D3D12_RESOURCE_STATES newState)
      {
         pendingResourceTransitions.push_back(
            CD3DX12_RESOURCE_BARRIER::Transition(
               resource,
               oldState,
               newState));
      }

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
         winrt::check_hresult(m_allocator->Reset());

         /*
          Before calling Reset(), the app must make sure that the GPU is no
          longer executing any command lists which are associated with the
          allocator; otherwise, the call will fail.
          Also, note that this API is not free-threaded and therefore can't be
          called on the same allocator at the same time from multiple threads.
          */
         winrt::check_hresult(m_cmdList->Reset(m_allocator.get(),
            m_pso_p->get()));
         m_vertBuffView.resize(0);
         m_heapsToSet.resize(0);
      }

      /*
       Returns a pointer to the D3D command list.
       */
      cmd_list* get() noexcept
      {
         return m_cmdList.get();
      }

      /*
       Returns a const pointer to the D3D command list.
       */
      const cmd_list* get() const noexcept
      {
         return m_cmdList.get();
      }

      private:
      /*
       When creating a command list, the command list type of the allocator,
       specified by D3D12_COMMAND_LIST_TYPE, must match the type of command
       list being created.
       */
      void make_allocator(device_3d* dev_p,
                          D3D12_COMMAND_LIST_TYPE listT)
      {
         winrt::check_hresult(dev_p->CreateCommandAllocator(
            listT,
            IID_PPV_ARGS(m_allocator.put())));
      }

      /*
       Immediately after being created, command lists are in the recording
       state. So, close it.
       */
      void make_cmd_list(device_3d* dev_p,
                         D3D12_COMMAND_LIST_TYPE listT,
                         size_t nodeMask)
      {
         winrt::check_hresult(dev_p->CreateCommandList(
            static_cast<UINT>(nodeMask),
            listT,
            m_allocator.get(),
            m_pso_p->get(),
            IID_PPV_ARGS(m_cmdList.put())));

         //Close command list.
         winrt::check_hresult(m_cmdList->Close());
      }

      std::shared_ptr<gpu::pso> m_pso_p;
      winrt::com_ptr<cmd_allocator> m_allocator;
      winrt::com_ptr<cmd_list> m_cmdList;

      /*
       Keep a list of the descriptor heaps to set. This is here so that the
       array of descriptor heap pointers does not go out of scope when
       descriptors() has finished.
       */
      std::vector<ID3D12DescriptorHeap*> m_heapsToSet;

      /*
       Keep a list of the vertex buffer views.
       */
      std::vector<D3D12_VERTEX_BUFFER_VIEW> m_vertBuffView;

      D3D12_INDEX_BUFFER_VIEW m_idxBuffView;

      protected:
      /*
       List of pending resource transitions.
       These are vectors instead of queues so that we can get the raw array
       pointer and guarantee that it is contiguous.
       */
      std::vector<D3D12_RESOURCE_BARRIER> pendingResourceTransitions;
   };
}