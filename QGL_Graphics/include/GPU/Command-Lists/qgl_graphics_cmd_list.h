#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Command-Lists/qgl_icmd_list.h"
#include "include/GPU/Command-Lists/qgl_cmd_bundle.h"
#include "include/GPU/Render/qgl_blender.h"
#include "include/GPU/Render/qgl_depth_stencil.h"
#include "include/GPU/Render/qgl_render_target.h"
#include "include/Descriptors/qgl_vector_descriptor.h"

namespace qgl::graphics::gpu
{
   class graphics_command_list : public icommand_list
   {
      public:
      graphics_command_list(graphics_device& dev,
                            gpu::ipso& pso,
                            gpu_idx_t nodeMask = 0,
                            const sys_char* debugName = nullptr) :
         icommand_list(dev,
                       pso,
                       D3D12_COMMAND_LIST_TYPE_DIRECT,
                       nodeMask,
                       debugName)
      {
      }

      graphics_command_list(const graphics_command_list&) = delete;

      graphics_command_list(graphics_command_list&&) = default;

      virtual ~graphics_command_list() noexcept = default;

      virtual void begin() override
      {
         m_rtTargetTransitions.clear();
         m_rtPresentTransitions.clear();
         pendingResourceTransitions.clear();
         m_frames.clear();
         m_viewports.clear();
         m_scissors.clear();
         m_rtvHandles.clear();
         m_dsvHandles.clear();
         reset();
      }

      virtual void end() override
      {
         // Transition the frames to present mode.
         for (auto& f : m_frames)
         {
            m_rtPresentTransitions.push_back(
               CD3DX12_RESOURCE_BARRIER::Transition(
                  f.get().frame_buffer().get(),
                  D3D12_RESOURCE_STATE_RENDER_TARGET,
                  D3D12_RESOURCE_STATE_PRESENT));
         }

         get()->ResourceBarrier(
            static_cast<UINT>(m_frames.size()), 
            m_rtPresentTransitions.data());
         check_result(get()->Close());
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
         get()->SetGraphicsRootDescriptorTable(tbl.root_index(), tbl.where());
      }

      /*
       Sets the frames (including the render target, depth stencil, viewport,
       and scissor) for the graphics command list. Usually, this is the first
       command to queue before queuing other rendering commands.
       */
      void frame_buffer(frame& f)
      {
         m_frames.push_back(f);
         m_rtvHandles.push_back(f.frame_buffer().where());
         m_dsvHandles.push_back(f.frame_stencil().where());
         m_viewports.push_back(f.frame_viewport().get());
         m_scissors.push_back(f.frame_scissor().get());

         finalize_frame_buffers();
      }

      /*
       Clears the render targets.
       Set the clear color using clear_color().
       Call frame_buffer() before calling this.
       */
      void clear_frame()
      {
         for (auto f : m_frames)
         {
            auto& target = f.get().frame_buffer();
            get()->ClearRenderTargetView(
               target.where(),
               m_clearColor.data(),
               static_cast<UINT>(target.rectangle_count()),
               target.rectangles());
         }
      }

      /*
       Clears the depth stencils.
       Call frame_buffer() before calling this.
       */
      void clear_depth()
      {
         for (auto f : m_frames)
         {
            auto& stencil = f.get().frame_stencil();
            get()->ClearDepthStencilView(
               stencil.where(),
               D3D12_CLEAR_FLAG_DEPTH,
               stencil.depth(),
               stencil.stencil(),
               static_cast<UINT>(stencil.rectangle_count()),
               stencil.rectangles());
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
         get()->OMSetStencilRef(static_cast<UINT>(ref));
      }

      /*
       Sets the color to clear the render targets to.
       */
      void clear_color(const std::array<float, 4>& rgba) noexcept
      {
         memcpy(m_clearColor.data(), rgba.data(), sizeof(float) * rgba.size());
      }

      /*
       Sets the color to clear the render targets to.
       */
      void clear_color(const descriptors::vector_descriptor& rgba) noexcept
      {
         m_clearColor[0] = rgba[0].operator float();
         m_clearColor[1] = rgba[1].operator float();
         m_clearColor[2] = rgba[2].operator float();
         m_clearColor[3] = rgba[3].operator float();
      }

      /*
       Sets the color to clear the render targets to.
       */
      void clear_color(const fixed_buffer<float, 4>& rgba) noexcept
      {
         memcpy(m_clearColor.data(), rgba.data(), sizeof(float) * rgba.size());
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
      void finalize_frame_buffers()
      {
         // Transition the render targets from Present to Render Target
         for (auto& f : m_frames)
         {
            m_rtTargetTransitions.push_back(
               CD3DX12_RESOURCE_BARRIER::Transition(
                  f.get().frame_buffer().get(),
                  D3D12_RESOURCE_STATE_PRESENT,
                  D3D12_RESOURCE_STATE_RENDER_TARGET));
         }

         //Set the viewports
         get()->RSSetViewports(static_cast<UINT>(m_viewports.size()),
                               m_viewports.data());

         //Set the scissors
         get()->RSSetScissorRects(static_cast<UINT>(m_scissors.size()),
                                  m_scissors.data());

         //Transition each render target from D3D12_RESOURCE_STATE_PRESENT to 
         //D3D12_RESOURCE_STATE_RENDER_TARGET
         get()->ResourceBarrier(
            static_cast<UINT>(m_rtTargetTransitions.size()),
            m_rtTargetTransitions.data());

         //Set the render targets.
         get()->OMSetRenderTargets(static_cast<UINT>(m_rtvHandles.size()),
                                   m_rtvHandles.size() > 0 ?
                                      m_rtvHandles.data() : nullptr,
                                   false,
                                   m_dsvHandles.size() > 0 ?
                                      m_dsvHandles.data() : nullptr);
      }

      fixed_buffer<float, 4> m_clearColor;
      std::vector<std::reference_wrapper<frame>> m_frames;
      std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_rtvHandles;
      std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_dsvHandles;
      std::vector<D3D12_RESOURCE_BARRIER> m_rtTargetTransitions;
      std::vector<D3D12_RESOURCE_BARRIER> m_rtPresentTransitions;
      std::vector<D3D12_RECT> m_scissors;
      std::vector<D3D12_VIEWPORT> m_viewports;
   };
}