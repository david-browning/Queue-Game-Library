#include "pch.h"
#include "include/Content/qgl_camera.h"

namespace qgl::content
{
   camera::camera(const content::buffers::CAMERA_BUFFER* cBuffer,
                  float aspectRatio,
                  graphics::d3d_device* dev_p,
                  const wchar_t* name,
                  const qgl::content::content_id id) :
      const_buffer(dev_p),
      content_item(name, id,
                   qgl::content::RESOURCE_TYPE_CAMERA,
                   qgl::content::CONTENT_LOADER_ID_CAMERA)
   {
      position(DirectX::XMVectorSet(static_cast<float>(cBuffer->Position[0]),
                                    static_cast<float>(cBuffer->Position[1]),
                                    static_cast<float>(cBuffer->Position[2]),
                                    static_cast<float>(cBuffer->Position[3])));

      up(DirectX::XMVectorSet(static_cast<float>(cBuffer->Up[0]),
                              static_cast<float>(cBuffer->Up[1]),
                              static_cast<float>(cBuffer->Up[2]),
                              static_cast<float>(cBuffer->Up[3])));

      look_at(DirectX::XMVectorSet(static_cast<float>(cBuffer->LookAt[0]),
                                   static_cast<float>(cBuffer->LookAt[1]),
                                   static_cast<float>(cBuffer->LookAt[2]),
                                   static_cast<float>(cBuffer->LookAt[3])));

      make_projection(static_cast<float>(cBuffer->FOV),
                      aspectRatio,
                      static_cast<float>(cBuffer->NearPlane),
                      static_cast<float>((cBuffer->FarPlane)));
      make_view();
   }

   void camera::make_projection(float fov, 
                                float aspectRatio, 
                                float nearPlane, 
                                float farPlane)
   {
      #ifdef DEBUG
      assert(mapped());
      #endif
      mapping()->Projection = DirectX::XMMatrixPerspectiveFovLH(fov,
                                                                aspectRatio,
                                                                nearPlane,
                                                                farPlane);
   }

   void camera::make_view()
   {
      #ifdef DEBUG
      assert(mapped());
      #endif
      mapping()->View = DirectX::XMMatrixLookToLH(mapping()->Position,
                                                  mapping()->LookAt,
                                                  mapping()->Up);
   }

   DirectX::XMVECTOR XM_CALLCONV camera::position() const
   {
      #ifdef DEBUG
      assert(mapped());
      #endif
      return mapping()->Position;
   }

   void XM_CALLCONV camera::position(const DirectX::FXMVECTOR pos)
   {
      #ifdef DEBUG
      assert(mapped());
      #endif
      mapping()->Position = pos;
   }

   void XM_CALLCONV camera::up(const DirectX::FXMVECTOR u)
   {
      #ifdef DEBUG
      assert(mapped());
      #endif
      mapping()->Up = u;
   }
   void XM_CALLCONV camera::look_at(const DirectX::FXMVECTOR lookAt)
   {
      #ifdef DEBUG
      assert(mapped());
      #endif
      mapping()->LookAt = lookAt;
   }
}