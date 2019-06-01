#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_light_buffer.h"
#include "include/GPU/Buffers/qgl_imappable.h"

namespace qgl::graphics::content
{
   class alignas(alignof(DirectX::XMVECTOR)) light :
      public qgl::content::content_item
   {
      public:
      light(const content::buffers::LIGHT_BUFFER* lBuffer,
            d3d_device* dev_p,
            const wchar_t* name,
            const qgl::content::content_id id);

      light(const light& c) = delete;

      light(light&& m) = delete;

      virtual ~light();

      inline float intensity() const
      {
         return m_lightBuffer.mapping()->Intensity;
      }

      inline void intensity(float intes)
      {
         m_lightBuffer.mapping()->Intensity = intes;
      }

      const auto& XM_CALLCONV position() const
      {
         return m_lightBuffer.mapping()->Position;
      }

      void XM_CALLCONV position(const DirectX::FXMVECTOR pos)
      {
         DirectX::XMStoreFloat4(&m_lightBuffer.mapping()->Position, pos);
      }

      const auto& XM_CALLCONV look_at() const
      {
         return m_lightBuffer.mapping()->LookAt;
      }

      void XM_CALLCONV look_at(const DirectX::FXMVECTOR lookAt)
      {
         DirectX::XMStoreFloat4(&m_lightBuffer.mapping()->LookAt, lookAt);
      }

      const auto& XM_CALLCONV color() const
      {
         return m_lightBuffer.mapping()->Color;
      }

      void XM_CALLCONV color(const DirectX::FXMVECTOR colr)
      {
         DirectX::XMStoreFloat4(&m_lightBuffer.mapping()->Color, colr);
      }

      inline void map()
      {
         m_lightBuffer.map();
      }

      inline void unmap()
      {
         m_lightBuffer.unmap();
      }

      private:
      struct alignas(alignof(DirectX::XMVECTOR)) LIGHT_CONST_BUFFER
      {
         DirectX::XMFLOAT4A Position;
         DirectX::XMFLOAT4A LookAt;
         DirectX::XMFLOAT4A Color;
         float Intensity;

         uint8_t padding[64 -
            (3 * sizeof(DirectX::XMFLOAT4A)) -
            sizeof(float)];
      };

      /*
       A light is just a wrapper around a constant buffer
       */
      #pragma warning(disable:4324)
      const_buffer<LIGHT_CONST_BUFFER> m_lightBuffer;
   };
}