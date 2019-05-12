#include "pch.h"
#include "include/qgl_sampler.h"

qgl::graphics::sampler::sampler(const SAMPLER_BUFFER& buffer,
                                winrt::com_ptr<d3d_device>& dev_p,
                                const std::wstring& name,
                                const content::content_id id) :
   igpu_buffer(dev_p),
   content_item(name, id,
                qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_DESCRIPTION,
                qgl::content::CONTENT_LOADER_IDS::CONTENT_LOADER_ID_SAMPLER)
{
   m_samplerDesc.AddressU = buffer.address_u();
   m_samplerDesc.AddressV = buffer.address_v();
   m_samplerDesc.AddressW = buffer.address_w();

   memcpy(&m_samplerDesc.BorderColor, buffer.border_color(), sizeof(float) * 4);

   m_samplerDesc.ComparisonFunc = buffer.comparision();
   m_samplerDesc.Filter = buffer.filter();
   m_samplerDesc.MaxAnisotropy = buffer.max_anisotropy();
   m_samplerDesc.MaxLOD = buffer.max_lod();
   m_samplerDesc.MinLOD = buffer.min_lod();
   m_samplerDesc.MipLODBias = buffer.mip_lod_bias();
}
