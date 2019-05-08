#include "pch.h"
#include "include/qgl_graphics_cmd_list.h"

qgl::graphics::low::graphics_command_list::graphics_command_list(const winrt::com_ptr<d3d_device>& dev_p,
                                                                 pipeline_state& pipelineState_p,
                                                                 UINT nodeMask) :
   m_pendingResourceTransitions(0),
   m_renderTargetTransitions(0),
   m_viewportsToSet(0),
   m_depthStencil_ps(0),
   m_scissorsToSet(0),
   m_rtvHandles(0),
   m_dsvHandles(0),
   icommand_list(dev_p, pipelineState_p, nodeMask)
{
   float rgba[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
   MemoryCopy<float>(m_clearColorRGBA, rgba, 4);
}


qgl::graphics::low::graphics_command_list::graphics_command_list(graphics_command_list&& r) :
   m_pendingResourceTransitions(std::move(r.m_pendingResourceTransitions)),
   m_renderTargetTransitions(std::move(r.m_renderTargetTransitions)),
   m_viewportsToSet(std::move(r.m_viewportsToSet)),
   m_depthStencil_ps(std::move(r.m_depthStencil_ps)),
   m_scissorsToSet(std::move(r.m_scissorsToSet)),
   m_rtvHandles(std::move(m_rtvHandles)),
   m_dsvHandles(std::move(r.m_dsvHandles)),
   icommand_list(std::move(r))
{
   MemoryCopy<float>(m_clearColorRGBA, r.m_clearColorRGBA, 4);
}

qgl::graphics::low::graphics_command_list::~graphics_command_list()
{

}

void qgl::graphics::low::graphics_command_list::frame_buffer(frame& f)
{
   //Get the number of frames.
   auto numTargets = 1;

   //Clear the array of RTV handles, render targets, and render target transitions.
   m_rtvHandles.resize(numTargets);
   m_renderTarget_ps.resize(numTargets);
   m_renderTargetTransitions.resize(numTargets);

   //Clear the array of DSV handles  and depth stencils
   m_dsvHandles.resize(numTargets);
   m_depthStencil_ps.resize(numTargets);
   m_viewportsToSet.resize(numTargets);
   m_scissorsToSet.resize(numTargets);

   //Add a transition for the render target.
   for (size_t i = 0; i < numTargets; i++)
   {
      auto& renderTarget = f.frame_buffer();
      auto& depthStencil = f.frame_stencil();
      auto& vp = f.frame_viewport();
      auto& scis = f.frame_scissor();

      m_renderTargetTransitions[i] = CD3DX12_RESOURCE_BARRIER::Transition(renderTarget.get(),
                                                                          //renderTarget.state(),
                                                                          D3D12_RESOURCE_STATE_PRESENT,
                                                                          D3D12_RESOURCE_STATE_RENDER_TARGET);
      //Update the state of the render target.
      renderTarget.state(D3D12_RESOURCE_STATE_RENDER_TARGET);

      //Make a copy of the render target view handle. This will be used in OMSetRenderTargets
      m_rtvHandles[i] = renderTarget.where();

      //Make a copy of the render target reference. 
      //This will be used later to convert the render targets to D3D12_RESOURCE_STATE_PRESENT
      m_renderTarget_ps[i] = &renderTarget;

      //Make a copy of the DSV and depth stencil so we can later clear the depth stencils using clear_depth()
      m_dsvHandles[i] = depthStencil.where();
      m_depthStencil_ps[i] = &depthStencil;

      m_viewportsToSet[i] = vp.get();
      m_scissorsToSet[i] = scis.get();
   }

   //Need to transition each render target from D3D12_RESOURCE_STATE_PRESENT to D3D12_RESOURCE_STATE_RENDER_TARGET
   m_cmdList_p->ResourceBarrier(static_cast<UINT>(m_renderTargetTransitions.size()),
                                m_renderTargetTransitions.data());


   m_cmdList_p->RSSetViewports(static_cast<UINT>(m_viewportsToSet.size()),
                               m_viewportsToSet.data());

   m_cmdList_p->RSSetScissorRects(static_cast<UINT>(m_scissorsToSet.size()),
                                  m_scissorsToSet.data());

   //Set the render targets.
   m_cmdList_p->OMSetRenderTargets(static_cast<UINT>(m_rtvHandles.size()),
                                   m_rtvHandles.data(),
                                   false,
                                   m_dsvHandles.data());
}

inline void qgl::graphics::low::graphics_command_list::clear_color(const float rgba[4])
{
   MemoryCopy<float>(m_clearColorRGBA, rgba, 4);
}

void qgl::graphics::low::graphics_command_list::begin()
{
   m_pendingResourceTransitions.resize(0);
   m_renderTargetTransitions.resize(0);
   reset();
}

void qgl::graphics::low::graphics_command_list::end()
{
   //When using d3d11on12, releasing the wrapped buffers takes care of transitioning the render 
   //targets to the correct state.

   ////Append PRESENT transitions to the list of pending transitions.
   //for (auto& rtv : m_renderTarget_ps)
   //{
   //   m_pendingResourceTransitions.push_back(CD3DX12_RESOURCE_BARRIER::Transition(
   //      rtv->get(),
   //      //rtv->state(),
   //      D3D12_RESOURCE_STATE_RENDER_TARGET,
   //      D3D12_RESOURCE_STATE_PRESENT));

   //   rtv->state(D3D12_RESOURCE_STATE_PRESENT);
   //}

   ////Flush
   //m_cmdList_p->ResourceBarrier(static_cast<UINT>(m_pendingResourceTransitions.size()),
   //                             m_pendingResourceTransitions.data());

   winrt::check_hresult(m_cmdList_p->Close());
}

void qgl::graphics::low::graphics_command_list::root_sig(const root_signature& sig)
{
   m_cmdList_p->SetGraphicsRootSignature(sig.com_get().get());
}

void qgl::graphics::low::graphics_command_list::table(const descriptor_table& tbl)
{
   m_cmdList_p->SetGraphicsRootDescriptorTable(tbl.root_index(), tbl.where());
}

void qgl::graphics::low::graphics_command_list::clear_frame()
{
   for (auto& target : m_renderTarget_ps)
   {
      auto rects = target->rectangles();
      if (rects)
      {
         m_cmdList_p->ClearRenderTargetView(target->where(),
                                            m_clearColorRGBA,
                                            static_cast<UINT>(rects->size()),
                                            rects->data());
      }
      else
      {
         m_cmdList_p->ClearRenderTargetView(target->where(),
                                            m_clearColorRGBA,
                                            0,
                                            nullptr);
      }
   }
}

void qgl::graphics::low::graphics_command_list::clear_depth()
{
   for (auto& stencil : m_depthStencil_ps)
   {
      auto rects = stencil->rectangles();
      m_cmdList_p->ClearDepthStencilView(stencil->where(), D3D12_CLEAR_FLAG_DEPTH,
                                         stencil->depth(), stencil->stencil(),
                                         static_cast<UINT>(rects->size()),
                                         rects->data());
   }
}

void qgl::graphics::low::graphics_command_list::blend_factor(const blender& blndr)
{
   throw E_NOTIMPL;
}

void qgl::graphics::low::graphics_command_list::stencil_ref(UINT ref)
{
   throw E_NOTIMPL;
}

