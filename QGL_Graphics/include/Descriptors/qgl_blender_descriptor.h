#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Descriptors/qgl_blend_descriptor.h"
#include "include/Descriptors/qgl_vector_descriptor.h"

namespace qgl::descriptors
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
      constexpr blender_descriptor()
      {
      }

      constexpr blender_descriptor(const blender_descriptor&) = default;

      constexpr blender_descriptor(blender_descriptor&&) noexcept = default;

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
       An array of D3D12_RENDER_TARGET_BLEND_DESC structures that describe the
       blend states for render targets; these correspond to the eight render
       targets that can be bound to the output-merger stage at one time.
       */
      fixed_buffer<blend_descriptor, NUM_RENDER_TARGETS> blend_descriptions;

      vector_descriptor blend_factor;

      /*
       Unused right now.
       */
      uint16_t flags = 0;

      /*
       Specifies whether to use alpha-to-coverage as a multisampling
       technique when setting a pixel to a render target.
       */
      uint8_t alpha_to_coverage = 0;

      /*
       Specifies whether to enable independent blending in simultaneous render
       targets. Set to TRUE to enable independent blending. If set to FALSE,
       only the RenderTarget[0] members are used; RenderTarget[1..7] are
       ignored.
       */
      uint8_t independent_blend = 0;
   };
#pragma pack(pop)
}