#pragma once
#include "include/qgl_graphics_include.h"
#include "include/qgl_graphics_device.h"
#include "include/Descriptors/qgl_texture_descriptor.h"
#include "include/GPU/Buffers/qgl_igpu_buffer.h"
#include "include/GPU/Memory/qgl_igpu_allocator.h"

namespace qgl::graphics::gpu
{
   class texture_buffer : public gpu::igpu_buffer<D3D12_RESOURCE_DESC,
                                                  D3D12_SHADER_RESOURCE_VIEW_DESC,
                                                  igpu_resource>

   {
      public:
      static constexpr auto INVALID_TEXTURE_TYPE_MSG = "Unknown texture type.";

      /*
       Load a texture from the raw buffer.
       */
      texture_buffer(const descriptors::texture_descriptor& desc,
                     const void* data,
                     size_t dataSize,
                     gpu::igpu_allocator& allocator,
                     graphics_device& dev) :
         m_allocator(allocator)
      {
         field_constructor(desc, dev);

         switch (desc.type)
         {
            case descriptors::texture_types::dds:
            {
               check_result(DirectX::LoadFromDDSMemory(
                  data, dataSize,
                  desc.image_flags.as<DirectX::DDS_FLAGS>(),
                  nullptr, m_img));
               m_clamp = static_cast<float>(desc.clamp);
               m_index = static_cast<decltype(m_index)>(desc.index);
               m_highestMip = static_cast<decltype(m_highestMip)>(
                  desc.highest_mip);
               break;
            }
            case descriptors::texture_types::hdr:
            {
               check_result(DirectX::LoadFromHDRMemory(
                  data, dataSize,
                  nullptr, m_img));
               break;
            }
            case descriptors::texture_types::wic:
            {
               // Optional WIC callback:
               //https://github.com/microsoft/DirectXTex/wiki/WIC-I-O-Functions
               check_result(DirectX::LoadFromWICMemory(
                  data, dataSize,
                  desc.image_flags.as<DirectX::WIC_FLAGS>(),
                  nullptr, m_img));
               break;
            }
            case descriptors::texture_types::tga:
            {
               check_result(DirectX::LoadFromTGAMemory(
                  data, dataSize,
                  nullptr, m_img));
               break;
            }
            default:
            {
               throw std::domain_error(INVALID_TEXTURE_TYPE_MSG);
            }
         }

         remake();
      }

      /*
       Load a texture from a file.
       */
      texture_buffer(const descriptors::texture_descriptor& desc,
                     const qgl::sys_str& filePath,
                     gpu::igpu_allocator& allocator,
                     graphics_device& dev) :
         texture_buffer(desc, filePath.c_str(), allocator, dev)
      {
         // Call the character pointer constructor.
      }

      /*
       Load a texture from a file.
       */
      texture_buffer(const descriptors::texture_descriptor& desc,
                     const sys_char* const filePath,
                     gpu::igpu_allocator& allocator,
                     graphics_device& dev) :
         m_allocator(allocator)
      {
         field_constructor(desc, dev);

         switch (desc.type)
         {
            case descriptors::texture_types::dds:
            {
               check_result(DirectX::LoadFromDDSFile(
                  filePath,
                  desc.image_flags.as<DirectX::DDS_FLAGS>(),
                  nullptr, m_img));
               m_clamp = static_cast<float>(desc.clamp);
               m_index = static_cast<decltype(m_index)>(desc.index);
               m_highestMip = static_cast<decltype(m_highestMip)>(
                  desc.highest_mip);
               break;
            }
            case descriptors::texture_types::hdr:
            {
               check_result(DirectX::LoadFromHDRFile(
                  filePath,
                  nullptr, m_img));
               break;
            }
            case descriptors::texture_types::wic:
            {
               // Optional WIC callback:
               //https://github.com/microsoft/DirectXTex/wiki/WIC-I-O-Functions
               check_result(DirectX::LoadFromWICFile(
                  filePath,
                  desc.image_flags.as<DirectX::WIC_FLAGS>(),
                  nullptr, m_img));
               break;
            }
            case descriptors::texture_types::tga:
            {
               check_result(DirectX::LoadFromTGAFile(
                  filePath,
                  nullptr, m_img));
               break;
            }
            default:
            {
               throw std::domain_error(INVALID_TEXTURE_TYPE_MSG);
            }
         }

         remake();
      }

      texture_buffer(const texture_buffer&) = delete;

      texture_buffer(texture_buffer&&) noexcept = default;

      virtual ~texture_buffer() noexcept
      {
         free_resource();
      }

      virtual const D3D12_RESOURCE_DESC* description() const
      {
         return &m_desc;
      }

      virtual const D3D12_SHADER_RESOURCE_VIEW_DESC* view() const
      {
         return &m_view;
      }

      /*
       Return the size of the pixel data in bytes.
       This includes all alignment padding as well.
       */
      virtual size_t size() const noexcept
      {
         return m_img.GetPixelsSize();
      }

      virtual gpu::gpu_alloc_handle alloc_handle() const
      {
         return m_allocHandle;
      }

      /*
       Returns a pointer to the texture resource.
       */
      virtual const igpu_resource* get() const
      {
         return m_allocator.get().get(m_allocHandle);
      }

      /*
       Returns a pointer to the texture resource.
       */
      virtual igpu_resource* get()
      {
         return m_allocator.get().get(m_allocHandle);
      }

      /*
       Recreates the texture resource using the cached allocator. The texture
       resource will need to be uploaded to the GPU.
       */
      void remake()
      {
         free_resource();
         make_resource();
      }

      /*
       Frees the old texture resources and remakes it using the passed
       allocator. The texture resource will need to be uploaded to the GPU.
       */
      void remake(gpu::igpu_allocator& allocator)
      {
         free_resource();
         m_allocator = allocator;
         make_resource();
      }

      /*
       Number of pixels wide the texture is.
       */
      size_t width() const noexcept
      {
         return m_img.GetMetadata().width;
      }

      /*
       Number of pixels tall the texture is. Should be 1 for 1D textures.
       */
      size_t height() const noexcept
      {
         return m_img.GetMetadata().height;
      }

      /*
       Texture depth. Should be 1 for 1D or 2D textures.
       */
      size_t depth() const noexcept
      {
         return m_img.GetMetadata().depth;
      }

      /*
       Number of mip levels in the texture.
       */
      size_t mips() const noexcept
      {
         return m_img.GetMetadata().mipLevels;
      }

      /*
       This is a multiple of 6 for cube maps.
       */
      size_t array_size() const noexcept
      {
         return m_img.GetMetadata().arraySize;
      }

      /*
       Texture format.
       */
      DXGI_FORMAT format() const noexcept
      {
         return m_actualSurface;
      }

      /*
       Returns true if this is a cube map.
       */
      bool cubemap() const noexcept
      {
         return m_img.GetMetadata().IsCubemap();
      }

      /*
       True if the texture is 3 dimensional.
       */
      bool volumemap() const noexcept
      {
         return m_img.GetMetadata().IsVolumemap();
      }

      /*
       Returns true if the alpha mode is pre-multiplied
       */
      bool premul_alpha() const noexcept
      {
         return m_img.GetMetadata().IsPMAlpha();
      }

      /*
       Returns a pointer to the pixel data.
       */
      const uint8_t* data() const noexcept
      {
         return m_img.GetPixels();
      }

      /*
       Returns a pointer to the pixel data.
       */
      uint8_t* data() noexcept
      {
         return m_img.GetPixels();
      }

      /*
       The index of the first texture to use in an array of textures.
       Only valid for DDS textures. For other texture types, this is 0.
       */
      size_t index() const noexcept
      {
         return m_index;
      }

      /*
       Index of the most detailed mipmap level to use.
       Only valid for DDS textures. For other texture types, this is 0.
       */
      size_t highest_mip() const noexcept
      {
         return m_highestMip;
      }

      /*
       A value to clamp sample LOD values to. For example, if you specify 2.0f
       for the clamp value, you ensure that no individual sample accesses a
       mip level less than 2.0f.
       Only valid for DDS textures. FOr other texture types, this is 0.0
       */
      float clamp() const noexcept
      {
         return m_clamp;
      }

      /*
       Texture dimension.
       */
      DirectX::TEX_DIMENSION dimension() const noexcept
      {
         return m_img.GetMetadata().dimension;
      }

      /*
       How to sample the texture.
       */
      const D3D12_SAMPLER_DESC& sampler() const noexcept
      {
         return m_sampler;
      }

      private:
      /*
       Copies fields from the texture descriptors that need to be cached.
       */
      void field_constructor(const descriptors::texture_descriptor& desc,
                             graphics_device& dev)
      {
         // Check that the device supports the texture.
         if (!DirectX::IsSupportedTexture(dev.dev_3d(), m_img.GetMetadata()))
         {
            throw std::domain_error{
               "The texture is not supported by the 3D device." };
         }

         m_clamp = static_cast<float>(desc.clamp);
         m_highestMip = desc.highest_mip;
         m_index = desc.index;
         m_resourceFlags = static_cast<D3D12_RESOURCE_FLAGS>(
            desc.resource_flags);;
         m_loadFlags = static_cast<DirectX::CREATETEX_FLAGS>(desc.load_flags);
         m_sampler = static_cast<D3D12_SAMPLER_DESC>(desc.sampler);
      }

      void make_resource()
      {
         auto& meta = m_img.GetMetadata();

         if (!meta.mipLevels || !meta.arraySize)
         {
            throw std::domain_error("mip levels or array size are 0.");
         }

         if (width() > UINT32_MAX || height() > UINT32_MAX ||
             mips() > UINT16_MAX || array_size() > UINT16_MAX)
         {
            throw std::domain_error(
               "width, height, mips, or array size are too large.");
         }

         m_actualSurface = meta.format;
         if (m_loadFlags & DirectX::CREATETEX_FLAGS::CREATETEX_FORCE_SRGB)
         {
            m_actualSurface = DirectX::MakeSRGB(m_actualSurface);
         }
         else if (m_loadFlags & DirectX::CREATETEX_FLAGS::CREATETEX_IGNORE_SRGB)
         {
            m_actualSurface = DirectX::MakeLinear(m_actualSurface);
         }

         D3D12_RESOURCE_DESC rescDesc = {};
         rescDesc.Width = static_cast<UINT>(width());
         rescDesc.Height = static_cast<UINT>(height());
         rescDesc.MipLevels = static_cast<UINT16>(mips());
         rescDesc.DepthOrArraySize =
            dimension() & DirectX::TEX_DIMENSION_TEXTURE3D ?
            static_cast<UINT16>(depth()) :
            static_cast<UINT16>(array_size());
         rescDesc.Format = format();
         rescDesc.Flags = m_resourceFlags;
         rescDesc.SampleDesc.Count = 1;
         rescDesc.SampleDesc.Quality = 0;
         rescDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(dimension());
         rescDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

         m_allocHandle = m_allocator.get().alloc(rescDesc);

         make_resc_desc();

         // Finalize making the view description.
         m_view.Format = format();
         m_view.Shader4ComponentMapping =
            D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
         m_view.ViewDimension = srv_dim();
         if (cubemap())
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
         if (cubemap())
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
               m_view.Texture1D.MipLevels = static_cast<UINT>(mips());
               m_view.Texture1D.MostDetailedMip =
                  static_cast<UINT>(m_highestMip);
               m_view.Texture1D.ResourceMinLODClamp = m_clamp;
               break;
            }
            case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE2D:
            {
               m_view.Texture2D.MipLevels = static_cast<UINT>(mips());
               m_view.Texture2D.MostDetailedMip =
                  static_cast<UINT>(m_highestMip);
               m_view.Texture2D.ResourceMinLODClamp = m_clamp;
               break;
            }
            case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE3D:
            {
               m_view.Texture3D.MipLevels = static_cast<UINT>(mips());
               m_view.Texture3D.MostDetailedMip =
                  static_cast<UINT>(m_highestMip);

               // TODO: What is this? Make it configurable?
               m_view.Texture2D.PlaneSlice = 0;
               m_view.Texture3D.ResourceMinLODClamp = m_clamp;
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
               m_view.Texture1DArray.ArraySize =
                  static_cast<UINT>(array_size());
               m_view.Texture1DArray.MipLevels = static_cast<UINT>(mips());
               m_view.Texture1DArray.FirstArraySlice =
                  static_cast<UINT>(m_index);
               m_view.Texture1DArray.MostDetailedMip =
                  static_cast<UINT>(m_highestMip);
               m_view.Texture1DArray.ResourceMinLODClamp = m_clamp;
               break;
            }
            case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE2D:
            {
               m_view.Texture2DArray.ArraySize =
                  static_cast<UINT>(array_size());
               m_view.Texture2DArray.FirstArraySlice =
                  static_cast<UINT>(m_index);
               m_view.Texture2DArray.MipLevels = static_cast<UINT>(mips());
               m_view.Texture2DArray.MostDetailedMip =
                  static_cast<UINT>(m_highestMip);

               // TODO: What is this? Make it configurable?
               m_view.Texture2DArray.PlaneSlice = 0;
               m_view.Texture2DArray.ResourceMinLODClamp = m_clamp;
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
            m_view.TextureCubeArray.First2DArrayFace =
               static_cast<UINT>(m_index);
            m_view.TextureCubeArray.MipLevels = static_cast<UINT>(mips());
            m_view.TextureCubeArray.MostDetailedMip =
               static_cast<UINT>(m_highestMip);
            m_view.TextureCubeArray.NumCubes =
               static_cast<UINT>(array_size()) / 6;
            m_view.TextureCubeArray.ResourceMinLODClamp = m_clamp;
         }
         else
         {
            // Just a single cube map.
            m_view.TextureCube.MipLevels = static_cast<UINT>(mips());
            m_view.TextureCube.MostDetailedMip =
               static_cast<UINT>(m_highestMip);
            m_view.TextureCube.ResourceMinLODClamp = m_clamp;
         }
      }

