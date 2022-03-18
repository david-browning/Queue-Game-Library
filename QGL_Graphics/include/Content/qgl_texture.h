#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/igpu_buffer.h"

namespace qgl::graphics
{
   class texture : public gpu::igpu_buffer<
      D3D12_RESOURCE_DESC,
      D3D12_SHADER_RESOURCE_VIEW_DESC,
      d3d_resource>
   {
      public:
      using ResourceDescriptionT = D3D12_RESOURCE_DESC;
      using ViewDescriptionT = D3D12_SHADER_RESOURCE_VIEW_DESC;

      texture(const void* textureData,
              size_t textureSizeBytes,
              const winrt::com_ptr<d3d_device>& dev_p);

      texture(const texture& r) = default;

      texture(texture&& r) = default;

      virtual ~texture() noexcept = default;

      virtual const ResourceDescriptionT* description() const
      {
         return &m_textureDesc;
      }

      virtual const ViewDescriptionT* view() const
      {
         return &m_viewDesc;
      }

      size_t width() const noexcept;

      size_t height() const noexcept;

      size_t depth() const noexcept;

      size_t array_size() const noexcept;

      size_t mip_levels() const noexcept;

      DXGI_FORMAT format() const noexcept;

      D3D12_RESOURCE_DIMENSION dimension() const noexcept;

      bool cube() const noexcept;

      private:
      ResourceDescriptionT m_textureDesc;
      ViewDescriptionT m_viewDesc;

      size_t m_subresourcesCount;
   };
}