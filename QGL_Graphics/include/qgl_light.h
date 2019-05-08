#pragma once
#include "qgl_graphics_include.h"
#include "qgl_constant_buffer.h"
#include "qgl_light_buffer.h"

namespace qgl::graphics
{
   class alignas(alignof(DirectX::XMVECTOR)) light : public qgl::content::wcontent_item
   {
      public:
      light(const LIGHT_BUFFER& lBuffer,
            winrt::com_ptr<d3d_device>& dev_p,
            const content::wcontent_item::str_t& name,
            const content::wcontent_item::id_t& id);

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