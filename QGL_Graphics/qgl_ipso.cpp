#include "pch.h"
#include "include/GPU/qgl_ipso.h"

using namespace qgl::content;

namespace qgl::graphics
{
   class pipeline_state_1_0 : public ipso
   {
      public:
      #ifdef DEBUG
      static constexpr D3D12_PIPELINE_STATE_FLAGS PSO_DEFAULT_FLAG =
         D3D12_PIPELINE_STATE_FLAG_NONE;
      #else
      static constexpr D3D12_PIPELINE_STATE_FLAGS PSO_DEFAULT_FLAG =
         D3D12_PIPELINE_STATE_FLAG_NONE;
      #endif

      pipeline_state_1_0(static_ptr_ref<graphics::d3d_device> dev_p) :
         m_dev(dev_p)
      {

      }

      virtual ~pipeline_state_1_0() noexcept = default;

      virtual void release()
      {
         //Calls the destructor.
         delete this;
      }

      HRESULT make(const IPSO_CREATION_PARAMS* params_p) noexcept
      {
         auto firstFrame = params_p->Frames[0];

         //Set the flags.
         m_psoDesc.Flags = PSO_DEFAULT_FLAG;

         //Set the blender
         auto blndr = firstFrame->const_frame_blender();
         m_psoDesc.BlendState = *blndr->description();
         m_psoDesc.SampleMask = blndr->mask();

         //Set the sampling.
         m_psoDesc.SampleDesc.Count = params_p->Sampler->count();
         m_psoDesc.SampleDesc.Quality = params_p->Sampler->quality();

         //Set the rasterizer.
         auto rastr = firstFrame->const_frame_rasterizer();
         m_psoDesc.RasterizerState = *rastr->description();

         //Set the vertex layout
         m_psoDesc.PrimitiveTopologyType = params_p->VertexDesc->topology();
         m_psoDesc.IBStripCutValue = params_p->VertexDesc->strip_cut();
         m_psoDesc.InputLayout.NumElements =
            static_cast<UINT>(params_p->VertexDesc->size());
         m_psoDesc.InputLayout.pInputElementDescs =
            params_p->VertexDesc->data();

         //Set the root signature.
         m_psoDesc.pRootSignature = params_p->RootSignature->get();

         //Set the node mask
         m_psoDesc.NodeMask = params_p->NodeMask;

         //TODO: Add support for stream output?
         //m_psoDesc.StreamOutput;

         this->frames(params_p->Frames, params_p->NumFrames);
         return set_shaders(params_p);
      }

      virtual HRESULT frames(gpu::render::iframe** frms,
                             size_t numFrames) noexcept
      {
         static constexpr auto maxRTVs =
            sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC::RTVFormats) /
            sizeof(DXGI_FORMAT);

         if (numFrames > maxRTVs)
         {
            OutputDebugString(L"Maximum of 8 RTVs allowed.");
            return E_INVALIDARG;
         }

         m_psoDesc.NumRenderTargets = static_cast<UINT>(numFrames);
         for (size_t i = 0; i < numFrames; i++)
         {
            auto stncl = frms[i]->const_frame_stencil();
            auto renderTarget = frms[i]->const_frame_buffer();

            m_psoDesc.RTVFormats[i] = renderTarget->format();
            m_psoDesc.DSVFormat = stncl->format();
            m_psoDesc.DepthStencilState = *stncl->depth_desc();
            i++;
         }

         return S_OK;
      }

      virtual const ID3D12PipelineState* const_get() const noexcept
      {
         return get_and_finalize();
      }

      virtual ID3D12PipelineState* get() noexcept
      {
         return get_and_finalize();
      }

