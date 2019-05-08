#pragma once
#include "qgl_graphics_include.h"
#include "qgl_root_signature.h"
#include "qgl_depth_stencil.h"
#include "qgl_ishader.h"
#include "qgl_render_target.h"
#include "qgl_blender.h"
#include "qgl_rasterizer.h"
#include "qgl_frame.h"

namespace qgl::graphics
{
   class frame;

   class LIB_EXPORT pipeline_state
   {
      public:
      #ifdef DEBUG
      static constexpr D3D12_PIPELINE_STATE_FLAGS PSO_DEFAULT_FLAG =
         D3D12_PIPELINE_STATE_FLAG_NONE;
      #else
      static constexpr D3D12_PIPELINE_STATE_FLAGS PSO_DEFAULT_FLAG =
         D3D12_PIPELINE_STATE_FLAG_NONE;
      #endif

      pipeline_state(const winrt::com_ptr<d3d_device>& dev_p,
                     root_signature& rootSig,
                     D3D12_PIPELINE_STATE_FLAGS flags = PSO_DEFAULT_FLAG,
                     UINT nodeMask = 0);

      pipeline_state(const pipeline_state& r);

      pipeline_state(pipeline_state&& r);

      virtual ~pipeline_state();

      inline const ID3D12PipelineState* get() const
      {
         p_finalize();
         return m_pso_p.get();
      }

      inline ID3D12PipelineState* get()
      {
         p_finalize();
         return m_pso_p.get();
      }

      /*
       Sets the depth stencil state, format, and render target views.
       */
      //void frames(std::vector<std::shared_ptr<frame>>::iterator first, 
      //            std::vector<std::shared_ptr<frame>>::iterator last)
      template<class InputIterator>
      void frames(InputIterator first, InputIterator last)
      {
         static constexpr auto maxRTVs = sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC::RTVFormats) /
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
       Sets the sample description.
       */
      void sample_description(const DXGI_SAMPLE_DESC& desc);

      /*
       Sets the sample description. Default values are provided.
       */
      void sample_description(UINT msaaCount = 1, UINT msaaQuality = 0);

      /*
       Sets the vertex layout and topology for the input assembler.
       Most likely, it will contain a POSITION, NORMAL, TEXCOORD, TANGENT
       */
      void layout(std::initializer_list<D3D12_INPUT_ELEMENT_DESC>& descs,
                  D3D12_PRIMITIVE_TOPOLOGY_TYPE topo,
                  D3D12_INDEX_BUFFER_STRIP_CUT_VALUE = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED);

      /*
       Sets the vertex shader. Do not allow the shader to go out of scope.
       */
      void shader(const ishader<SHADER_TYPES::SHADER_TYPE_VERTEX>& shdr)
      {
         m_psoDesc.VS = shdr.byte_code();
      }

      /*
       Sets the pixel shader. Do not allow the shader to go out of scope.
       */
      void shader(const ishader<SHADER_TYPES::SHADER_TYPE_PIXEL>& shdr)
      {
         m_psoDesc.PS = shdr.byte_code();
      }

      /*
       Sets the geometry shader. Do not allow the shader to go out of scope.
       */
      void shader(const ishader<SHADER_TYPES::SHADER_TYPE_GEOMETRY>& shdr)
      {
         m_psoDesc.GS = shdr.byte_code();
      }

      /*
       Sets the domain shader. Do not allow the shader to go out of scope.
       */
      void shader(const ishader<SHADER_TYPES::SHADER_TYPE_DOMAIN>& shdr)
      {
         m_psoDesc.DS = shdr.byte_code();
      }

      /*
       Sets the hull shader. Do not allow the shader to go out of scope.
       */
      void shader(const ishader<SHADER_TYPES::SHADER_TYPE_HULL>& shdr)
      {
         m_psoDesc.HS = shdr.byte_code();
      }

      void rasterizer_state(const rasterizer& rstzr);

      void blend_state(const blender& blndr);

      pipeline_state& operator=(const pipeline_state& r)
      {
         if (&r != this)
         {
            m_pso_p = r.m_pso_p;
            m_dev_p = r.m_dev_p;
            m_inputLayouts = r.m_inputLayouts;
            m_psoDesc = r.m_psoDesc;
         }
         return *this;
      }

      private:
      inline void p_finalize() const
      {
         if (!m_isFinalized)
         {
            //Check that a vertex shader is set.
            if (m_psoDesc.VS.pShaderBytecode != nullptr)
            {
               auto hr = m_dev_p->CreateGraphicsPipelineState(&m_psoDesc, IID_PPV_ARGS(m_pso_p.put()));
               winrt::check_hresult(hr);
               m_isFinalized = true;
            }
            else
            {
               throw std::runtime_error("No vertex shader is set.");
            }
         }
      }

      mutable bool m_isFinalized;

      #pragma warning(push)
      #pragma warning(disable: 4251)
      
      mutable winrt::com_ptr<ID3D12PipelineState> m_pso_p;

      winrt::com_ptr<d3d_device> m_dev_p;

      /*
       Layout of the vertices that get passed to the input assembler.
       Most likely, it will contain a POSITION, NORMAL, TEXCOORD, TANGENT
       */
      std::vector<D3D12_INPUT_ELEMENT_DESC> m_inputLayouts;

      #pragma warning(pop)

      /*
       The description used to create the pipeline state.
       */
      D3D12_GRAPHICS_PIPELINE_STATE_DESC m_psoDesc;
   };
}