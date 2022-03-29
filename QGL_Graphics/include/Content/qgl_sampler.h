#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/qgl_igpu_buffer.h"
#include "include/Content/Content-Descriptors/qgl_sampler_descriptor.h"

namespace qgl::graphics
{
   /*
    Represents a sampler. Samplers do not have a resource. Instead, they only
    have a description and can be added to a descriptor heap.
    */
   class sampler : public gpu::igpu_buffer<D3D12_SAMPLER_DESC, 
                                           nullptr_t, 
                                           gpu_resource>
   {
      public:
      using ResourceDescriptionT = D3D12_SAMPLER_DESC;
      using ViewDescriptionT = nullptr_t;

      /*
       Constructor
       */
      sampler(const descriptors::sampler_descriptor& buffer)
      {
         m_samplerDesc.AddressU =
            static_cast<D3D12_TEXTURE_ADDRESS_MODE>(buffer.addressu);
         m_samplerDesc.AddressV =
            static_cast<D3D12_TEXTURE_ADDRESS_MODE>(buffer.addressv);
         m_samplerDesc.AddressW =
            static_cast<D3D12_TEXTURE_ADDRESS_MODE>(buffer.addressw);

         for (auto i = 0; i < 4; i++)
         {
            m_samplerDesc.BorderColor[i] =
               static_cast<float>(buffer.border[i]);
         }

         m_samplerDesc.ComparisonFunc =
            static_cast<D3D12_COMPARISON_FUNC>(buffer.comparison_func);
         m_samplerDesc.Filter =
            static_cast<D3D12_FILTER>(buffer.filter);
         m_samplerDesc.MaxAnisotropy = buffer.anisotropy_max;
         m_samplerDesc.MaxLOD = static_cast<float>(buffer.lod_max);
         m_samplerDesc.MinLOD = static_cast<float>(buffer.lod_min);
         m_samplerDesc.MipLODBias = static_cast<float>(buffer.mip_lod_bias);
      }

      /*
       Copy constructor.
       */
      sampler(const sampler& r) = default;

      /*
       Move constructor.
       */
      sampler(sampler&& r) = default;

      /*
       Destructor.
       */
      virtual ~sampler() = default;

      /*
       Returns a description of the sampler.
       */
      virtual const ResourceDescriptionT* description() const
      {
         return &m_samplerDesc;
      }

      virtual const ViewDescriptionT* view() const
      {
         throw std::runtime_error("samplers do not have a view description.");
      }

      private:
      /*
       Populated by the constructor.
       */
      ResourceDescriptionT m_samplerDesc;
   };
}