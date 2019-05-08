#pragma once
#include "qgl_graphics_include.h"
#include "qgl_iresident.h"
#include "qgl_iengine.h"
#include "qgl_cpy_cmd_list.h"

#include "qgl_vertex_buffer.h"
#include "qgl_index_buffer.h"
#include "qgl_texture.h"
#include "qgl_constant_buffer.h"

namespace qgl::graphics
{
   /*
    Supports queuing:
      vertex_buffer
      index_buffer,
      const_buffer,
      light,
      camera,
      texture,
    */
   template<typename ValueT>
   class copy_engine : public iengine<ValueT>
   {
      public:
      copy_engine(winrt::com_ptr<d3d_device>& dev_p,
                  const std::shared_ptr<fence<ValueT>>& engineFence_p,
                  std::shared_ptr<pipeline_state> pipelineState_p = nullptr,
                  UINT nodeMask = 0) :
         m_cpyList(dev_p, pipelineState_p, nodeMask),
         iengine(engineFence_p)
      {

      }

      copy_engine(const copy_engine& r) = delete;

      copy_engine(copy_engine&& r) :
         m_cpyList(std::move(r.m_cpyList)),
         iengine(std::move(r))
      {

      }

      virtual ~copy_engine()
      {

      }

      virtual void begin()
      {
         m_cpyList.begin();
      }

      virtual sync_object<ValueT> flush()
      {
         //Append a sync point to the end of the command list so we can wait on it.
         auto lastSync = m_fence_p->next();
         m_fence_p->signal(lastSync);

         //Execute command list.
         m_cpyList.close();
         ID3D12CommandList* cmdLists[] = { m_cpyList.get() };
         m_fence_p->queue()->ExecuteCommandLists(1, cmdLists);

         return lastSync;
      }

      template<typename VertexT, bool UseSignal = false>
      sync_object<ValueT> upload(vertex_buffer<VertexT>& q)
      {
         //Get the next sync object from the fence.
         auto ret = m_fence_p->next();

         //Tell the copy command list to upload this.
         m_cpyList.copy(q);

         if (UseSignal)
         {
            m_fence_p->signal(ret);
         }

         return ret;
      }

      template<typename IndexT, bool UseSignal = false>
      sync_object<ValueT> upload(index_buffer<IndexT>& q)
      {
         //Get the next sync object from the fence.
         auto ret = m_fence_p->next();

         //Tell the copy command list to upload this.
         m_cpyList.copy(q);

         if (UseSignal)
         {
            m_fence_p->signal(ret);
         }

         return ret;
      }

      template<bool UseSignal = false>
      sync_object<ValueT> upload(const texture& q)
      {
         throw E_NOTIMPL;
      }

      template<typename T, bool UseSignal = false>
      sync_object<ValueT> map(const const_buffer<T>& q)
      {
         throw E_NOTIMPL;
      }

      template<typename T, bool UseSignal = false>
      sync_object<ValueT> unmap(const const_buffer<T>& q)
      {
         throw E_NOTIMPL;
      }

      //Camera

      //Light

      template<bool UseSignal = false>
      sync_object<ValueT> download(const texture& q)
      {
         throw E_NOTIMPL;
      }

      private:
      low::copy_command_list m_cpyList;
   };
}