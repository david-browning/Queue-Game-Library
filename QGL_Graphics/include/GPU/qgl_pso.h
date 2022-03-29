#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/qgl_frame.h"
#include "include/GPU/Root-Signature/qgl_root_signature.h"
#include "include/Content/qgl_shader.h"
#include "include/Content/qgl_multisampler.h"
#include "include/Content/qgl_vertex_layout.h"

namespace qgl::graphics::gpu
{
   /*
    Represents a collection of shaders and other parameters describing how to
    draw geometry. This includes the rasterizer, blend, and depth stencil
    states.
    Creating the pipeline state object is deferred until calling get(). The
    pipeline state parameters must be set first.
    */
   class pso
   {
      public:
      template<class FrameIt, class ShaderIt>
      pso(FrameIt firstFrame, FrameIt lastFrame,
         ShaderIt firstShader, ShaderIt lastShader,
         device_3d_ptr&& device_p,
         const multisampler& smplr,
         const vertex_layout& vLayout,
         root_signature& rootSig,
         size_t nodeMask) :
         m_dev(std::forward<device_3d_ptr>(device_p))
      {
         //Set the flags.
         m_psoDesc.Flags = PSO_DEFAULT_FLAG;

         //Set the blender
         auto blndr = firstFrame->frame_blender();
         m_psoDesc.BlendState = firstFrame->frame_blender().description();
         m_psoDesc.SampleMask = firstFrame->frame_blender().mask();

         //Set the sampling.
         m_psoDesc.SampleDesc.Count = static_cast<UINT>(smplr.count());
         m_psoDesc.SampleDesc.Quality = static_cast<UINT>(smplr.quality());

         //Set the rasterizer.
         m_psoDesc.RasterizerState =
            firstFrame->frame_rasterizer().description();

         //Set the vertex layout
         m_psoDesc.PrimitiveTopologyType = vLayout.topology();
         m_psoDesc.IBStripCutValue = vLayout.strip_cut();;
         m_psoDesc.InputLayout.NumElements =
            static_cast<UINT>(vLayout.size());
         m_psoDesc.InputLayout.pInputElementDescs = vLayout.data();

         //Set the root signature.
         m_psoDesc.pRootSignature = rootSig.get();

         //Set the node mask
         m_psoDesc.NodeMask = static_cast<UINT>(nodeMask);

         //TODO: Add support for stream output?
         //m_psoDesc.StreamOutput;

         frames(firstFrame, lastFrame);
         shaders(firstShader, lastShader);

         winrt::check_hresult(m_dev->CreateGraphicsPipelineState(
            &m_psoDesc,
            IID_PPV_ARGS(m_pipelineState.put())));
      }

      pso(const pso&) = delete;

      pso(pso&&) = default;

      virtual ~pso() noexcept = default;

      /*
       Sets the depth stencil state, format, and render target views. If the
       frames need to be recreated, the PSO does not have to be updated.
       This only needs to be called a second time if the render target format,
       depth stencil format, or depth stencil description change. This is
       unlikely after the PSO is built.
       Throws E_INVALIDARG if there are too many frames.
       */
      template<class FrameIt>
      void frames(FrameIt first, FrameIt last)
      {
         static constexpr auto maxRTVs =
            sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC::RTVFormats) /
            sizeof(DXGI_FORMAT);

         auto numFrames = std::distance(first, last);
         if (numFrames > maxRTVs)
         {
            OutputDebugString(L"Maximum of 8 RTVs allowed.");
            winrt::throw_hresult(E_INVALIDARG);
         }

         m_psoDesc.NumRenderTargets = static_cast<UINT>(numFrames);
         for (size_t i = 0; i < numFrames; i++)
         {
            auto frameIt = first + i;
            m_psoDesc.RTVFormats[i] = frameIt->frame_buffer().format();
            m_psoDesc.DSVFormat = frameIt->frame_stencil().format();
            m_psoDesc.DepthStencilState = frameIt->frame_stencil().depth_desc();

            // TODO: Why was this here?
            //i++;
         }
      }

      /*
       Returns a pointer to the D3D pipeline state.
       Returns nullptr if the pipeline cannot be
       created, possibly due to an incorrect parameter.
       */
      const ID3D12PipelineState* get() const noexcept
      {
         return m_pipelineState.get();
      }

      /*
       Returns a pointer to the D3D pipeline state.
       Returns nullptr if the pipeline cannot be
       created, possibly due to an incorrect parameter.
       */
      ID3D12PipelineState* get() noexcept
      {
         return m_pipelineState.get();
      }

      private:
      
      template<class ShaderIt>
      void shaders(ShaderIt firstShader, ShaderIt lastShader)
      {
         // Keep track that no shader is set twice.
         std::unordered_set<shader_types> seenShaders;

         for (auto shader = firstShader; shader != lastShader; shader++)
         {
            auto type = shader->type();

            //Make sure we haven't seen this shader before.
            if (seenShaders.count(type) > 0)
            {
#ifdef DEBUG
               OutputDebugString(
                  L"Two shaders of the same type were specified.");
#endif
               winrt::throw_hresult(E_INVALIDARG);
            }

            seenShaders.insert(type);

            //Depending on the type of shader, set the appropriate parameters
            //for the pipeline state.
            switch (type)
            {
               case shader_types::ds:
               {
                  m_psoDesc.DS = shader->byte_code();
                  break;
               }
               case shader_types::gs:
               {
                  m_psoDesc.GS = shader->byte_code();
                  break;
               }
               case shader_types::hs:
               {
                  m_psoDesc.HS = shader->byte_code();
                  break;
               }
               case shader_types::vs:
               {
                  m_psoDesc.VS = shader->byte_code();
                  break;
               }
               case shader_types::ps:
               {
                  m_psoDesc.PS = shader->byte_code();
                  break;
               }
               default:
               {
#ifdef DEBUG
                  OutputDebugString(L"A shader has an unknown type.");
#endif
                  winrt::throw_hresult(E_UNEXPECTED);
               }
            }

            //Make sure we have a PS and VS as they are required.
            if (seenShaders.count(shader_types::vs) == 0 ||
                seenShaders.count(shader_types::ps) == 0)
            {
#ifdef DEBUG
               OutputDebugString(L"No vertex or pixel shader was found.");
#endif
               winrt::throw_hresult(E_INVALIDARG);
            }
         }
      }

      /*
       The D3D12 pipeline state.
       */
      winrt::com_ptr<ID3D12PipelineState> m_pipelineState;

      /*
       The description used to create the pipeline state.
       */
      D3D12_GRAPHICS_PIPELINE_STATE_DESC m_psoDesc;

      device_3d_ptr m_dev;

#ifdef DEBUG
      static constexpr D3D12_PIPELINE_STATE_FLAGS PSO_DEFAULT_FLAG =
         D3D12_PIPELINE_STATE_FLAG_NONE;
#else
      static constexpr D3D12_PIPELINE_STATE_FLAGS PSO_DEFAULT_FLAG =
         D3D12_PIPELINE_STATE_FLAG_NONE;
#endif
   };
}