#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Descriptors/qgl_blend_descriptor.h"

namespace qgl::graphics::descriptors
{
   /*
    Number of render targets that can be bound to the output merger at once.
    */
   static constexpr auto NUM_RENDER_TARGETS =
      sizeof(D3D12_BLEND_DESC::RenderTarget) /
      sizeof(D3D12_RENDER_TARGET_BLEND_DESC);

   /*
    Describes a blender which is used by a graphics pipeline state.
    https://tinyurl.com/d3d12-blend-desc
    */
#pragma pack(push, 1)
   struct blender_descriptor final
   {
      /*
       Constructs a default blender buffer.
       Alpha Coverage is disabled.
       Independent Blend is disabled.
       The blend descriptions are default.
       */
      constexpr blender_descriptor() :
         flags(DEFAULT_FLAGS),
         alpha_to_coverage(FALSE),
         independent_blend(FALSE)
      {
      }

      blender_descriptor(const blender_descriptor&) = default;

      blender_descriptor(blender_descriptor&&) = default;

      ~blender_descriptor() noexcept = default;

      friend void swap(blender_descriptor& l,
         blender_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.flags, r.flags);
         swap(l.blend_descriptions, r.blend_descriptions);
         swap(l.alpha_to_coverage, r.alpha_to_coverage);
         swap(l.independent_blend, r.independent_blend);
         swap(l.blend_factor, r.blend_factor);
      }

      blender_descriptor& operator=(blender_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Unused right now.
       */
      uint16_t flags;

      /*
       Specifies whether to use alpha-to-coverage as a multisampling
       technique when setting a pixel to a render target.
       */
      uint8_t alpha_to_coverage;

      /*
       Specifies whether to enable independent blending in simultaneous render
       targets. Set to TRUE to enable independent blending. If set to FALSE,
       only the RenderTarget[0] members are used; RenderTarget[1..7] are
       ignored.
       */
      uint8_t independent_blend;

      math::rational<int32_t> blend_factor[4];

      /*
       An array of D3D12_RENDER_TARGET_BLEND_DESC structures that describe the
       blend states for render targets; these correspond to the eight render
       targets that can be bound to the output-merger stage at one time.
       */
      blend_descriptor blend_descriptions[NUM_RENDER_TARGETS];

      private:
      static constexpr uint16_t DEFAULT_FLAGS = 0xEEEE;
   };
#pragma pack(pop)
}