#include "pch.h"
#include "include/GPU/Command-Lists/qgl_icmd_list.h"

namespace qgl::graphics::gpu
{
   struct icommand_list::impl
   {
      impl(static_ptr_ref<igraphics_device> dev_p,
           D3D12_COMMAND_LIST_TYPE listT,
           static_ptr_ref<ipso> pipelineState_p,
           UINT nodeMask) :
         Allocator(nullptr),
         CmdList(nullptr),
         Pipeline_p(pipelineState_p)
      {
         make_allocator(dev_p->d3d12_device(), listT);
         make_cmd_list(dev_p->d3d12_device(), listT, nodeMask);
      }

      void pso(static_ptr_ref<ipso> pipeline_p)
      {
         Pipeline_p = pipeline_p;
         CmdList->SetPipelineState(Pipeline_p->get());
      }

      void topology(D3D_PRIMITIVE_TOPOLOGY topo) noexcept
      {
         CmdList->IASetPrimitiveTopology(topo);
      }

      void index(const D3D12_INDEX_BUFFER_VIEW* buff_p)
      {
         IndexBufferView = *buff_p;
         CmdList->IASetIndexBuffer(&IndexBufferView);
      }

      void descriptors(const ID3D12DescriptorHeap** heaps,
                       size_t numHeaps)
      {
         HeapsToSet.resize(numHeaps);
         memcpy(HeapsToSet.data(),
                heaps,
                sizeof(ID3D12DescriptorHeap*) * numHeaps);
         CmdList->SetDescriptorHeaps(static_cast<UINT>(HeapsToSet.size()),
                                     HeapsToSet.data());
      }

      void vertex_buffers(const D3D12_VERTEX_BUFFER_VIEW* views,
                          size_t numViews)
      {
         VertexBufferViews.resize(numViews);
         memcpy(VertexBufferViews.data(),
                views,
                sizeof(D3D12_VERTEX_BUFFER_VIEW) * numViews);

         CmdList->IASetVertexBuffers(
            0,
            static_cast<UINT>(VertexBufferViews.size()),
            VertexBufferViews.data());
      }


      void close()
      {
         CmdList->Close();
      }

      void reset()
      {
         // Command list allocators can only be reset when the associated
         // command lists have finished execution on the GPU; apps should use
         // fences to determine GPU execution progress.
         winrt::check_hresult(Allocator->Reset());

         /*
          Before calling Reset(), the app must make sure that the GPU is no
          longer executing any command lists which are associated with the
          allocator; otherwise, the call will fail.
          Also, note that this API is not free-threaded and therefore can't be
          called on the same allocator at the same time from multiple threads.
          */
         winrt::check_hresult(CmdList->Reset(Allocator.get(),
                                             Pipeline_p->get()));
         VertexBufferViews.resize(0);
         HeapsToSet.resize(0);
      }

      /*
       When creating a command list, the command list type of the allocator,
       specified by D3D12_COMMAND_LIST_TYPE, must match the type of command
       list being created.
       */
      void make_allocator(d3d_device* dev_p,
                          D3D12_COMMAND_LIST_TYPE listT)
      {
         winrt::check_hresult(dev_p->CreateCommandAllocator(
            listT,
            IID_PPV_ARGS(Allocator.put())));
      }

      /*
       Immediately after being created, command lists are in the recording
       state. So, close it.
       */
      void make_cmd_list(d3d_device* dev_p,
                         D3D12_COMMAND_LIST_TYPE listT,
                         UINT nodeMask)
      {
         winrt::check_hresult(dev_p->CreateCommandList(
            nodeMask,
            listT,
            Allocator.get(),
            Pipeline_p->get(),
            IID_PPV_ARGS(CmdList.put())));

         //Close command list.
         winrt::check_hresult(CmdList->Close());
      }


      static_ptr_ref<ipso> Pipeline_p;
      winrt::com_ptr<d3d_cmd_allocator> Allocator;
      winrt::com_ptr<d3d_command_list> CmdList;

      /*
       Keep a list of the descriptor heaps to set. This is here so that the
       array of descriptor heap pointers does not go out of scope when
       descriptors() has finished.
       */
      std::vector<ID3D12DescriptorHeap*> HeapsToSet;

      /*
       Keep a list of the vertex buffer views.
       */
      std::vector<D3D12_VERTEX_BUFFER_VIEW> VertexBufferViews;

      D3D12_INDEX_BUFFER_VIEW IndexBufferView;
   };

   icommand_list::icommand_list(static_ptr_ref<igraphics_device> dev_p,
                                D3D12_COMMAND_LIST_TYPE listT,
                                static_ptr_ref<ipso> pipelineState_p,
                                UINT nodeMask) :
      m_impl_p(new impl(dev_p, listT, pipelineState_p, nodeMask))
   {

   }

   icommand_list::icommand_list(icommand_list&& r)
   {
      delete m_impl_p;
      m_impl_p = r.m_impl_p;
      r.m_impl_p = nullptr;
   }

   icommand_list::~icommand_list() noexcept
   {
      delete m_impl_p;
      m_impl_p = nullptr;
   }

   void icommand_list::close()
   {
      m_impl_p->close();
   }

   void icommand_list::pso(static_ptr_ref<ipso> pipeline_p)
   {
      m_impl_p->pso(pipeline_p);
   }

   void icommand_list::topology(D3D_PRIMITIVE_TOPOLOGY topo) noexcept
   {
      m_impl_p->topology(topo);
   }

   void icommand_list::reset()
   {
      m_impl_p->reset();
   }

   d3d_command_list* icommand_list::get() noexcept
   {
      return m_impl_p->CmdList.get();
   }

   const d3d_command_list* icommand_list::get() const noexcept
   {
      return m_impl_p->CmdList.get();
   }

   void icommand_list::descriptors(const ID3D12DescriptorHeap** heaps,
                                   size_t numHeaps)
   {
      m_impl_p->descriptors(heaps, numHeaps);
   }

   void icommand_list::vertex_buffers(const D3D12_VERTEX_BUFFER_VIEW* views,
                                      size_t numViews)
   {
      m_impl_p->vertex_buffers(views, numViews);
   }

   void icommand_list::index(const D3D12_INDEX_BUFFER_VIEW* buff_p)
   {
      m_impl_p->index(buff_p);
   }
}