#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_camera_buffer.h"
#include "include/GPU/Buffers/qgl_const_buffer.h"
#pragma warning(push)
#pragma warning(disable: 4324)

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

   /*
    Camera uses left-hand coordinates.
    */
   class alignas(alignof(DirectX::XMMATRIX)) camera :
      public graphics::gpu::buffers::const_buffer<CONST_CAMERA_BUFFER>,
      public qgl::content::content_item
   {
      public:
      /*
       */
      camera(const content::buffers::CAMERA_BUFFER* cBuffer,
             float aspectRatio,
             graphics::d3d_device* dev_p,
             const wchar_t* name,
             const qgl::content::content_id id);

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
                           float farPlane = 1000.0f);

      /*
       Updates the view matrix using the position, look at, and up
       stored in the camera buffer.
       If compiled in DEBUG mode, this asserts the value is valid.
       The constant buffer must be mapped before calling this.
       */
      void make_view();

      /*
       Returns the camera's position.
       If compiled in DEBUG mode, this asserts the value is valid.
       The constant buffer must be mapped before calling this.
       */
      DirectX::XMVECTOR XM_CALLCONV position() const;

      /*
       Sets the camera position.
       If compiled in DEBUG mode, this asserts the value is valid.
       The constant buffer must be mapped before calling this.
       */
      void XM_CALLCONV position(const DirectX::FXMVECTOR pos);

      /*
       Sets the camera's up vector.
       If compiled in DEBUG mode, this asserts the value is valid.
       The constant buffer must be mapped before calling this.
       */
      void XM_CALLCONV up(const DirectX::FXMVECTOR u);

      /*
       Sets the camera's look at.
       If compiled in DEBUG mode, this asserts the value is valid.
       The constant buffer must be mapped before calling this.
       */
      void XM_CALLCONV look_at(const DirectX::FXMVECTOR lookAt);
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

#pragma warning(pop)