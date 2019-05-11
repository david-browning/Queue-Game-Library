#pragma once
#include "qgl_graphics_include.h"
#include "qgl_cmd_bundle.h"
#include "qgl_icmd_list.h"
#include "qgl_render_target.h"
#include "qgl_depth_stencil.h"
#include "qgl_igpu_buffer.h"
#include "qgl_descriptor_heap.h"
#include "qgl_constant_buffer.h"
#include "qgl_sampler.h"
#include "qgl_frame.h"
#include "qgl_blender.h"

namespace qgl::graphics::low
{
   class LIB_EXPORT graphics_command_list : public icommand_list<D3D12_COMMAND_LIST_TYPE_DIRECT>
   {
      public:
      //Direct command lists must specify an initial descriptor heap, but may
      //change the currently - bound descriptor heap inside the command list by 
      //calling ID3D12GraphicsCommandList::SetDescriptorHeap.

      graphics_command_list(const winrt::com_ptr<d3d_device>& dev_p,
                            pipeline_state& pipelineState_p,
                            UINT nodeMask = 0);

      graphics_command_list(const graphics_command_list& r) = delete;

      graphics_command_list(graphics_command_list&& r);

      virtual ~graphics_command_list();

      virtual void begin();

      /*
       Transitions the render target views to PRESENT, flushes the pending resource
       transitions, and closes the command list to that it can be executed.
       Call "get()" or "com_get()" to get the list so it can be executed.
       */
      void end();

      /*
       Sets the root signature for the command list.
       */
      virtual void root_sig(const root_signature& sig);

      /*
       Sets a descriptor table for the command list to use.
       */
      virtual void table(const descriptor_table& tbl);

      /*
       Clears the render targets.
       Set the clear color using clear_color().
       Set the render targets using frame_buffer().
       DBC: Call frame_buffer() before calling this.
       */
      _DBC_ void clear_frame();

      /*
       Clears the depth stencils.
       DBC: Call frame_buffer() before calling this.
       */
      _DBC_ void clear_depth();

      //m_renderCmdList_p->OMSetBlendFactor
      void blend_factor(const blender& blndr);

      //m_renderCmdList_p->OMSetStencilRef
      void stencil_ref(UINT ref);

      /*
       Sets the frame (including the render target, depth stencil, viewport, and scissor) for
       the graphics command list. Usually, this is the first command to queue before queuing
       other rendering commands.
       */
      void frame_buffer(frame& f);

      /*
       Sets the color to clear the render targets to.
       */
      inline void clear_color(const float rgba[4]);

      /*
        Gets a const pointer to the color the render targets are cleared to.
       */
      inline const float* clear_color() const
      {
         return m_clearColorRGBA;
      }

      /*
       Queues a bundle to execute.
       */
      template<bool ComputeBundle = false>
      void bundle(cmd_bundle<ComputeBundle>& bndl)
      {
         m_cmdList_p->ExecuteBundle(bndl.get());
      }

      /*
       Performs an immediate resource transition from one state to another. Resource's current
       state is updated.
       Transitions can be expensive.
       */
      template<typename ResourceDescriptionT, typename ViewDescriptionT, typename resourceT>
      void transition(igpu_buffer<ResourceDescriptionT, ViewDescriptionT, resourceT>& resource, D3D12_RESOURCE_STATES newState)
      {
         if (resource.state != newState)
         {
            m_cmdList_p->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
               resource.get(),
               resource.state(),
               newState));
            resource.state(newState);
         }
      }

      /*
       Queues a transition to be flushed at the end of the command list. Resource's current
       state is updated.
       The transitions are not recorded until "end()" is called.
       It is best to batch transitions at the end of the command list because they may
       cause a pipeline stall.
       */
      template<typename ResourceDescriptionT, typename ViewDescriptionT, typename resourceT>
      void transition_queue(igpu_buffer<ResourceDescriptionT, ViewDescriptionT, resourceT>& resource, D3D12_RESOURCE_STATES newState)
      {
         if (resource.state != newState)
         {
            m_pendingResourceTransitions.push_back(CD3DX12_RESOURCE_BARRIER::Transition(
               resource.get(),
               resource.state(),
               newState));
            resource.state(newState);
         }
      }

      protected:

      private:
      #pragma warning(push)
      #pragma warning(disable: 4251)

      /*
       List of pending resource transitions.
       These are vectors instead of queues so that we can get the raw array pointer and
       guarantee that it is contiguous.
       */
      std::vector<D3D12_RESOURCE_BARRIER> m_pendingResourceTransitions;


      /*
       Keep a list of the viewports to set. This is here so that the initializer list of
       viewports does not get lost when viewports() has finished.
       */
      std::vector<D3D12_VIEWPORT> m_viewportsToSet;

      /*
       Keep a list of the scissors rectangles to set. This is here so that the initializer list of
       rectangles does not get lost when scissors() has finished.
       */
      std::vector<D3D12_RECT> m_scissorsToSet;

      //Needed to set render targets.
      std::vector<render_target*> m_renderTarget_ps;
      std::vector<D3D12_RESOURCE_BARRIER> m_renderTargetTransitions;
      std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_rtvHandles;

      std::vector<depth_stencil*> m_depthStencil_ps;
      std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_dsvHandles;
      #pragma warning(pop)

      float m_clearColorRGBA[4];
   };
}