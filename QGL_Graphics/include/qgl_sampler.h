#pragma once
#include "qgl_graphics_include.h"
#include "qgl_igpu_buffer.h"
#include "qgl_sampler_buffer.h"
#include "qgl_ibindable.h"
#include <QGLContent.h>

namespace qgl::graphics
{
   /*
    Represents a sampler. Samplers do not have a resource. Instead, they only have a description 
    and can be added to a descriptor heap.
    */
   class LIB_EXPORT sampler : 
      public low::igpu_buffer<D3D12_SAMPLER_DESC, nullptr_t, d3d_resource>, 
      public content::wcontent_item
   {
      public:
      using ResourceDescriptionT = D3D12_SAMPLER_DESC;
      using ViewDescriptionT = nullptr_t;
      
      sampler(const SAMPLER_BUFFER& buffer,
              winrt::com_ptr<d3d_device>& dev_p,
              const content::wcontent_item::str_t& name, 
              const content::wcontent_item::id_t& id);

      sampler(const sampler& r) = delete;

      sampler(sampler&& r) = delete;

      virtual ~sampler() = default;

      virtual ResourceDescriptionT description() const
      {
         return m_samplerDesc;
      }

      virtual ViewDescriptionT view() const
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