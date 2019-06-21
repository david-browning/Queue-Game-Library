#include "pch.h"
#include "include/GPU/Command-Lists/qgl_graphics_cmd_list.h"
#include "include/GPU/Render/qgl_frame.h"
#include "include/Content/qgl_blender.h"
#include "include/GPU/Render/qgl_viewport.h"
#include "include/GPU/Render/qgl_scissor.h"

namespace qgl::graphics::gpu
{
   struct graphics_command_list::impl
   {
      /*
       List of pending resource transitions.
       These are vectors instead of queues so that we can get the raw array
       pointer and guarantee that it is contiguous.
       */
      std::vector<D3D12_RESOURCE_BARRIER> PendingResourceTransitions;

      float ClearColorRGBA[4];


      std::vector<render::depth_stencil*> DepthStencil_ps;
      std::vector<render::render_target*> RenderTargets_ps;
      std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> RTVHandles;
      std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> DSVHandles;
      std::vector<D3D12_RESOURCE_BARRIER> RenderTargetTransitions;
      std::vector<D3D12_RECT> Scissors;
      std::vector<D3D12_VIEWPORT> Viewports;
   };

   graphics_command_list::graphics_command_list(
      d3d_device* dev_p,
      pipeline_state* pipelineState_p,
      UINT nodeMask) : icommand_list(dev_p,
                                     D3D12_COMMAND_LIST_TYPE_DIRECT,
                                     pipelineState_p,
                                     nodeMask)
   {

   }

   graphics_command_list::graphics_command_list(graphics_command_list&& r) :
      icommand_list(std::move(r))
   {
      delete m_impl_p;
      m_impl_p = r.m_impl_p;
      r.m_impl_p = nullptr;
   }

   graphics_command_list::~graphics_command_list()
   {
      delete m_impl_p;
   }

   void graphics_command_list::begin()
   {
      m_impl_p->RenderTargetTransitions.resize(0);
      m_impl_p->PendingResourceTransitions.resize(0);
      reset();
   }

   void graphics_command_list::end()
   {
      //When using d3d11on12, releasing the wrapped buffers takes care of 
      //transitioning the render targets to the correct state.

      ////Append PRESENT transitions to the list of pending transitions.
      //for (auto& rtv : m_renderTarget_ps)
      //{
      //   m_pendingResourceTransitions.push_back(
      //      CD3DX12_RESOURCE_BARRIER::Transition(
      //      rtv->get(),
      //      rtv->state(),
      //      D3D12_RESOURCE_STATE_RENDER_TARGET,
      //      D3D12_RESOURCE_STATE_PRESENT));

      //   rtv->state(D3D12_RESOURCE_STATE_PRESENT);
      //}

      //Flush
      get()->ResourceBarrier(
         static_cast<UINT>(m_impl_p->PendingResourceTransitions.size()),
         m_impl_p->PendingResourceTransitions.data());

      //This gets called by close().
      //winrt::check_hresult(get()->Close());
   }

   void graphics_command_list::root_sig(root_signature* sig)
   {
      get()->SetGraphicsRootSignature(sig->get());
   }

   void graphics_command_list::table(descriptor_table* tbl)
   {
      get()->SetGraphicsRootDescriptorTable(tbl->root_index(),
                                            tbl->where());
   }

