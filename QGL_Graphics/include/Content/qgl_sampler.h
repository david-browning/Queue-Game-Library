#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_sampler_buffer.h"
#include "include/GPU/Buffers/igpu_buffer.h"

namespace qgl::graphics::content
{
   /*
    Represents a sampler. Samplers do not have a resource. Instead, they only
    have a description and can be added to a descriptor heap.
    */
   class QGL_GRAPHICS_API sampler :
      public qgl::content::content_item,
      public gpu::buffers::igpu_buffer<D3D12_SAMPLER_DESC, 
      nullptr_t, 
      d3d_resource>
   {
      public:
      using ResourceDescriptionT = D3D12_SAMPLER_DESC;
      using ViewDescriptionT = nullptr_t;

      sampler(const content::buffers::SAMPLER_BUFFER* buffer,
              const wchar_t* name,
              const qgl::content::content_id id);

      sampler(const sampler& r) = default;

      sampler(sampler&& r) = default;

      virtual ~sampler() = default;

      virtual const ResourceDescriptionT* description() const;

      virtual const ViewDescriptionT* view() const;

      private:
      /*
       Populated by the constructor.
       */
      ResourceDescriptionT m_samplerDesc;
   };
}