#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Descriptors/qgl_light_descriptor.h"
#include "include/GPU/Buffers/qgl_const_buffer.h"

namespace qgl::graphics
{
   struct alignas(alignof(DirectX::XMVECTOR)) LIGHT_CONST_BUFFER
   {
      DirectX::XMVECTOR position;
      DirectX::XMVECTOR look;
      DirectX::XMVECTOR color;
      float intensity;

      // Pad this structure up to 64 bytes.
      uint8_t padding[64 - (3 * sizeof(DirectX::XMVECTOR)) - sizeof(float)];
   };

   class alignas(alignof(DirectX::XMVECTOR)) light :
      public gpu::const_buffer<LIGHT_CONST_BUFFER>
   {
      /*
       Constructs a light as a constant buffer.
       Before changing the light on the CPU side, unmap it by calling unmap().
       After changing it, call map().
       If compiled in DEBUG mode, the getters and setters assert that the
       buffer is mapped.
       */
      light(const descriptors::light_descriptor& desc,
         gpu::gpu_allocator_ptr&& allocator) :
         const_buffer(std::forward<gpu::gpu_allocator_ptr>(allocator))
      {
         map();
         intensity(static_cast<float>(desc.intensity));

         color(DirectX::XMVectorSet(
            static_cast<float>(desc.color[0]),
            static_cast<float>(desc.color[1]),
            static_cast<float>(desc.color[2]),
            static_cast<float>(desc.color[3])));

         look_at(DirectX::XMVectorSet(
            static_cast<float>(desc.look[0]),
            static_cast<float>(desc.look[1]),
            static_cast<float>(desc.look[2]),
            static_cast<float>(desc.look[3])));

         position(DirectX::XMVectorSet(
            static_cast<float>(desc.position[0]),
            static_cast<float>(desc.position[1]),
            static_cast<float>(desc.position[2]),
            static_cast<float>(desc.position[3])));
      }

      /*
       Constant buffers cannot be copied.
       */
      light(const light&) = delete;

      light(light&&) = default;

      virtual ~light() = default;

      /*
       Returns the intensity of the light.
       Values are between 0 and 1.0 inclusive.
       The constant buffer must be mapped before calling this.
       */
      float intensity() const
      {
#ifdef DEBUG
         assert(mapped());
#endif
         return mapping()->intensity;
      }

      /*
       Sets the intensity of the light. Value must be between 0 and 1.0
       inclusive.
       If compiled in DEBUG mode, this asserts the value is valid.
       The constant buffer must be mapped before calling this.
       */
      void intensity(float intes)
      {
#ifdef DEBUG
         assert(mapped());
         assert(intes >= 0.0f && intes <= 1.0f);
#endif
         mapping()->intensity = intes;
      }

      /*
       Returns the light's position.
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
       Sets the light's position.
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
       Returns the light's look at.
       The constant buffer must be mapped before calling this.
       */
      DirectX::XMVECTOR XM_CALLCONV look_at() const
      {
#ifdef DEBUG
         assert(mapped());
#endif
         return mapping()->look;
      }

      /*
       Sets the light's look at.
       The constant buffer must be mapped before calling this.
       */
      void XM_CALLCONV look_at(const DirectX::FXMVECTOR lookAt)
      {
#ifdef DEBUG
         assert(mapped());
#endif
         mapping()->look = lookAt;
      }

      /*
       Returns the light's color.
       The constant buffer must be mapped before calling this.
       */
      DirectX::XMVECTOR XM_CALLCONV color() const
      {
#ifdef DEBUG
         assert(mapped());
#endif
         return mapping()->color;
      }

      /*
       Sets the light's color.
       The values in the color must be between 0 and 1.0 inclusive.
       If compiled in DEBUG mode, this asserts the value is valid.
       The constant buffer must be mapped before calling this.
       */
      void XM_CALLCONV color(const DirectX::FXMVECTOR colr)
      {
#ifdef DEBUG
         assert(mapped());
         assert(DirectX::XMVector4LessOrEqual(colr, DirectX::XMVectorSplatOne()));
         assert(DirectX::XMVector4GreaterOrEqual(colr, DirectX::XMVectorZero()));
#endif
         mapping()->color = colr;
      }
   };
}