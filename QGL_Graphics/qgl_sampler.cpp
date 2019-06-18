#include "pch.h"
#include "include\Content\qgl_sampler.h"

namespace qgl::content
{
   sampler::sampler(const content::buffers::SAMPLER_BUFFER* buffer,
                    const wchar_t* name,
                    const qgl::content::content_id id) :
      content_item(name, id,
                   qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_DESCRIPTION,
                   qgl::content::CONTENT_LOADER_IDS::CONTENT_LOADER_ID_SAMPLER)
   {
      m_samplerDesc.AddressU =
         static_cast<D3D12_TEXTURE_ADDRESS_MODE>(buffer->AddressU);
      m_samplerDesc.AddressV =
         static_cast<D3D12_TEXTURE_ADDRESS_MODE>(buffer->AddressV);
      m_samplerDesc.AddressW =
         static_cast<D3D12_TEXTURE_ADDRESS_MODE>(buffer->AddressW);

      for (auto i = 0; i < 4; i++)
      {
         m_samplerDesc.BorderColor[i] = buffer->BorderColor[i];
      }

      m_samplerDesc.ComparisonFunc =
         static_cast<D3D12_COMPARISON_FUNC>(buffer->ComparisonFunction);
      m_samplerDesc.Filter =
         static_cast<D3D12_FILTER>(buffer->Filter);
      m_samplerDesc.MaxAnisotropy = buffer->MaxAnisotropy;
      m_samplerDesc.MaxLOD = buffer->MaxLOD;
      m_samplerDesc.MinLOD = buffer->MinLOD;
      m_samplerDesc.MipLODBias = buffer->MipLODBias;
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