      private:
      HRESULT set_shaders(const IPSO_CREATION_PARAMS* params_p) noexcept
      {
         //Set the shaders. Keep track that no shader is set twice.
         std::set<buffers::SHADER_TYPES> seenShaders;

         for (size_t i = 0; i < params_p->NumShaders; i++)
         {
            auto shdr_p = params_p->Shaders[i];
            auto type = shdr_p->type();

            //Make sure we haven't seen this shader before.
            if (seenShaders.count(type) > 0)
            {
               #ifdef DEBUG
               OutputDebugString(
                  L"Two shaders of the same type were specified.");
               #endif
               return E_INVALIDARG;
            }

            seenShaders.insert(type);

            //Depending on the type of shader, set the appropriate parameters
            //for the pipeline state.
            switch (type)
            {
               case buffers::SHADER_TYPES::SHADER_TYPE_DS:
               {
                  m_psoDesc.DS = *shdr_p->byte_code();
                  break;
               }
               case buffers::SHADER_TYPES::SHADER_TYPE_GS:
               {
                  m_psoDesc.GS = *shdr_p->byte_code();
                  break;
               }
               case buffers::SHADER_TYPES::SHADER_TYPE_HS:
               {
                  m_psoDesc.HS = *shdr_p->byte_code();
                  break;
               }
               case buffers::SHADER_TYPES::SHADER_TYPE_VS:
               {
                  m_psoDesc.VS = *shdr_p->byte_code();
                  break;
               }
               case buffers::SHADER_TYPES::SHADER_TYPE_PS:
               {
                  m_psoDesc.PS = *shdr_p->byte_code();
                  break;
               }
               default:
               {
                  #ifdef DEBUG
                  OutputDebugString(L"A shader has an unknown type.");
                  #endif
                  return E_UNEXPECTED;
               }
            }

            //Make sure we have a PS and VS as they are required.
            if (seenShaders.count(buffers::SHADER_TYPES::SHADER_TYPE_VS) == 0 ||
                seenShaders.count(buffers::SHADER_TYPES::SHADER_TYPE_PS) == 0)
            {
               #ifdef DEBUG
               OutputDebugString(L"No vertex or pixel shader was found.");
               #endif
               return E_INVALIDARG;
            }
         }

         return S_OK;
      }

      ID3D12PipelineState* get_and_finalize() const
      {
         if (!m_isFinalized)
         {
            auto hr = m_dev->CreateGraphicsPipelineState(
               &m_psoDesc,
               IID_PPV_ARGS(m_pipelineState.put()));

            if (FAILED(hr))
            {
               return nullptr;
            }

            m_isFinalized = true;
         }

         return m_pipelineState.get();
      }

      /*
       The D3D12 pipeline state.
       */
      mutable winrt::com_ptr<ID3D12PipelineState> m_pipelineState;

      /*
       True if the pipeline state has been constructed.
       */
      mutable bool m_isFinalized;

      /*
       The description used to create the pipeline state.
       */
      D3D12_GRAPHICS_PIPELINE_STATE_DESC m_psoDesc;

      static_ptr_ref<graphics::d3d_device> m_dev;
   };

   HRESULT qgl_make_pipeline(static_ptr_ref<graphics::d3d_device> dev_p,
                             const IPSO_CREATION_PARAMS* params_p,
                             qgl_version_t v,
                             ipso** out_p) noexcept
   {
      //Make sure out_p is not nullptr.
      if (out_p == nullptr)
      {
         #ifdef DEBUG
         OutputDebugString(L"out_p cannot be nullptr.");
         #endif
         return E_INVALIDARG;
      }

      //This is what will be assigned to out_p.
      ipso* ret = nullptr;

      switch (std::hash<qgl_version_t>{}(v))
      {
         case qgl::hashes::VERSION_0_1_HASH:
         case qgl::hashes::VERSION_0_2_HASH:
         {
            ret = new(std::nothrow)pipeline_state_1_0(dev_p);

            if (ret == nullptr)
            {
               #ifdef DEBUG
               OutputDebugString(L"Out of memory!");
               #endif
               return E_OUTOFMEMORY;
            }

            auto hr = (dynamic_cast<pipeline_state_1_0*>(ret))->make(params_p);
            if (FAILED(hr))
            {
               return hr;
            }
            break;
         }
         default:
         {
            #ifdef DEBUG
            OutputDebugString(L"This QGL version is not supported.");
            #endif
            return E_NOINTERFACE;
         }
      }

      *out_p = ret;
      return S_OK;
   }
}