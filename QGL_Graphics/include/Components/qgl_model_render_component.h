#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Command-Lists/qgl_cmd_bundle.h"
#include "include/Content/qgl_model.h"

namespace qgl::graphics::components
{
   ///*
   // A model render component controls how to display geometry. It sets the
   // pipeline state for a given command bundle before setting the necessary
   // buffers to render the geometry.
   // */
   //template<typename VertexT, D3D_PRIMITIVE_TOPOLOGY TopologyT, typename IndexT>
   //class model_render_component : 
   //   public icomponent<gpu::cmd_bundle<false>, content::model<VertexT, 
   //                                                            TopologyT, 
   //                                                            IndexT>>
   //{
   //   public:
   //   using uc_t = gpu::cmd_bundle<false>;
   //   using u_t = content::model<VertexT, TopologyT, IndexT>;

   //   /*
   //    The render component takes ownership of the pipeline state. After this
   //    is constructed, the pso can go out of scope.
   //    */
   //   model_render_component(const gpu::pipeline_state* pso,
   //                          const GUID* g) :
   //      m_pso(*pso),
   //      icomponent(g)
   //   {

   //   }

   //   model_render_component(const model_render_component&) = default;

   //   model_render_component(model_render_component&&) = default;

   //   virtual ~model_render_component() noexcept = default;

   //   /*
   //    Populates the command bundle with the necessary commands to render
   //    obj. This does not close the bundle in case the caller wants to add more
   //    commands. This does not reset the bundle, set the root signature, heaps,
   //    or root descriptor tables. The caller must do these things.
   //    */
   //   virtual void update(uc_t* context, u_t* obj)
   //   {
   //      //Set topology
   //      context->topology(obj->vert_buffer()->topology());

   //      //Set index buffer
   //      context->index<IndexT>(obj->idx_buffer());

   //      //Set vertex buffer
   //      context->vertex_buffers({ obj->vert_buffer() });

   //      //Set PSO
   //      context->pso(&m_pso);
   //   }

   //   private:
   //   /*
   //    Pipeline state this component uses.
   //    */
   //   gpu::pipeline_state m_pso;
   //};
}