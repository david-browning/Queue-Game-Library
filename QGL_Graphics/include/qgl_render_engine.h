#pragma once
#include "qgl_graphics_include.h"
#include "qgl_d3d_device.h"
#include "qgl_iengine.h"
#include "std_ring.h"
#include "qgl_d3d_device.h"
#include "qgl_graphics_cmd_list.h"
#include "qgl_render_target.h"
#include "qgl_depth_stencil.h"
#include "qgl_pso.h"
#include "qgl_cmd_bundle.h"
#include "qgl_frame.h"
#include "qgl_text_render_component.h"

namespace qgl::graphics
{
   //Before rendering, call begin() and set the render targets.
   template<typename ValueT>
   class render_engine : public iengine<ValueT>
   {
      public:
      template<class InputIterator>
      render_engine(const std::shared_ptr<graphics_device>& dev_p,
                    InputIterator first,
                    InputIterator last,
                    const std::shared_ptr<fence<ValueT>>& engineFence_p,
                    pipeline_state& pipelineState_p,
                    UINT nodeMask = 0) :
         m_frames(first, last),
         m_frameIdx(dev_p->frame_index()),
         m_dev(dev_p),
         iengine(engineFence_p)
      {
         for (size_t i = 0; i < m_frames.size(); i++)
         {
            m_cmdList_ps.emplace_back(dev_p->com_d3d12_device(), pipelineState_p, nodeMask);
            
            #ifdef DEBUG
            std::wstringstream nameStream;
            nameStream << "Graphics Command List " << i;
            m_cmdList_ps[i].get()->SetName(nameStream.str().c_str());
            #endif
         }

         m_frames.advance_to(m_frameIdx);
      }

      render_engine(const render_engine& r) = delete;

      render_engine(render_engine&& r) :
         m_frames(std::move(r.m_frames)),
         m_frameIdx(r.m_frameIdx),
         m_dev(std::move(r.m_dev)),
         iengine(std::move(r))
      {
      }

      virtual ~render_engine()
      {
         m_frameIdx = static_cast<size_t>(-1);
      }

      /*
       Moves to the next frame and does the necessary setup to render to the render targets.
       */
      virtual void begin()
      {
         p_begin();
      }

      /////////////////////////////////////////////3D/////////////////////////////////////////////
      /*
       Renders something.
       */
      template<typename RenderObjectT, typename RenderContextT, bool UseSignal = false>
      sync_object<ValueT> render(icomponent<RenderContextT, RenderObjectT>& comp,
                                 RenderContextT& context,
                                 RenderObjectT& toRender)
      {
         auto ret = m_fence_p->next();
         comp.update(context, toRender);

         if (UseSignal)
         {
            m_fence_p->signal(ret);
         }

         return ret;
      }

      /*
       Sets the pipeline state for the current frame.
       */
      template<bool UseSignal = false>
      sync_object<ValueT> pso(const pipeline_state& pipeline)
      {
         auto ret = m_fence_p->next();
         m_frames.front().pso(pipeline);

         if (UseSignal)
         {
            m_fence_p->signal(ret);
         }

         return ret;
      }

      /*
       Executes a bundle for the current frame.
       */
      template<bool UseSignal = false, bool ComputeBundle = false>
      sync_object<ValueT> bundle(const low::cmd_bundle<ComputeBundle>& bndl)
      {
         auto ret = m_fence_p->next();
         std::get<CMD_LIST_IDX>(m_frames.front()).bundle(bndl);

         if (UseSignal)
         {
            m_fence_p->signal(ret);
         }

         return ret;
      }

      /*
       Sets the color to clear the render targets to.
       This sets the same clear color for all render targets.
       */
      inline void clear_color(const float rgba[4])
      {
         for (auto& cmdList : m_cmdList_ps)
         {
            cmdList.clear_color(rgba);
         }
      }

      /*
       Clears the render target for the current frame.
       The clear color can be set using clear_color(). The color only needs to be set once.
       The clear color is copied to internal storage so you do not need to call this every time
       you render a frame.
       */
      template<bool UseSignal = false>
      sync_object<ValueT> clear_frames()
      {
         auto ret = m_fence_p->next();
         m_cmdList_ps[m_frameIdx].clear_frame();

         if (UseSignal)
         {
            m_fence_p->signal(ret);
         }

         return ret;
      }

      /////////////////////////////////////////////2D/////////////////////////////////////////////
      /*
       Sets the transform for the 2D surface. For normal operation,
       use the identity matrix.
       */
      void transform_2d(const D2D1::Matrix3x2F& t)
      {
         m_dev->d2d1_context()->SetTransform(t);
      }

      ////////////////////////////////////////House keeping////////////////////////////////////////
      /*
       Releases the frames. This must be done before resizing the render target.
       */
      void release()
      {
         auto n = m_fence_p->next();
         m_fence_p->signal(n);
         m_fence_p->wait(n);

         for (auto it = m_frames.begin(); it != m_frames.end(); ++it)
         {
            (*it)->release(m_dev->com_d3d11on12_device());
         }
      }

      virtual sync_object<ValueT> flush()
      {
         auto& frontCMDList = m_cmdList_ps[m_frameIdx];

         //Append a sync point to the end of the command list so we can wait on it.
         auto lastSync = m_fence_p->next();
         m_fence_p->signal(lastSync);

         //Execute command list.
         frontCMDList.end();
         ID3D12CommandList* cmdLists[] = { frontCMDList.get() };
         m_fence_p->queue()->ExecuteCommandLists(1, cmdLists);

         
         auto& rTarget = m_frames.front()->frame_buffer();
         //End the d2d draw.
         winrt::check_hresult(m_dev->d2d1_context()->EndDraw());

         //Release the wrapped buffers.
         rTarget.release_resources(m_dev->com_d3d11on12_device());

         //Flush the d3d11 context.
         m_dev->d3d11on12_context()->Flush();

         return lastSync;
      }

      /*
       Presents the rendered frame.
       Recommend waiting on for the sync_object returned by flush() before presenting.
       */
      inline void present()
      {
         winrt::check_hresult(m_dev->swap_chain()->Present(m_dev->sync_interval(),
                                                           m_dev->swap_flags()));
      }

      private:
      std::ring<std::shared_ptr<frame>> m_frames;
      std::vector<low::graphics_command_list> m_cmdList_ps;
      std::shared_ptr<graphics_device> m_dev;
      size_t m_frameIdx;

      void p_begin()
      {
         //Advance which frame we are rendering.
         m_frameIdx = m_dev->frame_index();
         m_frames.advance_to(m_frameIdx);

         auto& frontData = m_frames.front();

         //Begin recording the command list.
         auto& frontCMDList = m_cmdList_ps[m_frameIdx];
         frontCMDList.begin();

         //Set the d3d12 render targets. This takes care of transitioning them to
         //the correct state.
         frontCMDList.frame_buffer(*frontData);

         //Acquire the wrapped resource for d2d rendering
         auto& rTarget = m_frames.front()->frame_buffer();
         rTarget.acquire_resources(m_dev->com_d3d11on12_device());

         //Set the d2d render targets.
         m_dev->d2d1_context()->SetTarget(rTarget.d2d_target());

         //Begin d2d drawing
         m_dev->d2d1_context()->BeginDraw();

         //Ready to record commands.
      }
   };
}