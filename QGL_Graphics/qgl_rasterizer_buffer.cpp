#include "pch.h"
#include "include/qgl_rasterizer_buffer.h"

qgl::graphics::RASTERIZER_BUFFER::RASTERIZER_BUFFER() :
   m_depthBiasClamp(D3D12_DEFAULT_DEPTH_BIAS_CLAMP),
   m_slopeScaledDepthBias(D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS),
   m_depthBias(D3D12_DEFAULT_DEPTH_BIAS),
   m_forcedSampleCount(0),
   m_fill_mode(D3D12_FILL_MODE_SOLID),
   m_cull_mode(D3D12_CULL_MODE_BACK),
   m_conservativeRaster(D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF),
   m_isFrontCounterClockwise(false),
   m_isDepthClip(true),
   m_isMultisample(false),
   m_isAntialiasedLine(false)
{
}

qgl::graphics::RASTERIZER_BUFFER::RASTERIZER_BUFFER(const RASTERIZER_BUFFER& r) :
   m_depthBiasClamp(r.m_depthBiasClamp),
   m_slopeScaledDepthBias(r.m_slopeScaledDepthBias),
   m_depthBias(r.m_depthBias),
   m_forcedSampleCount(r.m_forcedSampleCount),
   m_fill_mode(r.m_fill_mode),
   m_cull_mode(r.m_cull_mode),
   m_conservativeRaster(r.m_conservativeRaster),
   m_isFrontCounterClockwise(r.m_isFrontCounterClockwise),
   m_isDepthClip(r.m_isDepthClip),
   m_isMultisample(r.m_isMultisample),
   m_isAntialiasedLine(r.m_isAntialiasedLine)
{
}

qgl::graphics::RASTERIZER_BUFFER::RASTERIZER_BUFFER(RASTERIZER_BUFFER&& r) :
   m_depthBiasClamp(r.m_depthBiasClamp),
   m_slopeScaledDepthBias(r.m_slopeScaledDepthBias),
   m_depthBias(r.m_depthBias),
   m_forcedSampleCount(r.m_forcedSampleCount),
   m_fill_mode(r.m_fill_mode),
   m_cull_mode(r.m_cull_mode),
   m_conservativeRaster(r.m_conservativeRaster),
   m_isFrontCounterClockwise(r.m_isFrontCounterClockwise),
   m_isDepthClip(r.m_isDepthClip),
   m_isMultisample(r.m_isMultisample),
   m_isAntialiasedLine(r.m_isAntialiasedLine)
{
}
