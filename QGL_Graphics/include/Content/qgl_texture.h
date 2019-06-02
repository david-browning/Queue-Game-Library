#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/igpu_buffer.h"

namespace qgl::content
{
   class QGL_GRAPHICS_API texture :
      public graphics::gpu::buffers::igpu_buffer<D3D12_RESOURCE_DESC,
      D3D12_SHADER_RESOURCE_VIEW_DESC,
      graphics::d3d_resource>,
      public content_item
   {
      public:
      using ResourceDescriptionT = D3D12_RESOURCE_DESC;
      using ViewDescriptionT = D3D12_SHADER_RESOURCE_VIEW_DESC;

      texture(const void* textureData,
              size_t textureSizeBytes,
              graphics::d3d_device* dev_p,
              CONTENT_LOADER_IDS loaderID,
              const wchar_t* name,
              const content_id id);

      texture(const texture& r);

      texture(texture&& r);

      virtual ~texture();

      virtual const ResourceDescriptionT* description() const;

      virtual const ViewDescriptionT* view() const;

      size_t width() const noexcept;

      size_t height() const noexcept;

      size_t depth() const noexcept;

      size_t array_size() const noexcept;

      size_t mip_levels() const noexcept;

      DXGI_FORMAT format() const noexcept;

      D3D12_RESOURCE_DIMENSION dimension() const noexcept;

      bool cube() const noexcept;

      private:
      struct impl;
      impl* m_impl_p;

      ResourceDescriptionT m_textureDesc;
      ViewDescriptionT m_viewDesc;

      size_t m_subresourcesCount;
   };
}