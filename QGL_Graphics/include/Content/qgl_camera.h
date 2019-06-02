#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_camera_buffer.h"
#include "include/GPU/Buffers/qgl_const_buffer.h"

namespace qgl::content
{
   struct alignas(alignof(DirectX::XMVECTOR)) CONST_CAMERA_BUFFER
   {
      DirectX::XMMATRIX View;
      DirectX::XMMATRIX Projection;
      DirectX::XMVECTOR Position;
      DirectX::XMVECTOR LookAt;
      DirectX::XMVECTOR Up;
   };

   template<bool RightHandMode = true>
   class alignas(alignof(DirectX::XMMATRIX)) camera :
      public graphics::gpu::buffers::const_buffer<CONST_CAMERA_BUFFER>,
      public qgl::content::content_item
   {
      public:
      camera(const content::buffers::CAMERA_BUFFER* cBuffer,
             float viewWidth,
             float viewHeight,
             graphics::d3d_device* dev_p,
             const wchar_t* name,
             const qgl::content::content_id id) :
         const_buffer(dev_p),
         content_item(name, id,
                      qgl::content::RESOURCE_TYPE_CAMERA,
                      qgl::content::CONTENT_LOADER_ID_CAMERA)
      {
         position(DirectX::XMVectorSet(cBuffer->Position[0],
                                       cBuffer->Position[1],
                                       cBuffer->Position[2],
                                       cBuffer->Position[3]));

         up(DirectX::XMVectorSet(cBuffer->Up[0],
                                 cBuffer->Up[1],
                                 cBuffer->Up[2],
                                 cBuffer->Up[3]));

         look_at(DirectX::XMVectorSet(cBuffer->LookAt[0],
                                      cBuffer->LookAt[1],
                                      cBuffer->LookAt[2],
                                      cBuffer->LookAt[3]));

         make_projection(cBuffer->FOV,
                         viewWidth / viewHeight,
                         cBuffer->NearPlane,
                         cBuffer->FarPlane);
         make_view();
      }

      /*
       Copy constructor.
       */
      camera(const camera& c) = default;

      /*
       Move constructor.
       */
      camera(camera&&) = default;

      /*
       Destructor.
       */
      virtual ~camera() = default;

      /*
       Updates the projection matrix.
       If compiled in DEBUG mode, this asserts the value is valid.
       The constant buffer must be mapped before calling this.
       */
      void make_projection(float fov,
                           float aspectRatio,
                           float nearPlane = 1.0f,
                           float farPlane = 1000.0f)
      {
         #ifdef DEBUG
         assert(mapped());
         #endif
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
       Updates the view matrix using the position, look at, and up
       stored in the camera buffer.
       If compiled in DEBUG mode, this asserts the value is valid.
       The constant buffer must be mapped before calling this.
       */
      void make_view()
      {
         #ifdef DEBUG
         assert(mapped());
         #endif
         if constexpr (RightHandMode)
         {
            p_updateViewRH();
         }
         else
         {
            p_updateViewLH();
         }
      }

      /*
       Returns the camera's position.
       If compiled in DEBUG mode, this asserts the value is valid.
       The constant buffer must be mapped before calling this.
       */
      DirectX::XMVECTOR XM_CALLCONV position() const
      {
         #ifdef DEBUG
         assert(mapped());
         #endif
         return mapping()->Position;
      }

      /*
       Sets the camera position.
       If compiled in DEBUG mode, this asserts the value is valid.
       The constant buffer must be mapped before calling this.
       */
      void XM_CALLCONV position(const DirectX::FXMVECTOR pos)
      {
         #ifdef DEBUG
         assert(mapped());
         #endif
         mapping()->Position = pos;
      }

      /*
       Sets the camera's up vector.
       If compiled in DEBUG mode, this asserts the value is valid.
       The constant buffer must be mapped before calling this.
       */
      void XM_CALLCONV up(const DirectX::FXMVECTOR u)
      {
         #ifdef DEBUG
         assert(mapped());
         #endif
         mapping()->Up = u;
      }

      /*
       Sets the camera's look at.
       If compiled in DEBUG mode, this asserts the value is valid.
       The constant buffer must be mapped before calling this.
       */
      void XM_CALLCONV look_at(const DirectX::FXMVECTOR lookAt)
      {
         #ifdef DEBUG
         assert(mapped());
         #endif
         mapping()->LookAt = lookAt;
      }

      private:

      void p_updateViewRH()
      {
         mapping()->View = DirectX::XMMatrixLookToRH(mapping()->Position,
                                                     mapping()->LookAt,
                                                     mapping()->Up);
      }

      void p_updateViewLH()
      {
         mapping()->View = DirectX::XMMatrixLookToLH(mapping()->Position,
                                                     mapping()->LookAt,
                                                     mapping()->Up);
      }

      /*
       Updates the projection matrix stored in the camera buffer.
       */
      void p_updateProjectionRH(float fov,
                                float aspectRatio,
                                float nearPlane,
                                float farPlane)
      {
         mapping()->Projection = DirectX::XMMatrixPerspectiveFovRH(fov,
                                                                   aspectRatio,
                                                                   nearPlane,
                                                                   farPlane);
      }

      void p_updateProjectionLH(float fov,
                                float aspectRatio,
                                float nearPlane,
                                float farPlane)
      {
         mapping()->Projection = DirectX::XMMatrixPerspectiveFovLH(fov,
                                                                   aspectRatio,
                                                                   nearPlane,
                                                                   farPlane);
      }
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