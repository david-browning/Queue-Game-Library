#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Root-Signature/qgl_root_signature.h"
#include "include/Content/qgl_blender.h"
#include "include/Content/qgl_rasterizer.h"
#include "include/Content/qgl_shader.h"

namespace qgl::graphics::gpu::buffers
{
   class frame;
}

namespace qgl::graphics::gpu
{
   /*
    Represents a collection of shaders and other parameters describing how to
    draw geometry. This includes the rasterizer, blend, and depth stencil 
    states. 
    Creating the pipeline state object is deferred until calling get(). The 
    pipeline state parameters must be set first.
    */
   class QGL_GRAPHICS_API pipeline_state
   {
      public:
      #ifdef DEBUG
      static constexpr D3D12_PIPELINE_STATE_FLAGS PSO_DEFAULT_FLAG =
         D3D12_PIPELINE_STATE_FLAG_NONE;
      #else
      static constexpr D3D12_PIPELINE_STATE_FLAGS PSO_DEFAULT_FLAG =
         D3D12_PIPELINE_STATE_FLAG_NONE;
      #endif

      pipeline_state(d3d_device* dev_p);

      pipeline_state(const pipeline_state&);

      pipeline_state(pipeline_state&&);

      virtual ~pipeline_state() noexcept;
            
      /*
       Sets the depth stencil state, format, and render target views.
       */
      template<class InputIterator>
      void frames(InputIterator first, InputIterator last)
      {
         static constexpr auto maxRTVs = 
            sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC::RTVFormats) /
            sizeof(DXGI_FORMAT);

         const auto numFrames = last - first;
         if (numFrames > maxRTVs)
         {
            throw std::invalid_argument("Maximum of 8 RTVs allowed.");
         }

         m_psoDesc.NumRenderTargets = static_cast<UINT>(numFrames);
         size_t i = 0;
         for (auto it = first; it != last; ++it)
         {
            auto& frame = *it;
            auto& stncl = frame->frame_stencil();
            auto& renderTarget = frame->frame_buffer();
            m_psoDesc.RTVFormats[i] = renderTarget.format();
            m_psoDesc.DSVFormat = stncl.format();
            m_psoDesc.DepthStencilState = stncl.depth_state();
            i++;
         }
      }

      /*
       Returns a pointer to the D3D pipeline state. Throws an exception is the
       pipeline state cannot be created, possibly due to an incorrect 
       parameter.
       If the pipeline state has not been created, this creates it before 
       returning the pointer. 
       */
      const ID3D12PipelineState* get() const;

      /*
       Sets the sample description.
       */
      void sample_description(DXGI_SAMPLE_DESC desc);

      /*
       Sets the sample description. Default values are provided.
       */
      void sample_description(UINT msaaCount = 1, UINT msaaQuality = 0);

      /*
       Sets the vertex layout and topology for the input assembler.
       Most likely, it will contain a POSITION, NORMAL, TEXCOORD, TANGENT
       The description array is copied so descs can go out of scope after 
       calling this.
       */
      void layout(const D3D12_INPUT_ELEMENT_DESC* descs,
                  size_t numDescs,
                  D3D12_PRIMITIVE_TOPOLOGY_TYPE topo,
                  D3D12_INDEX_BUFFER_STRIP_CUT_VALUE = 
                  D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED);

      /*
       Sets the vertex shader. Do not allow the shader to go out of scope.
       */
      void vs(const content::shader* shdr);

      /*
       Sets the pixel shader. Do not allow the shader to go out of scope.
       */
      void ps(const content::shader* shdr);

      /*
       Sets the geometry shader. Do not allow the shader to go out of scope.
       */
      void gs(const content::shader* shdr);

      /*
       Sets the domain shader. Do not allow the shader to go out of scope.
       */
      void ds(const content::shader* shdr);

      /*
       Sets the hull shader. Do not allow the shader to go out of scope.
       */
      void hs(const content::shader* shdr);

      /*
       Sets the rasterizer. Do not allow the pointer to go out of scope.
       */
      void rasterizer_state(const content::rasterizer* rstzr);

      /*
       Sets the blend state. Do not allow the pointer to go out of scope.
       */
      void blend_state(const content::blender* blndr);

      private:
      struct impl;
      impl* m_impl_p = nullptr;
   };
}