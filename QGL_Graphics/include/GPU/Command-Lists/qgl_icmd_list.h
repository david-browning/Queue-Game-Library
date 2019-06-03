#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Root-Signature/qgl_root_signature.h"
#include "include/GPU/Descriptors/qgl_descriptor_table.h"
#include "include/GPU/Descriptors/qgl_descriptor_heap.h"
#include "include/GPU/qgl_pso.h"
#include "include/GPU/Buffers/qgl_index_buffer.h"
#include "include/GPU/Buffers/qgl_vertex_buffer.h"

namespace qgl::graphics::gpu
{
   template<D3D12_COMMAND_LIST_TYPE ListT>
   class icommand_list
   {
      public:
      icommand_list(d3d_device* dev_p,
                    pipeline_state* pipelineState_p,
                    UINT nodeMask = 0) :
         m_allocator_p(nullptr),
         m_cmdList_p(nullptr),
         m_pipeline_p(pipelineState_p)
      {
         make_allocator(dev_p);
         make_cmd_list(dev_p, nodeMask);
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
         m_cmdList_p->Close();
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
      virtual void root_sig(const root_signature* sig) = 0;

      /*
       Sets a single descriptor table on the root signature.
       The root signature slot is defined by the table.
       */
      virtual void table(const descriptor_table& tbl) = 0;

      /*
       Changes the pipeline state.
       Note that bundles don't inherit the pipeline state set by previous
       calls in direct command lists that are their parents.
       */
      void pso(pipeline_state* pipeline_p)
      {
         m_pipeline_p = pipeline_p;
         m_cmdList_p->SetPipelineState(m_pipeline_p->get());
      }

      /*
       Sets the descriptor heaps. This should only be called once because the
       array of descriptor heaps is reset whenever this is called. Calling
       this method twice will cause the old array of descriptor heaps to become
       invalid.
       SetDescriptorHeaps can be called on a bundle, but the bundle descriptor
       heaps must match the calling command list descriptor heap.
       */
      template<D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapT,
         D3D12_DESCRIPTOR_HEAP_FLAGS Flag>
         void descriptors(descriptor_heap<DescriptorHeapT, Flag>* heaps,
                          size_t numHeaps)
      {
         //Resize the list of heaps and clear it out
         m_heapsToSet.resize(numHeaps);
         //m_heapsToSet.clear();

         for (size_t i = 0; i < numHeaps; i++)
         {
            m_heapsToSet[i] = heaps[i].get();
         }

         m_cmdList_p->SetDescriptorHeaps(static_cast<UINT>(m_heapsToSet.size()),
                                         m_heapsToSet.data());
      }

      /*
       Sets the vertex buffers. Do not allow the vertex buffers to go out of
       scope.
      */
      template<typename VertexT>
      void vertex_buffers(const buffers::vertex_buffer<VertexT>* buffers,
                          size_t numBuffers)
      {
         m_vertexBufferViews.resize(numBuffers);
         //m_vertexBufferViews.clear();

         for (size_t i = 0; i < numBuffers; i++)
         {
            m_vertexBufferViews[i] = buffers[i].view();
         }

         m_cmdList_p->IASetVertexBuffers(
            0,
            static_cast<UINT>(m_vertexBufferViews.size()),
            m_vertexBufferViews.data());
      }

      /*
       Sets the index buffer.
       */
      template<typename IndexT>
      void index(buffers::index_buffer<IndexT>* buff)
      {
         m_indexBufferViews = *buff->view();
         m_cmdList_p->IASetIndexBuffer(&m_indexBufferViews);
      }

      /*
       Sets the topology of the vertex buffers.
       */
      void topology(D3D_PRIMITIVE_TOPOLOGY topo) noexcept
      {
         m_cmdList_p->IASetPrimitiveTopology(topo);
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
         winrt::check_hresult(m_allocator_p->Reset());

         /*
          Before calling Reset(), the app must make sure that the GPU is no 
          longer executing any command lists which are associated with the 
          allocator; otherwise, the call will fail.
          Also, note that this API is not free-threaded and therefore can't be 
          called on the same allocator at the same time from multiple threads.
          */
         winrt::check_hresult(m_cmdList_p->Reset(m_allocator_p.get(), 
                                                 m_pipeline_p->get()));
         m_vertexBufferViews.resize(0);
         m_heapsToSet.resize(0);
      }

      /*
       Returns a pointer to the D3D command list.
       */
      d3d_command_list* get() noexcept
      {
         return m_cmdList_p.get();
      }

      /*
       Returns a const pointer to the D3D command list.
       */
      const d3d_command_list* get() const noexcept
      {
         return m_cmdList_p.get();
      }

      private:

      /*
       When creating a command list, the command list type of the allocator,
       specified by D3D12_COMMAND_LIST_TYPE, must match the type of command
       list being created.
       */
      void make_allocator(d3d_device* dev_p)
      {
         winrt::check_hresult(dev_p->CreateCommandAllocator(
            ListT,
            IID_PPV_ARGS(m_allocator_p.put())));
      }

      /*
       Immediately after being created, command lists are in the recording
       state. So, close it.
       */
      void make_cmd_list(d3d_device* dev_p, UINT nodeMask)
      {
         winrt::check_hresult(dev_p->CreateCommandList(
            nodeMask,
            ListT,
            m_allocator_p.get(),
            m_pipeline_p->get(),
            IID_PPV_ARGS(m_cmdList_p.put())));

         //Close command list.
         winrt::check_hresult(m_cmdList_p->Close());
      }

      pipeline_state* m_pipeline_p;

      winrt::com_ptr<d3d_cmd_allocator> m_allocator_p;
      winrt::com_ptr<d3d_command_list> m_cmdList_p;

      /*
       Keep a list of the descriptor heaps to set. This is here so that the
       array of descriptor heap pointers does not go out of scope when
       descriptors() has finished.
       */
      std::vector<ID3D12DescriptorHeap*> m_heapsToSet;

      /*
       Keep a list of the vertex buffer views.
       */
      std::vector<D3D12_VERTEX_BUFFER_VIEW> m_vertexBufferViews;

      /*
       Keep a list of the index buffer views.
       */
      D3D12_INDEX_BUFFER_VIEW m_indexBufferViews;
   };
}