#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_light_buffer.h"
#include "include/GPU/Buffers/qgl_const_buffer.h"
#pragma warning(push)
#pragma warning(disable: 4324)

namespace qgl::content
{
   struct alignas(alignof(DirectX::XMVECTOR)) QGL_GRAPHICS_API
      LIGHT_CONST_BUFFER
   {
      DirectX::XMVECTOR Position;
      DirectX::XMVECTOR LookAt;
      DirectX::XMVECTOR Color;
      float Intensity;

      uint8_t padding[64 -
         (3 * sizeof(DirectX::XMVECTOR)) -
         sizeof(float)];
   };

   class alignas(alignof(DirectX::XMVECTOR)) QGL_GRAPHICS_API light :
      public graphics::gpu::buffers::const_buffer<LIGHT_CONST_BUFFER>,
      public qgl::content::content_item
   {
      public:
      /*
       Constructs a light as a constant buffer.
       Before changing the light on the CPU side, unmap it by calling unmap().
       After changing it, call map().
       If compiled in DEBUG mode, the getters and setters assert that the
       buffer is mapped.
       */
      light(const content::buffers::LIGHT_BUFFER* lBuffer,
            graphics::d3d_device* dev_p,
            const wchar_t* name,
            const qgl::content::content_id id);

      /*
       Constant buffers cannot be copied.
       */
      light(const light& c) = delete;

      /*
       Move constructor.
       */
      light(light&& m) = default;

      /*
       Destructor.
       */
      virtual ~light() = default;

      /*
       Returns the intensity of the light.
       Values are between 0 and 1.0 inclusive.
       The constant buffer must be mapped before calling this.
       */
      float intensity() const;

      /*
       Sets the intensity of the light. Value must be between 0 and 1.0
       inclusive.
       If compiled in DEBUG mode, this asserts the value is valid.
       The constant buffer must be mapped before calling this.
       */
      void intensity(float intes);

      /*
       Returns the light's position.
       The constant buffer must be mapped before calling this.
       */
      DirectX::XMVECTOR XM_CALLCONV position() const;

      /*
       Sets the light's position.
       The constant buffer must be mapped before calling this.
       */
      void XM_CALLCONV position(const DirectX::FXMVECTOR pos);

      /*
       Returns the light's look at.
       The constant buffer must be mapped before calling this.
       */
      DirectX::XMVECTOR XM_CALLCONV look_at() const;

      /*
       Sets the light's look at.
       The constant buffer must be mapped before calling this.
       */
      void XM_CALLCONV look_at(const DirectX::FXMVECTOR lookAt);

      /*
       Returns the light's color.
       The constant buffer must be mapped before calling this.
       */
      DirectX::XMVECTOR XM_CALLCONV color() const;

      /*
       Sets the light's color.
       The values in the color must be between 0 and 1.0 inclusive.
       If compiled in DEBUG mode, this asserts the value is valid.
       The constant buffer must be mapped before calling this.
       */
      void XM_CALLCONV color(const DirectX::FXMVECTOR colr);
   };
}

#pragma warning(pop)