      void make_resc_desc()
      {
         switch (dimension())
         {
            case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE1D:
            {
               m_desc = CD3DX12_RESOURCE_DESC::Tex1D(
                  format(),
                  width(),
                  static_cast<UINT16>(array_size()),
                  static_cast<UINT16>(mips()),
                  D3D12_RESOURCE_FLAG_NONE,
                  D3D12_TEXTURE_LAYOUT_UNKNOWN,
                  0);
               break;
            }
            case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE2D:
            {
               m_desc = CD3DX12_RESOURCE_DESC::Tex2D(
                  format(),
                  width(),
                  static_cast<UINT>(height()),
                  static_cast<UINT16>(array_size()),
                  static_cast<UINT16>(mips()),
                  1, 0,
                  D3D12_RESOURCE_FLAG_NONE,
                  D3D12_TEXTURE_LAYOUT_UNKNOWN,
                  0);
               break;
            }
            case DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE3D:
            {
               m_desc = CD3DX12_RESOURCE_DESC::Tex3D(
                  format(),
                  width(),
                  static_cast<UINT>(height()),
                  static_cast<UINT16>(depth()),
                  static_cast<UINT16>(mips()),
                  D3D12_RESOURCE_FLAG_NONE,
                  D3D12_TEXTURE_LAYOUT_UNKNOWN,
                  0);
               break;
            }
            default:
            {
               throw std::out_of_range{ "Unsupported texture dimension." };
            }
         }
      }

      void free_resource() noexcept
      {
         m_allocator.get().free(m_allocHandle);
      }

      gpu::gpu_alloc_handle m_allocHandle;
      std::reference_wrapper<gpu::igpu_allocator> m_allocator;

      DirectX::CREATETEX_FLAGS m_loadFlags;
      D3D12_RESOURCE_FLAGS m_resourceFlags;

      size_t m_highestMip = 0;
      size_t m_index = 0;
      float m_clamp = 0.0f;

      D3D12_RESOURCE_DESC m_desc;
      D3D12_SHADER_RESOURCE_VIEW_DESC m_view;

      /*
       It may be possible to change the texture format.
       This overrides the format specified in the DDS header.
       */
      DXGI_FORMAT m_actualSurface;

      /*
       How to sample the texture.
       */
      D3D12_SAMPLER_DESC m_sampler;

      /*
       The actual texture and its metadata.
       */
      DirectX::ScratchImage m_img;
   };
}