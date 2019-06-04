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
   /*
    Override begin(), root_sig(), and table().
    */
   class QGL_GRAPHICS_API icommand_list
   {
      public:
      icommand_list(d3d_device* dev_p,
                    D3D12_COMMAND_LIST_TYPE listT,
                    pipeline_state* pipelineState_p,
                    UINT nodeMask = 0);

      /*
       Copy constructor. This is delete to keep multiple threads from setting
       the same command list concurrently.
       */
      icommand_list(const icommand_list&) = delete;

      /*
       Move constructor.
       */
      icommand_list(icommand_list&&);

      virtual ~icommand_list() noexcept;

      /*
       Puts the command list in the recording state.
       Classes that implement this function should start by calling "reset()".
       */
      virtual void begin() = 0;

      /*
       Closes the bundle so it can be executed.
       */
      void close();

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
      virtual void root_sig(root_signature* sig) = 0;

      /*
       Sets a single descriptor table on the root signature.
       The root signature slot is defined by the table.
       */
      virtual void table(descriptor_table* tbl) = 0;

      /*
       Changes the pipeline state.
       Note that bundles don't inherit the pipeline state set by previous
       calls in direct command lists that are their parents.
       */
      void pso(pipeline_state* pipeline_p);

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
      void descriptors(
         std::initializer_list<descriptor_heap<DescriptorHeapT, Flag>> heaps)
      {
         std::vector<const ID3D12DescriptorHeap*> ptrs;
         for (auto& h : heaps)
         {
            ptrs.push_back(h.get());
         }

         descriptors(ptrs.data(), ptrs.size());
      }

      /*
       Sets the vertex buffers. Do not allow the vertex buffers to go out of
       scope.
      */
      template<typename VertexT>
      void vertex_buffers(
         std::initializer_list<buffers::vertex_buffer<VertexT>> buffers)
      {
         std::vector<D3D12_VERTEX_BUFFER_VIEW> views;
         for (auto& b : buffers)
         {
            views.push_back(*b.view());
         }

         vertex_buffers(views.data(), views.size());
      }

      /*
       Sets the index buffer.
       */
      template<typename IndexT>
      void index(buffers::index_buffer<IndexT>* buff)
      {
         index(buff->view());
      }

      /*
       Sets the topology of the vertex buffers.
       */
      void topology(D3D_PRIMITIVE_TOPOLOGY topo) noexcept;

      /*
       Clears the command list. It must be rebuilt using this class's member
       functions. This leaves the command list in the recording state.
       A typical pattern is to submit a command list and then immediately reset
       it to reuse the allocated memory for another command list.
       Do not reset until the GPU is done using the command list and allocator.
       Use fences to wait until the GPU is done with this.
       */
      void reset();

      /*
       Returns a pointer to the D3D command list.
       */
      d3d_command_list* get() noexcept;

      /*
       Returns a const pointer to the D3D command list.
       */
      const d3d_command_list* get() const noexcept;

      private:

      void descriptors(const ID3D12DescriptorHeap** heaps,
                       size_t numHeaps);

      void vertex_buffers(const D3D12_VERTEX_BUFFER_VIEW* views,
                          size_t numViews);

      void index(const D3D12_INDEX_BUFFER_VIEW* buff_p);

      struct impl;
      impl* m_impl_p = nullptr;
   };
}