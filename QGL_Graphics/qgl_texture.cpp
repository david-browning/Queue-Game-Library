#include "pch.h"
#include "include/qgl_texture.h"
#include "include/qgl_dds.h"

qgl::graphics::texture::texture(const TEXTURE_BUFFER& textureData,
                                winrt::com_ptr<d3d_device>& dev_p,
                                qgl::content::CONTENT_LOADER_IDS loaderID,
                                const content::wcontent_item::str_t& name,
                                const content::wcontent_item::id_t& id) :
   m_textureDesc(),
   m_viewDesc(),
   m_data(textureData),
   m_subresourcesCount(0),
   igpu_buffer(dev_p),
   content_item(name, id,
                qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_TEXTURE,
                loaderID)
{
   p_allocate();
}

qgl::graphics::texture::texture(TEXTURE_BUFFER&& textureData,
                                winrt::com_ptr<d3d_device>& dev_p,
                                qgl::content::CONTENT_LOADER_IDS loaderID,
                                const content::wcontent_item::str_t& name,
                                const content::wcontent_item::id_t& id) :
   m_textureDesc(),
   m_viewDesc(),
   m_data(std::move(textureData)),
   m_subresourcesCount(0),
   igpu_buffer(dev_p),
   content_item(name, id,
                qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_TEXTURE,
                loaderID)
{
   p_allocate();
}

qgl::graphics::texture::~texture()
{
}

void qgl::graphics::texture::p_allocate()
{
   p_createTexDesc();

   //Reserve space on the GPU for the texture.
   auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
   winrt::check_hresult(m_dev_p->CreateCommittedResource(
      &heapProps,
      D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
      &m_textureDesc,
      D3D12_RESOURCE_STATE_COPY_DEST,
      nullptr,
      IID_PPV_ARGS(m_resource.put())));
   name_d3d(m_resource.get(), name().c_str());

   p_createViewDesc();
}

void qgl::graphics::texture::p_createTexDesc()
{
   //https://docs.microsoft.com/en-us/windows/desktop/api/d3d12/ns-d3d12-d3d12_resource_desc
   m_textureDesc = {};
   m_textureDesc.DepthOrArraySize = (m_data.dimmension() == D3D12_RESOURCE_DIMENSION_TEXTURE3D) ?
      static_cast<UINT16>(m_data.depth()) : static_cast<UINT16>(m_data.array_size());
   m_textureDesc.Dimension = m_data.dimmension();
   m_textureDesc.Width = m_data.width();
   m_textureDesc.Height = m_data.height();
   m_textureDesc.Format = m_data.format();
   m_textureDesc.MipLevels = m_data.mip_length();
   m_textureDesc.SampleDesc.Count = 1;
   m_textureDesc.SampleDesc.Quality = 0;
   m_textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS | D3D12_RESOURCE_FLAG_NONE;
   m_textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
}

void qgl::graphics::texture::p_createViewDesc()
{
   m_viewDesc = {};
   m_viewDesc.ViewDimension = low::srv_dim_from_tbuffer(m_data);
   m_viewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
   m_viewDesc.Format = m_data.format();

   switch (m_viewDesc.ViewDimension)
   {
      case D3D12_SRV_DIMENSION_TEXTURE1D:
      {
         m_viewDesc.Texture1D.MipLevels = static_cast<UINT>(m_data.mip_length());
         break;
      }
      case D3D12_SRV_DIMENSION_TEXTURE1DARRAY:
      {
         m_viewDesc.Texture1DArray.MipLevels = static_cast<UINT>(m_data.mip_length());
         m_viewDesc.Texture1DArray.ArraySize = static_cast<UINT>(m_data.array_size());
         break;
      }
      case D3D12_SRV_DIMENSION_TEXTURE2D:
      {
         m_viewDesc.Texture2D.MipLevels = static_cast<UINT>(m_data.mip_length());
         break;
      }
      case D3D12_SRV_DIMENSION_TEXTURE2DARRAY:
      {
         m_viewDesc.Texture2DArray.MipLevels = static_cast<UINT>(m_data.mip_length());
         m_viewDesc.Texture2DArray.ArraySize = static_cast<UINT>(m_data.array_size());
         break;
      }
      case D3D12_SRV_DIMENSION_TEXTURE3D:
      {
         m_viewDesc.Texture3D.MipLevels = static_cast<UINT>(m_data.mip_length());
         break;
      }
      case D3D12_SRV_DIMENSION_TEXTURECUBE:
      {
         m_viewDesc.TextureCube.MipLevels = static_cast<UINT>(m_data.mip_length());
         break;
      }
      case D3D12_SRV_DIMENSION_TEXTURECUBEARRAY:
      {
         assert(m_data.array_size() % 6 == 0);
         m_viewDesc.TextureCubeArray.MipLevels = static_cast<UINT>(m_data.mip_length());
         m_viewDesc.TextureCubeArray.NumCubes = static_cast<UINT>(m_data.array_size() / 6);
         break;
      }
      default:
      {
         throw std::domain_error("The SRV dimension is not recognized.");
      }
   }
}
