#include "pch.h"
#include "include/GPU/qgl_pso.h"
#include "include\GPU\qgl_pso.h"
#include "include/GPU/Render/qgl_frame.h"
#include "include/GPU/Render/qgl_blender.h"

namespace qgl::graphics::gpu
{
   struct pipeline_state::impl
   {
      impl(d3d_device* dev_p) :
         m_pipelineState(nullptr),
         m_isFinalized(false),
         PSODesc(),
         m_dev(dev_p)
      {

      }

      impl(const impl& r) :
         PSODesc(r.PSODesc),
         m_pipelineState(nullptr),
         m_inputLayout(r.m_inputLayout),
         m_isFinalized(false),
         m_dev(r.m_dev)
      {
         //Set m_isFinalized to false so that the PSO will be reconstructed in 
         //new GPU memory.
      }

      impl(impl&& r) :
         PSODesc(std::move(r.PSODesc)),
         m_pipelineState(nullptr),
         m_inputLayout(std::move(r.m_inputLayout)),
         m_isFinalized(false),
         m_dev(r.m_dev)
      {
         //Set m_isFinalized to false so that the PSO will be reconstructed in 
         //new GPU memory.
      }

      ~impl() noexcept = default;

      ID3D12PipelineState* get_and_finalize() const
      {
         if (!m_isFinalized)
         {
            //Check that a vertex shader is set.
            if (PSODesc.VS.pShaderBytecode != nullptr)
            {
               auto hr = m_dev->CreateGraphicsPipelineState(
                  &PSODesc,
                  IID_PPV_ARGS(m_pipelineState.put()));

               winrt::check_hresult(hr);
               m_isFinalized = true;
            }
            else
            {
               throw std::runtime_error("No vertex shader is set.");
            }
         }

         return m_pipelineState.get();
      }

      void frames(const render::frame* frms,
                  size_t numFrames)
      {
         static constexpr auto maxRTVs =
            sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC::RTVFormats) /
            sizeof(DXGI_FORMAT);

         if (numFrames > maxRTVs)
         {
            throw std::invalid_argument("Maximum of 8 RTVs allowed.");
         }

         PSODesc.NumRenderTargets = static_cast<UINT>(numFrames);
         for (size_t i = 0; i < numFrames; i++)
         {
            auto stncl = frms[i].frame_stencil();
            auto renderTarget = frms[i].frame_buffer();

            PSODesc.RTVFormats[i] = renderTarget->format();
            PSODesc.DSVFormat = stncl->format();
            PSODesc.DepthStencilState = *stncl->depth_desc();
            i++;
         }
      }

      mutable winrt::com_ptr<ID3D12PipelineState> m_pipelineState;
      mutable bool m_isFinalized;

      /*
       Layout of the vertices that get passed to the input assembler.
       Most likely, it will contain a POSITION, NORMAL, TEXCOORD, TANGENT
       */
      std::vector<D3D12_INPUT_ELEMENT_DESC> m_inputLayout;

      /*
       The description used to create the pipeline state.
       */
      D3D12_GRAPHICS_PIPELINE_STATE_DESC PSODesc;

      d3d_device* m_dev;
   };

   pipeline_state::pipeline_state(d3d_device* dev_p) :
      m_impl_p(new impl(dev_p))
   {
   }

   pipeline_state::pipeline_state(const pipeline_state& r)
   {
      delete m_impl_p;
      m_impl_p = new impl(*r.m_impl_p);
   }

   pipeline_state::pipeline_state(pipeline_state&& m)
   {
      delete m_impl_p;
      m_impl_p = m.m_impl_p;
      m.m_impl_p = nullptr;
   }

   pipeline_state::~pipeline_state() noexcept
   {
      delete m_impl_p;
      m_impl_p = nullptr;
   }

   void pipeline_state::frames(const render::frame* frms, 
                               size_t numFrames)
   {
      m_impl_p->frames(frms, numFrames);
   }

   const ID3D12PipelineState* pipeline_state::get() const
   {
      return m_impl_p->get_and_finalize();
   }

   ID3D12PipelineState * pipeline_state::get()
   {
      return m_impl_p->get_and_finalize();
   }

   void pipeline_state::sample_description(DXGI_SAMPLE_DESC desc)
   {
      m_impl_p->PSODesc.SampleDesc = desc;
   }

   void pipeline_state::sample_description(UINT msaaCount,
                                           UINT msaaQuality)
   {
      m_impl_p->PSODesc.SampleDesc.Count = msaaCount;
      m_impl_p->PSODesc.SampleDesc.Quality = msaaQuality;
   }

   void pipeline_state::layout(const D3D12_INPUT_ELEMENT_DESC* descs,
                               size_t numDescs,
                               D3D12_PRIMITIVE_TOPOLOGY_TYPE topo,
                               D3D12_INDEX_BUFFER_STRIP_CUT_VALUE)
   {
      m_impl_p->m_inputLayout.resize(numDescs);
      memcpy(m_impl_p->m_inputLayout.data(),
             descs,
             sizeof(D3D12_INPUT_ELEMENT_DESC) * numDescs);
      m_impl_p->PSODesc.InputLayout.NumElements =
         static_cast<UINT>(m_impl_p->m_inputLayout.size());
      m_impl_p->PSODesc.InputLayout.pInputElementDescs =
         m_impl_p->m_inputLayout.data();

      m_impl_p->PSODesc.PrimitiveTopologyType = topo;
   }

   void pipeline_state::vs(const content::shader* shdr)
   {
      m_impl_p->PSODesc.VS = *shdr->byte_code();
   }

   void pipeline_state::ps(const content::shader* shdr)
   {
      m_impl_p->PSODesc.PS = *shdr->byte_code();
   }

   void pipeline_state::gs(const content::shader* shdr)
   {
      m_impl_p->PSODesc.GS = *shdr->byte_code();
   }

   void pipeline_state::ds(const content::shader* shdr)
   {
      m_impl_p->PSODesc.DS = *shdr->byte_code();
   }

   void pipeline_state::hs(const content::shader* shdr)
   {
      m_impl_p->PSODesc.HS = *shdr->byte_code();
   }

   void pipeline_state::rasterizer_state(const content::rasterizer* rstzr)
   {
      m_impl_p->PSODesc.RasterizerState = *rstzr->description();
   }

   void pipeline_state::blend_state(const render::blender* blndr)
   {
      m_impl_p->PSODesc.BlendState = *blndr->description();
      m_impl_p->PSODesc.SampleMask = blndr->mask();
   }
}