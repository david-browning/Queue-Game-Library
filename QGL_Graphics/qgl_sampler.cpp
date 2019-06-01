#include "pch.h"
#include "include\Content\qgl_sampler.h"

namespace qgl::graphics::content
{
   sampler::sampler(const content::buffers::SAMPLER_BUFFER* buffer, 
                    const wchar_t* name,
                    const qgl::content::content_id id) :
      content_item(name, id,
                   qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_DESCRIPTION,
                   qgl::content::CONTENT_LOADER_IDS::CONTENT_LOADER_ID_SAMPLER)
   {
      m_samplerDesc.AddressU = buffer->address_u();
      m_samplerDesc.AddressV = buffer->address_v();
      m_samplerDesc.AddressW = buffer->address_w();

      memcpy(&m_samplerDesc.BorderColor, 
             buffer->border_color(), 
             sizeof(float) * 4);

      m_samplerDesc.ComparisonFunc = buffer->comparision();
      m_samplerDesc.Filter = buffer->filter();
      m_samplerDesc.MaxAnisotropy = buffer->max_anisotropy();
      m_samplerDesc.MaxLOD = buffer->max_lod();
      m_samplerDesc.MinLOD = buffer->min_lod();
      m_samplerDesc.MipLODBias = buffer->mip_lod_bias();
   }

   const sampler::ResourceDescriptionT* sampler::description() const
   {
      return &m_samplerDesc;
   }

   const sampler::ViewDescriptionT* sampler::view() const
   {
      throw std::runtime_error("samplers do not have a view description.");
   }
}