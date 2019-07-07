#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/qgl_shader.h"
#include "include/Content/qgl_vert_description.h"
#include "include/Content/qgl_multisample_desc.h"
#include "include/GPU/Root-Signature/qgl_root_signature.h"
#include "include/GPU/Render/qgl_frame.h"

namespace qgl::graphics
{
   /*
    Represents a collection of shaders and other parameters describing how to
    draw geometry. This includes the rasterizer, blend, and depth stencil
    states.
    Creating the pipeline state object is deferred until calling get(). The
    pipeline state parameters must be set first.
    */
   class ipso : public iqgl
   {
      public:
      /*
       Returns a pointer to the D3D pipeline state.
       If the pipeline state has not been created, this creates it before
       returning the pointer. Returns nullptr if the pipeline cannot be
       created, possibly due to an incorrect parameter.
       */
      virtual const ID3D12PipelineState* const_get() const noexcept = 0;

      /*
       Returns a pointer to the D3D pipeline state.
       If the pipeline state has not been created, this creates it before
       returning the pointer. Returns nullptr if the pipeline cannot be
       created, possibly due to an incorrect parameter.
       */
      virtual ID3D12PipelineState* get() noexcept = 0;

      /*
       Sets the depth stencil state, format, and render target views. If the
       frames need to be recreated, the PSO does not have to be updated.
       This only needs to be called a second time if the render target format,
       depth stencil format, or depth stencil description change. This is
       unlikely after the PSO is built.
       Returns E_INVALIDARG if there are too many frames.
       */
      virtual HRESULT frames(gpu::render::iframe** frms,
                          size_t numFrames) noexcept = 0;
   };

   struct IPSO_CREATION_PARAMS
   {
      content::shader** Shaders;
      size_t NumShaders;
      gpu::render::iframe** Frames;
      size_t NumFrames;
      static_ptr_ref<content::multisample_desc> Sampler;
      static_ptr_ref<content::vertex_description> VertexDesc;
      static_ptr_ref<gpu::root_signature> RootSignature;
      UINT NodeMask;
   };

   /*
    Constructs an ipso.
    */
   extern "C"[[nodiscard]] QGL_GRAPHICS_API HRESULT QGL_CC
      qgl_make_pipeline(static_ptr_ref<graphics::d3d_device> dev_p,
                        const IPSO_CREATION_PARAMS* params_p,
                        qgl_version_t v,
                        ipso** out_p) noexcept;
}