   void graphics_command_list::frame_buffer(render::frame* f,
                                            size_t numFrames)
   {
      m_impl_p->RenderTargetTransitions.resize(numFrames);
      m_impl_p->Viewports.resize(numFrames);
      m_impl_p->Scissors.resize(numFrames);
      m_impl_p->RTVHandles.resize(numFrames);
      m_impl_p->DSVHandles.resize(numFrames);
      m_impl_p->RenderTargets_ps.resize(numFrames);
      m_impl_p->DepthStencil_ps.resize(numFrames);

      for (size_t i = 0; i < numFrames; i++)
      {
         auto renderTarget = f->frame_buffer();
         auto depthStencil = f->frame_stencil();
         auto vp = f->frame_viewport();
         auto scis = f->frame_scissor();

         m_impl_p->RenderTargetTransitions[i] =
            CD3DX12_RESOURCE_BARRIER::Transition(
               renderTarget->get(),
               D3D12_RESOURCE_STATE_PRESENT,
               D3D12_RESOURCE_STATE_RENDER_TARGET);

         //Update the state of the render target.
         renderTarget->state(D3D12_RESOURCE_STATE_RENDER_TARGET);

         //Make a copy of the render target view handle. 
         //This will be used in OMSetRenderTargets
         m_impl_p->RTVHandles[i] = renderTarget->where();

         //Make a copy of the render target reference. 
         //This will be used later to convert the render targets to 
         //D3D12_RESOURCE_STATE_PRESENT
         m_impl_p->RenderTargets_ps[i] = renderTarget;

         //Make a copy of the DSV and depth stencil so we can later clear the
         //depth stencils using clear_depth()
         m_impl_p->DSVHandles[i] = depthStencil->where();
         m_impl_p->DepthStencil_ps[i] = depthStencil;

         m_impl_p->Viewports[i] = *vp->get();
         m_impl_p->Scissors[i] = *scis->get();
      }

      //Transition each render target from D3D12_RESOURCE_STATE_PRESENT to 
      //D3D12_RESOURCE_STATE_RENDER_TARGET
      get()->ResourceBarrier(
         static_cast<UINT>(m_impl_p->RenderTargetTransitions.size()),
         m_impl_p->RenderTargetTransitions.data());

      //Set the viewports
      get()->RSSetViewports(static_cast<UINT>(m_impl_p->Viewports.size()),
                            m_impl_p->Viewports.data());

      //Set the scissors
      get()->RSSetScissorRects(static_cast<UINT>(m_impl_p->Scissors.size()),
                               m_impl_p->Scissors.data());

      //Set the render targets.
      get()->OMSetRenderTargets(static_cast<UINT>(m_impl_p->RTVHandles.size()),
                                m_impl_p->RTVHandles.data(),
                                false,
                                m_impl_p->DSVHandles.data());
   }

   void graphics_command_list::clear_frame()
   {
      for (auto target : m_impl_p->RenderTargets_ps)
      {
         get()->ClearRenderTargetView(
            target->where(),
            m_impl_p->ClearColorRGBA,
            static_cast<UINT>(target->rectangle_count()),
            target->rectangles());
      }
   }

   void graphics_command_list::clear_depth()
   {
      for (auto stencil : m_impl_p->DepthStencil_ps)
      {
         get()->ClearDepthStencilView(
            stencil->where(),
            D3D12_CLEAR_FLAG_DEPTH,
            stencil->depth(),
            stencil->stencil(),
            static_cast<UINT>(stencil->rectangle_count()),
            stencil->rectangles());
      }
   }

   void graphics_command_list::blend_factor(const content::blender* blndr)
   {
      get()->OMSetBlendFactor(blndr->blend_factor());
   }

   void graphics_command_list::stencil_ref(UINT ref)
   {
      get()->OMSetStencilRef(ref);
   }

   void graphics_command_list::clear_color(const float rgba[4]) noexcept
   {
      memcpy(m_impl_p->ClearColorRGBA, rgba, 4 * sizeof(float));
   }

   const float* graphics_command_list::clear_color() const noexcept
   {
      return m_impl_p->ClearColorRGBA;
   }

   void graphics_command_list::transition(d3d_resource* resource,
                                          D3D12_RESOURCE_STATES oldState,
                                          D3D12_RESOURCE_STATES newState)
   {
      auto transition = CD3DX12_RESOURCE_BARRIER::Transition(
         resource,
         oldState,
         newState);
      get()->ResourceBarrier(1, &transition);
   }

   void graphics_command_list::transition_queue(d3d_resource* resource,
                                                D3D12_RESOURCE_STATES oldState,
                                                D3D12_RESOURCE_STATES newState)
   {
      m_impl_p->PendingResourceTransitions.push_back(
         CD3DX12_RESOURCE_BARRIER::Transition(
            resource,
            oldState,
            newState));
   }

   void graphics_command_list::bundle(d3d_command_list* computeBndl)
   {
      get()->ExecuteBundle(computeBndl);
   }
}