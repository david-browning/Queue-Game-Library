#pragma once
#include "include/qgl_game_include.h"
#include "include/Descriptors/qgl_camera_descriptor.h"


namespace qgl::components
{
   using namespace qgl::graphics;

   static constexpr guid CAMERA_GUID{ "EF8AE8C04FD541F788EB245570C85AD3" };

#pragma pack(push, 1)
   struct alignas(alignof(DirectX::XMVECTOR)) CONST_CAMERA_BUFFER
   {
      DirectX::XMMATRIX view;
      DirectX::XMMATRIX projection;
      DirectX::XMVECTOR position;
      DirectX::XMVECTOR look;
      DirectX::XMVECTOR up;
      char padding[256 - 
         (2 * sizeof(DirectX::XMMATRIX)) - 
         (3 * sizeof(DirectX::XMVECTOR))];
   };
#pragma pack(pop)

   /*
    Camera uses left-hand coordinates.
    */
   class camera final : public gpu::const_buffer<CONST_CAMERA_BUFFER>,
                        public game_component<camera>
   {
      public:
      /*
       This does not own the allocator pointer. Do not free the allocator or let
       it go out of scope before destroying this.
       */
      camera(const descriptors::camera_descriptor& desc,
             float aspectRatio,
             gpu::igpu_allocator* allocator_p,
             game_update_functor<camera> updateFunctor) :
         const_buffer(allocator_p),
         component(CAMERA_GUID, updateFunctor)
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

      camera(camera&&) noexcept = delete;

      virtual ~camera() noexcept = default;

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