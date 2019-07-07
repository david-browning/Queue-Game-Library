#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Command-Lists/qgl_icmd_list.h"
#include "include/GPU/Command-Lists/qgl_cmd_bundle.h"
#include "include/GPU/Buffers/igpu_buffer.h"

namespace qgl::graphics::gpu::render
{
   class frame;
}

namespace qgl::graphics::gpu
{
   class QGL_GRAPHICS_API graphics_command_list : public icommand_list
   {
      public:
      graphics_command_list(static_ptr_ref<igraphics_device> dev_p,
                            static_ptr_ref<ipso> pipelineState_p,
                            UINT nodeMask = 0);

      graphics_command_list(const graphics_command_list&) = delete;

      graphics_command_list(graphics_command_list&&);

      virtual ~graphics_command_list() noexcept;

      virtual void begin();

      /*
       Transitions the render target views to PRESENT, flushes the pending
       resource transitions, and closes the command list to that it can be
       executed.
       Call "get()" to get the list so it can be executed.
       */
      void end();

      /*
       Sets the root signature for the command list.
       */
      virtual void root_sig(static_ptr_ref<root_signature> sig);

      /*
       Sets a descriptor table for the command list to use.
       */
      virtual void table(static_ptr_ref<descriptor_table> tbl);

      /*
       Sets the frames (including the render target, depth stencil, viewport,
       and scissor) for the graphics command list. Usually, this is the first
       command to queue before queuing other rendering commands.
       */
      void frame_buffer(render::frame* f,
                        size_t numFrames = 1);

      /*
       Clears the render targets.
       Set the clear color using clear_color().
       Call frame_buffer() before calling this.
       */
      void clear_frame();

      /*
       Clears the depth stencils.
       Call frame_buffer() before calling this.
       */
      void clear_depth();

      /*
       Sets the blend factor that modulate values for a pixel shader, 
       render target, or both.
       */
      void blend_factor(const static_ptr_ref<render::blender> blndr);

      /*
       Sets the reference value for depth stencil tests.
       */
      void stencil_ref(UINT ref);

      /*
       Sets the color to clear the render targets to.
       */
      void clear_color(const float rgba[4]) noexcept;

      /*
        Gets a const pointer to the color the render targets are cleared to.
       */
      const float* clear_color() const noexcept;

      /*
       Queues a bundle to execute.
       */
      template<bool ComputeBundle = false>
      void bundle(cmd_bundle<ComputeBundle>* bndl)
      {
         bundle(bndl->get());
      }

      /*
       Performs an immediate resource transition from one state to another. 
       Resource's current state is updated.
       Transitions can be expensive.
       */
      template<typename ResourceDescriptionT,
         typename ViewDescriptionT,
         typename ResourceT>
         void transition(buffers::igpu_buffer<
                         ResourceDescriptionT,
                         ViewDescriptionT,
                         ResourceT>* resource,
                         D3D12_RESOURCE_STATES newState)
      {
         if (resource->state != newState)
         {
            transition(resource->get(), resource->state(), newState);
            resource->state(newState);
         }
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
         void transition_queue(buffers::igpu_buffer<ResourceDescriptionT,
                               ViewDescriptionT,
                               ResourceT>* resource,
                               D3D12_RESOURCE_STATES newState)
      {
         if (resource->state != newState)
         {
            transition_queue(resource->get(), resource->state(), newState);
            resource->state(newState);
         }
      }

      private:
      struct impl;
      impl* m_impl_p;

      void transition(d3d_resource* resource,
                      D3D12_RESOURCE_STATES oldState,
                      D3D12_RESOURCE_STATES newState);

      void transition_queue(d3d_resource* resource,
                            D3D12_RESOURCE_STATES oldState,
                            D3D12_RESOURCE_STATES newState);

      void bundle(d3d_command_list* computeBndl);
   };
}