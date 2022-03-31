#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Helpers/qgl_dx_constructors.h"
#include "include/GPU/Memory/qgl_igpu_allocator.h"
#include "include/GPU/Buffers/qgl_const_buffer.h"
#include "include/Content/Content-Descriptors/qgl_camera_descriptor.h"

namespace qgl::graphics
{
   struct alignas(alignof(DirectX::XMVECTOR)) CONST_CAMERA_BUFFER
   {
      DirectX::XMMATRIX view;
      DirectX::XMMATRIX projection;
      DirectX::XMVECTOR position;
      DirectX::XMVECTOR look;
      DirectX::XMVECTOR up;
   };

   /*
    Camera uses left-hand coordinates.
    */
   class alignas(alignof(DirectX::XMMATRIX)) camera :
      public gpu::const_buffer<CONST_CAMERA_BUFFER>
   {
      camera(const descriptors::camera_descriptor& desc,
             float aspectRatio,
             gpu::gpu_allocator_ptr&& allocator) :
         const_buffer(std::forward<gpu::gpu_allocator_ptr>(allocator))
      {
         map();
         position(DirectX::XMVectorSet(
            static_cast<float>(desc.position[0]),
            static_cast<float>(desc.position[1]),
            static_cast<float>(desc.position[2]),
            static_cast<float>(desc.position[3])));

         up(DirectX::XMVectorSet(
            static_cast<float>(desc.up[0]),
            static_cast<float>(desc.up[1]),
            static_cast<float>(desc.up[2]),
            static_cast<float>(desc.up[3])));

         look_at(DirectX::XMVectorSet(
            static_cast<float>(desc.look[0]),
            static_cast<float>(desc.look[1]),
            static_cast<float>(desc.look[2]),
            static_cast<float>(desc.look[3])));

         make_projection(
            static_cast<float>(desc.fov),
            aspectRatio,
            static_cast<float>(desc.near_plane),
            static_cast<float>((desc.far_plane)));
         make_view();
      }

      camera(const camera&) = delete;

      camera(camera&&) = default;

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
         mapping()->projection = DirectX::XMMatrixPerspectiveFovLH(fov,
                                                                   aspectRatio,
                                                                   nearPlane,
                                                                   farPlane);
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
         mapping()->view = DirectX::XMMatrixLookToLH(mapping()->position,
                                                     mapping()->look,
                                                     mapping()->up);
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
         return mapping()->position;
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
         mapping()->position = pos;
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
         mapping()->up = u;
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
         mapping()->look = lookAt;
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