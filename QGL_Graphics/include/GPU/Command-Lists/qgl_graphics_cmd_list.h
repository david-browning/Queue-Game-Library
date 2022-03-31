#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Command-Lists/qgl_icmd_list.h"
#include "include/GPU/Command-Lists/qgl_cmd_bundle.h"

namespace qgl::graphics::gpu
{
   class graphics_command_list : public icommand_list
   {
      public:
      graphics_command_list(graphics_device_ptr& dev_p,
                            const std::shared_ptr<gpu::pso>& pipelineState_p,
                            size_t nodeMask = 0) :
         icommand_list(dev_p,
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            pipelineState_p,
            nodeMask)
      {
      }

      graphics_command_list(const graphics_command_list&) = delete;

      graphics_command_list(graphics_command_list&&) = default;

      virtual ~graphics_command_list() noexcept = default;

      virtual void begin()
      {
         m_renderTargetTransitions.clear();
         pendingResourceTransitions.clear();
         reset();
      }

      /*
       Transitions the render target views to PRESENT, flushes the pending
       resource transitions, and closes the command list to that it can be
       executed.
       Call "get()" to get the list so it can be executed.
       */
      void end()
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
            static_cast<UINT>(pendingResourceTransitions.size()),
            pendingResourceTransitions.data());

         //This gets called by close().
         //winrt::check_hresult(get()->Close());
      }

      /*
       Sets the root signature for the command list.
       */
      virtual void root_sig(root_signature& sig)
      {
         get()->SetGraphicsRootSignature(sig.get());
      }

      /*
       Sets a descriptor table for the command list to use.
       */
      virtual void table(descriptor_table& tbl)
      {
         get()->SetGraphicsRootDescriptorTable(tbl.root_index(),
                                      tbl.where());
      }

      /*
       Sets the frames (including the render target, depth stencil, viewport,
       and scissor) for the graphics command list. Usually, this is the first
       command to queue before queuing other rendering commands.
       */
      template<class FrameIt>
      void frame_buffer(FrameIt first, FrameIt last)
      {
         m_renderTargetTransitions.clear();
         m_viewports.clear();
         m_scissors.clear();
         m_rtvHandles.clear();
         m_dsvHandles.clear();
         m_renderTargets.clear();
         m_depthStencils.clear();

         for (auto it = first; it != last; it++)
         {
            auto& renderTarget = it->frame_buffer();

            m_renderTargetTransitions.push_back(
               CD3DX12_RESOURCE_BARRIER::Transition(
                  renderTarget.get(),
                  D3D12_RESOURCE_STATE_PRESENT,
                  D3D12_RESOURCE_STATE_RENDER_TARGET));

            //Update the state of the render target.
            renderTarget.state(D3D12_RESOURCE_STATE_RENDER_TARGET);

            //Make a copy of the render target view handle. 
            //This will be used in OMSetRenderTargets
            m_rtvHandles.push_back(renderTarget.where());

            //Make a copy of the render target reference. 
            //This will be used later to convert the render targets to 
            //D3D12_RESOURCE_STATE_PRESENT
            m_renderTargets.push_back(renderTarget);

            //Make a copy of the DSV and depth stencil so we can later clear the
            //depth stencils using clear_depth()
            m_dsvHandles.push_back(it->frame_stencil().where());
            m_depthStencils.push_back(it->frame_stencil());

            m_viewports.push_back(it->frame_viewport().get());
            m_scissors.push_back(it->frame_scissor().get());
         }

         //Transition each render target from D3D12_RESOURCE_STATE_PRESENT to 
         //D3D12_RESOURCE_STATE_RENDER_TARGET
         get()->ResourceBarrier(
            static_cast<UINT>(m_renderTargetTransitions.size()),
            m_renderTargetTransitions.data());

         //Set the viewports
         get()->RSSetViewports(static_cast<UINT>(m_viewports.size()),
                               m_viewports.data());

         //Set the scissors
         get()->RSSetScissorRects(static_cast<UINT>(m_scissors.size()),
                                  m_scissors.data());

         //Set the render targets.
         get()->OMSetRenderTargets(static_cast<UINT>(m_rtvHandles.size()),
                                   m_rtvHandles.data(),
                                   false,
                                   m_dsvHandles.data());
      }

      /*
       Clears the render targets.
       Set the clear color using clear_color().
       Call frame_buffer() before calling this.
       */
      void clear_frame()
      {
         for (auto target : m_renderTargets)
         {
            get()->ClearRenderTargetView(
               target.get().where(),
               m_clearColor.data(),
               static_cast<UINT>(target.get().rectangle_count()),
               target.get().rectangles());
         }
      }

      /*
       Clears the depth stencils.
       Call frame_buffer() before calling this.
       */
      void clear_depth()
      {
         for (auto stencil : m_depthStencils)
         {
            get()->ClearDepthStencilView(
               stencil.get().where(),
               D3D12_CLEAR_FLAG_DEPTH,
               stencil.get().depth(),
               stencil.get().stencil(),
               static_cast<UINT>(stencil.get().rectangle_count()),
               stencil.get().rectangles());
         }
      }

      /*
       Sets the blend factor that modulate values for a pixel shader,
       render target, or both.
       */
      void blend_factor(const blender& blndr)
      {
         get()->OMSetBlendFactor(blndr.blend_factor());
      }

      /*
       Sets the reference value for depth stencil tests.
       */
      void stencil_ref(size_t ref)
      {
         get()->OMSetStencilRef(ref);
      }

      /*
       Sets the color to clear the render targets to.
       */
      void clear_color(const std::array<float, 4>& rgba) noexcept
      {
         m_clearColor = rgba;
      }

      /*
       Gets a const pointer to the color the render targets are cleared to.
       */
      const float* clear_color() const noexcept
      {
         return m_clearColor.data();
      }

      /*
       Queues a bundle to execute.
       */
      template<bool ComputeBundle = false>
      void bundle(cmd_bundle<ComputeBundle>& bndl)
      {
         get()->ExecuteBundle(bndl.get());
      }
            
      private:
      std::array<float, 4> m_clearColor;

      std::vector<std::reference_wrapper<depth_stencil>> m_depthStencils;
      std::vector<std::reference_wrapper<render_target>> m_renderTargets;

      std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_rtvHandles;
      std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_dsvHandles;
      std::vector<D3D12_RESOURCE_BARRIER> m_renderTargetTransitions;
      std::vector<D3D12_RECT> m_scissors;
      std::vector<D3D12_VIEWPORT> m_viewports;
   };
}