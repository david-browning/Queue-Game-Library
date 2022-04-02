#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/qgl_igpu_buffer.h"
#include "include/GPU/Memory/qgl_igpu_allocator.h"
#include "include/Content/Content-Descriptors/texture_descriptor.h"

namespace qgl::graphics::gpu
{
   class texture_buffer : public gpu::igpu_buffer<D3D12_RESOURCE_DESC,
      D3D12_SHADER_RESOURCE_VIEW_DESC,
      igpu_resource>
   {
      public:
      using ResDescT = D3D12_RESOURCE_DESC;
      using ViewDescT = D3D12_SHADER_RESOURCE_VIEW_DESC;
      using texture_data = typename std::vector<uint8_t>;

      texture_buffer(texture_data&& data,
                     gpu_allocator_ptr&& allocator_sp,
                     descriptors::texture_descriptor&& config,
                     i3d_device* dev_p) :
         m_allocator_sp(std::forward<gpu_allocator_ptr>(allocator_sp)),
         m_data(std::forward<texture_data>(data)),
         m_config(std::forward<descriptors::texture_descriptor>(config))
      {
         construct(dev_p);
      }

      /*
       Copies the texture data to an internal buffer. After construction, it
       can be freed.
       */
      texture_buffer(const std::byte* textureData, size_t textureSizeBytes,
                     gpu_allocator_ptr&& allocator_p,
                     descriptors::texture_descriptor&& config, i3d_device* dev_p) :
         m_allocator_sp(std::forward<gpu_allocator_ptr>(allocator_p)),
         m_data(textureSizeBytes),
         m_config(std::forward<descriptors::texture_descriptor>(config))
      {
         std::memcpy(m_data.data(), textureData, textureSizeBytes);
         construct(dev_p);
      }

      texture_buffer(const texture_buffer& r) = delete;

      texture_buffer(texture_buffer&& r) = default;

      virtual ~texture_buffer() noexcept
      {
         m_allocator_sp->free(m_alloc_h);
      }

      virtual const ResDescT* description() const
      {
         return &m_desc;
      }

      virtual const ViewDescT* view() const
      {
         return &m_view;
      }

      virtual size_t size() const noexcept
      {
         return width() * height() * depth() * mip_levels() * m_pixelSize;
      }

      virtual gpu_alloc_handle alloc_handle() const noexcept
      {
         return m_alloc_h;
      }

      virtual const igpu_resource* get() const
      {
         return m_allocator_sp->resource(m_alloc_h);
      }

      virtual igpu_resource* get()
      {
         return m_allocator_sp->resource(m_alloc_h);
      }

      /*
       Returns the width (in physical pixels) of the texture.
       */
      physp_t width() const noexcept
      {
         return m_meta.width;
      }

      /*
       Returns the height (in physical pixels) of the texture.
       */
      physp_t height() const noexcept
      {
         return m_meta.height;
      }

      size_t depth() const noexcept
      {
         return m_meta.depth;
      }

      size_t array_size() const noexcept
      {
         return m_meta.arraySize;
      }

      /*
       Returns how many mip levels the texture has.
       */
      size_t mip_levels() const noexcept
      {
         return m_meta.mipLevels;
      }

      DXGI_FORMAT format() const noexcept
      {
         return m_meta.format;
      }

      DirectX::TEX_DIMENSION dimension() const noexcept
      {
         return m_meta.dimension;
      }

      bool cube() const
      {
         return m_meta.IsCubemap();
      }

      private:
      void construct(i3d_device* dev_p)
      {
         // Get meta data from the DDS buffer
         winrt::check_hresult(DirectX::GetMetadataFromDDSMemory(
            m_data.data(), m_data.size(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE,
            m_meta));

         // Check that the device supports the texture.
         if (!DirectX::IsSupportedTexture(dev_p, m_meta))
         {
            throw std::domain_error{
               "The texture is not supported by the 3D device." };
         }

         // Cache the size of a pixel.
         m_pixelSize = helpers::format_size(format());

         make_resc_desc();

         m_alloc_h = m_allocator_sp->alloc(
            size(), m_desc, D3D12_RESOURCE_STATE_COMMON);

         m_view.Format = format();
         m_view.Shader4ComponentMapping =
            D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
         m_view.ViewDimension = srv_dim();

         // Finalize making the view description.

         if (cube())
         {
            finalize_view_desc_cube();
         }
         else if (array_size() > 1)
         {
            // Not a cube map but it is a texture array.
            finalize_view_desc_array();
         }
         else
         {
            // Just a texture.
            finalize_view_desc();
         }
      }

      D3D12_SRV_DIMENSION srv_dim() const
      {
         if (cube())
         {
            if (array_size() > 6)
            {
               return D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
            }
            else
            {
               return D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURECUBE;
            }
         }
         else if (array_size() > 1)
         {
            switch (dimension())
            {
               case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE1D:
               {
                  return D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
               }
               case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE2D:
               {
                  return D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
               }
               default:
               {
                  throw std::out_of_range{
                     "Unsupported array of textures dimension." };
               }
            }
         }
         else
         {
            switch (dimension())
            {
               case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE1D:
               {
                  return D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE1D;
               }
               case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE2D:
               {
                  return D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
               }
               case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE3D:
               {
                  return D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE3D;
               }
               default:
               {
                  throw std::out_of_range{ "Unsupported texture dimension." };
               }
            }
         }
      }

      void finalize_view_desc()
      {
         switch (dimension())
         {
            case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE1D:
            {
               m_view.Texture1D.MipLevels = mip_levels();
               m_view.Texture1D.MostDetailedMip = m_config.highest_mip;
               m_view.Texture1D.ResourceMinLODClamp =
                  m_config.clamp.operator float();
               break;
            }
            case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE2D:
            {
               m_view.Texture2D.MipLevels = mip_levels();
               m_view.Texture2D.MostDetailedMip = m_config.highest_mip;
               m_view.Texture2D.ResourceMinLODClamp =
                  m_config.clamp.operator float();
               break;
            }
            case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE3D:
            {
               m_view.Texture3D.MipLevels = mip_levels();
               m_view.Texture3D.MostDetailedMip = m_config.highest_mip;

               // TODO: What is this? Make it configurable?
               m_view.Texture2D.PlaneSlice = 0;
               m_view.Texture3D.ResourceMinLODClamp =
                  m_config.clamp.operator float();
               break;
            }
            default:
            {
               throw std::out_of_range{ "Unsupported texture dimension." };
            }
         }
      }

      void finalize_view_desc_array()
      {
         switch (dimension())
         {
            case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE1D:
            {
               m_view.Texture1DArray.ArraySize = array_size();
               m_view.Texture1DArray.MipLevels = mip_levels();
               m_view.Texture1DArray.FirstArraySlice = m_config.index;
               m_view.Texture1DArray.MostDetailedMip = m_config.highest_mip;
               m_view.Texture1DArray.ResourceMinLODClamp =
                  m_config.clamp.operator float();
               break;
            }
            case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE2D:
            {
               m_view.Texture2DArray.ArraySize = array_size();
               m_view.Texture2DArray.FirstArraySlice = m_config.index;
               m_view.Texture2DArray.MipLevels = mip_levels();
               m_view.Texture2DArray.MostDetailedMip = m_config.highest_mip;

               // TODO: What is this? Make it configurable?
               m_view.Texture2DArray.PlaneSlice = 0;
               m_view.Texture2DArray.ResourceMinLODClamp =
                  m_config.clamp.operator float();
               break;
            }
            default:
            {
               throw std::out_of_range{ "Unsupported texture dimension." };
            }
         }
      }

      void finalize_view_desc_cube()
      {
         // If its an array of cube maps:
         if (array_size() > 6)
         {
            m_view.TextureCubeArray.First2DArrayFace = m_config.index;
            m_view.TextureCubeArray.MipLevels = mip_levels();
            m_view.TextureCubeArray.MostDetailedMip = m_config.highest_mip;
            m_view.TextureCubeArray.NumCubes = array_size() / 6;
            m_view.TextureCubeArray.ResourceMinLODClamp =
               m_config.clamp.operator float();
         }
         else
         {
            // Just a single cube map.
            m_view.TextureCube.MipLevels = mip_levels();
            m_view.TextureCube.MostDetailedMip = m_config.highest_mip;
            m_view.TextureCube.ResourceMinLODClamp =
               m_config.clamp.operator float();
         }
      }

      void make_resc_desc()
      {
         switch (dimension())
         {
            case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE1D:
            {
               m_desc = CD3DX12_RESOURCE_DESC::Tex1D(
               format(), width(), array_size(), mip_levels(),
                  D3D12_RESOURCE_FLAG_NONE, D3D12_TEXTURE_LAYOUT_UNKNOWN, 0);
               break;
            }
            case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE2D:
            {
               m_desc = CD3DX12_RESOURCE_DESC::Tex2D(
                  format(), width(), height(), array_size(), mip_levels(),
                  1, 0,
                  D3D12_RESOURCE_FLAG_NONE, D3D12_TEXTURE_LAYOUT_UNKNOWN, 0);
               break;
            }
            case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE3D:
            {
               m_desc = CD3DX12_RESOURCE_DESC::Tex3D(
                  format(), width(), height(), depth(), mip_levels(),
                  D3D12_RESOURCE_FLAG_NONE, D3D12_TEXTURE_LAYOUT_UNKNOWN, 0);
               break;
            }
            default:
            {
               throw std::out_of_range{ "Unsupported texture dimension." };
            }
         }
      }

      descriptors::texture_descriptor m_config;
      texture_data m_data;
      gpu_allocator_ptr m_allocator_sp;
      DirectX::TexMetadata m_meta;
      size_t m_pixelSize;

      ResDescT m_desc;
      gpu_alloc_handle m_alloc_h;
      ViewDescT m_view;
   };
}