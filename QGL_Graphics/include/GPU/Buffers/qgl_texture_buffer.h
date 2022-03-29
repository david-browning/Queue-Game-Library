#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/qgl_igpu_buffer.h"
#include "include/GPU/Memory/qgl_igpu_allocator.h"

namespace qgl::graphics::gpu
{
   class texture_buffer : public gpu::igpu_buffer<D3D12_RESOURCE_DESC,
      D3D12_SHADER_RESOURCE_VIEW_DESC,
      gpu_resource>
   {
      public:
      using ResDescT = D3D12_RESOURCE_DESC;
      using ViewDescT = D3D12_SHADER_RESOURCE_VIEW_DESC;
      using texture_data = typename std::vector<uint8_t>;

      texture_buffer(texture_data&& data, gpu_allocator_ptr&& allocator_p);

      texture_buffer(const void* textureData, size_t textureSizeBytes,
                     gpu_allocator_ptr&& allocator_p);

      texture_buffer(const texture_buffer& r) = delete;

      texture_buffer(texture_buffer&& r) = default;

      virtual ~texture_buffer() noexcept = default;

      virtual const ResDescT* description() const;

      virtual const ViewDescT* view() const;

      virtual size_t size() const noexcept;

      virtual gpu_alloc_handle alloc_handle() const noexcept;

      virtual const gpu_resource* get() const;

      virtual gpu_resource* get();

      /*
       Returns the width (in pixels) of the texture.
       */
      size_t width() const noexcept;

      /*
       Returns the height (in pixels) of the texture.
       */
      size_t height() const noexcept;

      size_t depth() const noexcept;

      size_t array_size() const noexcept;

      /*
       Returns how many mip levels the texture has.
       */
      size_t mip_levels() const noexcept;

      DXGI_FORMAT format() const noexcept;

      D3D12_RESOURCE_DIMENSION dimension() const noexcept;

      bool cube() const noexcept;

      private:
      void make_view_desc();

      void make_resc_desc();
   };
}