#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics
{
   struct LIB_EXPORT TEXTURE_BUFFER
   {
      public:
      TEXTURE_BUFFER(const std::vector<uint8_t>& data);

      TEXTURE_BUFFER(std::vector<uint8_t>&& data);

      TEXTURE_BUFFER(size_t texSizeBytes);

      TEXTURE_BUFFER(const TEXTURE_BUFFER& r);

      TEXTURE_BUFFER(TEXTURE_BUFFER&& r);

      ~TEXTURE_BUFFER();

      inline uint8_t* data()
      {
         return m_data.data();
      }

      inline UINT width() const
      {
         return m_width;
      }

      inline UINT& width()
      {
         return m_width;
      }

      inline UINT height() const
      {
         return m_height;
      }

      inline UINT& height()
      {
         return m_height;
      }

      inline UINT depth() const
      {
         return m_depth;
      }

      inline UINT& depth()
      {
         return m_depth;
      }

      inline UINT mip_length() const
      {
         return m_mipCount;
      }

      inline UINT16& mip_length()
      {
         return m_mipCount;
      }

      inline DXGI_FORMAT format() const
      {
         return m_format;
      }

      inline DXGI_FORMAT& format()
      {
         return m_format;
      }

      inline bool cube() const
      {
         return m_isCubeMap;
      }

      inline bool& cube()
      {
         return m_isCubeMap;
      }

      inline UINT array_size() const
      {
         return m_arraySize;
      }

      inline UINT& array_size()
      {
         return m_arraySize;
      }

      inline D3D12_RESOURCE_DIMENSION dimmension() const
      {
         return m_resourceDimension;
      }

      inline D3D12_RESOURCE_DIMENSION& dimmension()
      {
         return m_resourceDimension;
      }

      /*
       Returns the size in bytes of the texture data.
       */
      inline size_t size() const
      {
         return m_data.size();
      }

      inline std::vector<D3D12_SUBRESOURCE_DATA> subresources() const
      {
         return m_subresources;
      }

      inline std::vector<D3D12_SUBRESOURCE_DATA>& subresources()
      {
         return m_subresources;
      }

      private:
      #pragma warning(push)
      #pragma warning(disable: 4251)
      std::vector<uint8_t> m_data;
      std::vector<D3D12_SUBRESOURCE_DATA> m_subresources;
      #pragma warning(pop)

      //DDS_ALPHA_MODE AlphaMode;
      D3D12_RESOURCE_DIMENSION m_resourceDimension;
      DXGI_FORMAT m_format;
      UINT m_width;
      UINT m_height;
      UINT m_depth;
      UINT16 m_mipCount;
      UINT m_arraySize;
      bool m_isCubeMap;
   };
}