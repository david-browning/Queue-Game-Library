#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_sampler_buffer.h"
#include "include/GPU/Buffers/igpu_buffer.h"

namespace qgl::content
{
   /*
    Represents a sampler. Samplers do not have a resource. Instead, they only
    have a description and can be added to a descriptor heap.
    */
   class QGL_GRAPHICS_API sampler :
      public graphics::gpu::buffers::igpu_buffer<D3D12_SAMPLER_DESC,
      nullptr_t, 
      graphics::d3d_resource>,
      public content_item
   {
      public:
      using ResourceDescriptionT = D3D12_SAMPLER_DESC;
      using ViewDescriptionT = nullptr_t;

      /*
       Constructor
       */
      sampler(const buffers::SAMPLER_BUFFER* buffer,
              const wchar_t* name,
              const content_id id);

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
      virtual const ResourceDescriptionT* description() const;

      virtual const ViewDescriptionT* view() const;

      private:
      /*
       Populated by the constructor.
       */
      ResourceDescriptionT m_samplerDesc;
   };
}