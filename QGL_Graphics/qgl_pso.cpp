#include "pch.h"
#include "include/qgl_pso.h"
#include "include/qgl_frame.h"

qgl::graphics::pipeline_state::pipeline_state(const winrt::com_ptr<d3d_device>& dev_p,
                                              root_signature& rootSig,
                                              D3D12_PIPELINE_STATE_FLAGS flags,
                                              UINT nodeMask) :
   m_psoDesc(),
   m_dev_p(dev_p),
   m_pso_p(nullptr),
   m_inputLayouts(0),
   m_isFinalized(false)
{
   m_psoDesc.pRootSignature = rootSig.get();
   m_psoDesc.Flags = flags;
   m_psoDesc.NodeMask = nodeMask;
}

qgl::graphics::pipeline_state::pipeline_state(const pipeline_state& r) :
   m_psoDesc(r.m_psoDesc),
   m_dev_p(r.m_dev_p),
   m_pso_p(nullptr),
   m_inputLayouts(r.m_inputLayouts),
   m_isFinalized(false)
{
   //Set m_isFinalized to false so that the PSO will be reconstructed in new GPU memory.
   //Perform a deep copy instead of a shallow one by copying the pointer.
}

qgl::graphics::pipeline_state::pipeline_state(pipeline_state&& r) :
   m_psoDesc(std::move(r.m_psoDesc)),
   m_dev_p(std::move(r.m_dev_p)),
   m_pso_p(nullptr),
   m_inputLayouts(std::move(r.m_inputLayouts)),
   m_isFinalized(false)
{
   //Set m_isFinalized to false so that the PSO will be reconstructed in new GPU memory.
   //Perform a deep copy instead of a shallow one by copying the pointer.
}

qgl::graphics::pipeline_state::~pipeline_state()
{
}

void qgl::graphics::pipeline_state::sample_description(const DXGI_SAMPLE_DESC& desc)
{
   m_psoDesc.SampleDesc = desc;
}

void qgl::graphics::pipeline_state::sample_description(UINT msaaCount, UINT msaaQuality)
{
   m_psoDesc.SampleDesc.Count = msaaCount;
   m_psoDesc.SampleDesc.Quality = msaaQuality;
}

void qgl::graphics::pipeline_state::layout(std::initializer_list<D3D12_INPUT_ELEMENT_DESC>& descs,
                                           D3D12_PRIMITIVE_TOPOLOGY_TYPE topo,
                                           D3D12_INDEX_BUFFER_STRIP_CUT_VALUE)

{
   m_inputLayouts = descs;
   m_psoDesc.InputLayout.NumElements = static_cast<UINT>(m_inputLayouts.size());
   m_psoDesc.InputLayout.pInputElementDescs = m_inputLayouts.data();

   m_psoDesc.PrimitiveTopologyType = topo;
}

void qgl::graphics::pipeline_state::rasterizer_state(const rasterizer& rstzr)
{
   m_psoDesc.RasterizerState = rstzr.description();
}

void qgl::graphics::pipeline_state::blend_state(const blender& blndr)
{
   m_psoDesc.BlendState = blndr.description();
   m_psoDesc.SampleMask = blndr.mask();
}
