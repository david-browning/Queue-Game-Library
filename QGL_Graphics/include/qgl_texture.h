#pragma once
#include "qgl_graphics_include.h"
#include "qgl_igpu_buffer.h"
#include "qgl_texture_buffer.h"

namespace qgl::graphics
{
   class LIB_EXPORT texture :
      public low::igpu_buffer<D3D12_RESOURCE_DESC, D3D12_SHADER_RESOURCE_VIEW_DESC, d3d_resource>,
      public content::content_item
   {
      public:
      using ResourceDescriptionT = D3D12_RESOURCE_DESC;
      using ViewDescriptionT = D3D12_SHADER_RESOURCE_VIEW_DESC;

      texture(const TEXTURE_BUFFER& textureData,
              winrt::com_ptr<d3d_device>& dev_p,
              qgl::content::CONTENT_LOADER_IDS loaderID,
              const std::wstring& name,
              const content::content_id id);

      texture(TEXTURE_BUFFER&& textureData,
              winrt::com_ptr<d3d_device>& dev_p,
              qgl::content::CONTENT_LOADER_IDS loaderID,
              const std::wstring& name,
              const content::content_id id);

      texture(const texture& r) = delete;

      texture(texture&& r) = delete;

      virtual ~texture();

      #pragma region Description Getters
      virtual ResourceDescriptionT description() const
      {
         return m_textureDesc;
      }

      virtual ViewDescriptionT view() const
      {
         return m_viewDesc;
      }
      #pragma endregion

      private:
      void p_allocate();

      void p_createTexDesc();

      void p_createViewDesc();

      ResourceDescriptionT m_textureDesc;
      ViewDescriptionT m_viewDesc;

      TEXTURE_BUFFER m_data;
      size_t m_subresourcesCount;
   };
}