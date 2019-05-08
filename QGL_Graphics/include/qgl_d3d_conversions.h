#pragma once
#include "qgl_graphics_include.h"
#include "qgl_igpu_buffer.h"

namespace qgl::graphics::convert
{
   constexpr qgl::graphics::low::RESOURCE_TYPES range_to_resource_type(
      D3D12_DESCRIPTOR_RANGE_TYPE t)
   {
      switch (t)
      {
         case D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV :
         {
            return low::RESOURCE_TYPES::RESOURCE_TYPE_CONST_BUFFER;
         }
         case D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER :
         {
            return low::RESOURCE_TYPES::RESOURCE_TYPE_SAMPLER;
         }
         case D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV :
         {
            return low::RESOURCE_TYPES::RESOURCE_TYPE_TEXTURE;
         }
         default :
         {

         }
      }
   }
}