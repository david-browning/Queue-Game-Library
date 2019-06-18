#include "pch.h"
#include "include/Content/qgl_light.h"

namespace qgl::content
{
   light::light(const content::buffers::LIGHT_BUFFER* lBuffer,
                qgl::graphics::d3d_device* dev_p,
                const wchar_t* name,
                const qgl::content::content_id id) :
      const_buffer(dev_p),
      content_item(name, id,
                   qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_LIGHT,
                   qgl::content::CONTENT_LOADER_IDS::CONTENT_LOADER_ID_LIGHT)
   {
      intensity(lBuffer->Intensity);

      color(DirectX::XMVectorSet(lBuffer->Color[0],
                                 lBuffer->Color[1],
                                 lBuffer->Color[2],
                                 lBuffer->Color[3]));

      look_at(DirectX::XMVectorSet(lBuffer->LookAt[0],
                                   lBuffer->LookAt[1],
                                   lBuffer->LookAt[2],
                                   lBuffer->LookAt[3]));

      position(DirectX::XMVectorSet(lBuffer->Position[0],
                                    lBuffer->Position[1],
                                    lBuffer->Position[2],
                                    lBuffer->Position[3]));
   }

   float light::intensity() const
   {
      #ifdef DEBUG
      assert(mapped());
      #endif
      return mapping()->Intensity;
   }

   void light::intensity(float intes)
   {
      #ifdef DEBUG
      assert(mapped());
      assert(intes >= 0.0f && intes <= 1.0f);
      #endif
      mapping()->Intensity = intes;
   }

   DirectX::XMVECTOR XM_CALLCONV light::position() const
   {
      #ifdef DEBUG
      assert(mapped());
      #endif
      return mapping()->Position;
   }

   void XM_CALLCONV light::position(const DirectX::FXMVECTOR pos)
   {
      #ifdef DEBUG
      assert(mapped());
      #endif
      mapping()->Position = pos;
   }

   DirectX::XMVECTOR XM_CALLCONV light::look_at() const
   {
      #ifdef DEBUG
      assert(mapped());
      #endif
      return mapping()->LookAt;
   }

   void XM_CALLCONV light::look_at(const DirectX::FXMVECTOR lookAt)
   {
      #ifdef DEBUG
      assert(mapped());
      #endif
      mapping()->LookAt = lookAt;
   }

   DirectX::XMVECTOR XM_CALLCONV light::color() const
   {
      #ifdef DEBUG
      assert(mapped());
      #endif
      return mapping()->Color;
   }

   void XM_CALLCONV light::color(const DirectX::FXMVECTOR colr)
   {
      #ifdef DEBUG
      assert(mapped());
      assert(DirectX::XMVector4LessOrEqual(colr, DirectX::XMVectorSplatOne()));
      assert(DirectX::XMVector4GreaterOrEqual(colr, DirectX::XMVectorZero()));
      #endif
      mapping()->Color = colr;
   }
}

