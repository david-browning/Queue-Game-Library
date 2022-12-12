#include "pch.h"
#include "include/Shaders/qgl_shader_descriptor.h"

namespace qgl::graphics::shaders
{
   shader_types QGL_CC to_shader_type(
      D3D12_SHADER_VERSION_TYPE t) noexcept
   {
      switch (t)
      {
         case D3D12_SHADER_VERSION_TYPE::D3D12_SHVER_PIXEL_SHADER:
         {
            return shader_types::ps;
         }
         case D3D12_SHADER_VERSION_TYPE::D3D12_SHVER_VERTEX_SHADER:
         {
            return shader_types::vs;
         }
         case D3D12_SHADER_VERSION_TYPE::D3D12_SHVER_GEOMETRY_SHADER:
         {
            return shader_types::gs;
         }
         case D3D12_SHADER_VERSION_TYPE::D3D12_SHVER_HULL_SHADER:
         {
            return shader_types::hs;
         }
         case D3D12_SHADER_VERSION_TYPE::D3D12_SHVER_DOMAIN_SHADER:
         {
            return shader_types::ds;
         }
         case D3D12_SHADER_VERSION_TYPE::D3D12_SHVER_COMPUTE_SHADER:
         {
            return shader_types::cs;
         }
         default:
         {
            return shader_types::unknown;
         }
      }
   }
}