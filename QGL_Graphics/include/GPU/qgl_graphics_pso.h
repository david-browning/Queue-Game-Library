#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Components/Content/qgl_shader.h"
#include "include/Components/Content/qgl_vertex_layout.h"
#include "include/GPU/Render/qgl_multisampler.h"
#include "include/GPU/qgl_frame.h"

namespace qgl::graphics::gpu
{
   /*
    Represents a collection of shaders and other parameters describing how to
    draw geometry. This includes the rasterizer, blend, and depth stencil
    states.
    */
   class graphics_pso : public ipso
   {
      public:
      graphics_pso(root_signature& rootSig,
                   const frame_stager& frms,
                   const shader_stager& shdrs,
                   const multisampler& smplr,
                   const components::vertex_layout& vLayout,
                   graphics_device* device_p,
                   size_t nodeMask) :
         m_dev_p(device_p)
      {
         //Set the flags.
         m_psoDesc.Flags = PSO_DEFAULT_FLAG;


         auto& firstFrame = frms.front();

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

         frames(frms);
         shaders(shdrs);

         winrt::check_hresult(m_dev_p->dev_3d()->CreateGraphicsPipelineState(
            &m_psoDesc,
            IID_PPV_ARGS(m_pipelineState.put())));
      }

      graphics_pso(const graphics_pso&) = delete;

      graphics_pso(graphics_pso&& x) noexcept :
         ipso(std::move(x)),
         m_pipelineState(std::move(x.m_pipelineState)),
         m_psoDesc(std::move(x.m_psoDesc)),
         m_dev_p(std::move(x.m_dev_p))
      {
         x.m_pipelineState = nullptr;
         x.m_dev_p = nullptr;
      }

      virtual ~graphics_pso() noexcept
      {
         m_pipelineState = nullptr;
         m_dev_p = nullptr;
      }

      /*
       Sets the depth stencil state, format, and render target views. If the
       frames need to be recreated, the PSO does not have to be updated.
       This only needs to be called a second time if the render target format,
       depth stencil format, or depth stencil description change. This is
       unlikely after the PSO is built.
       Throws E_INVALIDARG if there are too many frames.
       */
      void frames(const frame_stager& frms)
      {
         static constexpr auto maxRTVs =
            sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC::RTVFormats) /
            sizeof(DXGI_FORMAT);

         auto numFrames = frms.size();
         if (numFrames > maxRTVs)
         {
            OutputDebugString(L"Maximum of 8 RTVs allowed.");
            winrt::throw_hresult(E_INVALIDARG);
         }

         m_psoDesc.NumRenderTargets = static_cast<UINT>(numFrames);

         auto frmIt = frms.begin();
         for (size_t i = 0; i < numFrames; i++, frmIt++)
         {
            m_psoDesc.RTVFormats[i] = (*frmIt)->frame_buffer().format();
            m_psoDesc.DSVFormat = (*frmIt)->frame_stencil().format();
            m_psoDesc.DepthStencilState =
               (*frmIt)->frame_stencil().depth_desc();
         }
      }

      /*
       Returns a pointer to the D3D pipeline state.
       Returns nullptr if the pipeline cannot be
       created, possibly due to an incorrect parameter.
       */
      virtual const ID3D12PipelineState* get() const noexcept override
      {
         return m_pipelineState.get();
      }

      /*
       Returns a pointer to the D3D pipeline state.
       Returns nullptr if the pipeline cannot be
       created, possibly due to an incorrect parameter.
       */
      virtual ID3D12PipelineState* get() noexcept override
      {
         return m_pipelineState.get();
      }

      private:

      void shaders(const shader_stager& shdrs)
      {
         // Keep track that no shader is set twice.
         std::unordered_set<descriptors::shader_types> seenShaders;

         for (auto& shdr : shdrs)
         {
            auto type = shdr->type();

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
               case descriptors::shader_types::ds:
               {
                  m_psoDesc.DS = shdr->byte_code();
                  break;
               }
               case descriptors::shader_types::gs:
               {
                  m_psoDesc.GS = shdr->byte_code();
                  break;
               }
               case descriptors::shader_types::hs:
               {
                  m_psoDesc.HS = shdr->byte_code();
                  break;
               }
               case descriptors::shader_types::vs:
               {
                  m_psoDesc.VS = shdr->byte_code();
                  break;
               }
               case descriptors::shader_types::ps:
               {
                  m_psoDesc.PS = shdr->byte_code();
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
         }
         
         //Make sure we have a PS and VS as they are required.
         if (seenShaders.count(descriptors::shader_types::vs) == 0 ||
             seenShaders.count(descriptors::shader_types::ps) == 0)
         {
#ifdef DEBUG
            OutputDebugString(L"No vertex or pixel shader was found.");
#endif
            winrt::throw_hresult(E_INVALIDARG);
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

      graphics_device* m_dev_p;

#ifdef DEBUG
      static constexpr D3D12_PIPELINE_STATE_FLAGS PSO_DEFAULT_FLAG =
         D3D12_PIPELINE_STATE_FLAG_NONE;
#else
      static constexpr D3D12_PIPELINE_STATE_FLAGS PSO_DEFAULT_FLAG =
         D3D12_PIPELINE_STATE_FLAG_NONE;
#endif
   };
}