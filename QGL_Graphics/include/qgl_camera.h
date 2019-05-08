#pragma once
#include "qgl_graphics_include.h"
#include "qgl_camera_buffer.h"
#include "qgl_constant_buffer.h"

namespace qgl::graphics
{
   template<bool RightHandMode = true>
   class alignas(alignof(DirectX::XMMATRIX)) camera : public qgl::content::wcontent_item
   {
      public:
      camera(const CAMERA_BUFFER& cBuffer,
             float viewWidth, float viewHeight,
             const winrt::com_ptr<d3d_device>& dev_p,
             const content::wcontent_item::str_t& name,
             const content::wcontent_item::id_t& id) :
         m_cameraBuffer(dev_p),
         content_item(name, id,
                      qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_CAMERA,
                      qgl::content::CONTENT_LOADER_IDS::CONTENT_LOADER_ID_CAMERA)
      {
         position(DirectX::XMVectorSet(cBuffer.position()[0], cBuffer.position()[1],
                                       cBuffer.position()[2], cBuffer.position()[3]));

         up(DirectX::XMVectorSet(cBuffer.up()[0], cBuffer.up()[1],
                                 cBuffer.up()[2], cBuffer.up()[3]));

         look_at(DirectX::XMVectorSet(cBuffer.look_at()[0], cBuffer.look_at()[1],
                                      cBuffer.look_at()[2], cBuffer.look_at()[3]));

         make_project(cBuffer.fov(), viewWidth / viewHeight, cBuffer.near_plane(), cBuffer.far_plane());
         make_view();
      }


      camera(const camera& c) = delete;

      camera(camera&& m) = delete;

      virtual ~camera()
      {

      }

      /*
       Updates the projection matrix.
       */
      inline void make_project(float fov,
                               float aspectRatio,
                               float nearPlane = 1.0f,
                               float farPlane = 1000.0f)
      {
         if constexpr (RightHandMode)
         {
            p_updateProjectionRH(fov, aspectRatio, nearPlane, farPlane);
         }
         else
         {
            p_updateProjectionLH(fov, aspectRatio, nearPlane, farPlane);
         }
      }

      /*
       Updates the view matrix.
       */
      inline void make_view()
      {
         if constexpr (RightHandMode)
         {
            p_updateViewRH();
         }
         else
         {
            p_updateViewLH();
         }
      }

      const auto& XM_CALLCONV position() const
      {
         return m_cameraBuffer.mapping()->Position;
      }

      void XM_CALLCONV position(const DirectX::FXMVECTOR pos)
      {
         DirectX::XMStoreFloat4A(&m_cameraBuffer.mapping()->Position, pos);
      }

      void XM_CALLCONV up(const DirectX::FXMVECTOR u)
      {
         DirectX::XMStoreFloat4A(&m_cameraBuffer.mapping()->Up, u);
      }

      void XM_CALLCONV look_at(const DirectX::FXMVECTOR lookAt)
      {
         DirectX::XMStoreFloat4A(&m_cameraBuffer.mapping()->LookAt, lookAt);
      }

      inline void map()
      {
         m_cameraBuffer.map();
      }

      inline void unmap()
      {
         m_cameraBuffer.unmap();
      }

      private:
      struct alignas(alignof(DirectX::XMVECTOR)) CONST_CAMERA_BUFFER
      {
         DirectX::XMFLOAT4X4A View;
         DirectX::XMFLOAT4X4A Projection;
         DirectX::XMFLOAT4A Position;
         DirectX::XMFLOAT4A LookAt;
         DirectX::XMFLOAT4A Up;
      };

      /*
       Updates the view matrix using the position, look at, and up stored in the camera buffer.
       */
      void p_updateViewRH()
      {
         DirectX::XMStoreFloat4x4A(&m_cameraBuffer.mapping()->View,
                                   DirectX::XMMatrixLookToRH(DirectX::XMLoadFloat4A(&m_cameraBuffer.mapping()->Position),
                                                             DirectX::XMLoadFloat4A(&m_cameraBuffer.mapping()->LookAt),
                                                             DirectX::XMLoadFloat4A(&m_cameraBuffer.mapping()->Up)));
      }

      void p_updateViewLH()
      {
         DirectX::XMStoreFloat4x4A(&m_cameraBuffer.mapping()->View,
                                   DirectX::XMMatrixLookToLH(DirectX::XMLoadFloat4A(&m_cameraBuffer.mapping()->Position),
                                                             DirectX::XMLoadFloat4A(&m_cameraBuffer.mapping()->LookAt),
                                                             DirectX::XMLoadFloat4A(&m_cameraBuffer.mapping()->Up)));
      }

      /*
       Updates the projection matrix stored in the camera buffer.
       */
      void p_updateProjectionRH(float fov,
                                float aspectRatio,
                                float nearPlane,
                                float farPlane)
      {
         DirectX::XMStoreFloat4x4A(&m_cameraBuffer.mapping()->Projection,
                                   DirectX::XMMatrixPerspectiveFovRH(fov, aspectRatio,
                                                                     nearPlane, farPlane));
      }

      void p_updateProjectionLH(float fov,
                                float aspectRatio,
                                float nearPlane,
                                float farPlane)
      {
         DirectX::XMStoreFloat4x4A(&m_cameraBuffer.mapping()->Projection,
                                   DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio,
                                                                     nearPlane, farPlane));
      }

      const_buffer<CONST_CAMERA_BUFFER> m_cameraBuffer;
   };

   // Constants used to calculate screen rotations.
   namespace ScreenRotation
   {
      // 0-degree Z-rotation
      static const DirectX::XMFLOAT4X4 Rotation0(
         1.0f, 0.0f, 0.0f, 0.0f,
         0.0f, 1.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
      );

   // 90-degree Z-rotation
      static const DirectX::XMFLOAT4X4 Rotation90(
         0.0f, 1.0f, 0.0f, 0.0f,
         -1.0f, 0.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
      );

   // 180-degree Z-rotation
      static const DirectX::XMFLOAT4X4 Rotation180(
         -1.0f, 0.0f, 0.0f, 0.0f,
         0.0f, -1.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
      );

   // 270-degree Z-rotation
      static const DirectX::XMFLOAT4X4 Rotation270(
         0.0f, -1.0f, 0.0f, 0.0f,
         1.0f, 0.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
      );
   };
}