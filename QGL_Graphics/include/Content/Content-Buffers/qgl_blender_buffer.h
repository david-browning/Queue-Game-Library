#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_blender_desc_buffer.h"

namespace qgl::content::buffers
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
   struct QGL_GRAPHICS_API BLENDER_BUFFER
   {
      public:
      /*
       Constructs a default blender buffer.
       Alpha Coverage is disabled.
       Independent Blend is disabled.
       The blend descriptions are default.
       */
      BLENDER_BUFFER();

      /*
       Copy constructor.
       */
      BLENDER_BUFFER(const BLENDER_BUFFER&) = default;

      /*
       Move constructor.
       */
      BLENDER_BUFFER(BLENDER_BUFFER&&) = default;

      /*
       Destructor. Do not mark as virtual.
       */
      ~BLENDER_BUFFER() noexcept = default;

      bool alpha_coverage() const noexcept;

      bool independent_blend() const noexcept;

      const float* blend_factor() const noexcept;

      /*
       Returns a pointer to an array of blend descriptions. The array has 
       NUM_RENDER_TARGETS entries.
       */
      const BLEND_DESC* blend_descs() const noexcept;

      /*
       Swaps the contents of first with the contents of second.
       */
      friend void swap(BLENDER_BUFFER& first,
                       BLENDER_BUFFER& second) noexcept
      {
         using std::swap;
         swap(first.Flags, second.Flags);
         swap(first.BlendDescriptions, second.BlendDescriptions);
         swap(first.IsAlphaToCoverage, second.IsAlphaToCoverage);
         swap(first.IsIndependentBlend, second.IsIndependentBlend);
         swap(first.BlendFactor, second.BlendFactor);
      }

      /*
       Assignment operator.
       */
      BLENDER_BUFFER& operator=(BLENDER_BUFFER r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Unused right now.
       */
      uint16_t Flags;

      /*
       Specifies whether to use alpha-to-coverage as a multisampling
       technique when setting a pixel to a render target.
       */
      uint8_t IsAlphaToCoverage;

      /*
       Specifies whether to enable independent blending in simultaneous render
       targets. Set to TRUE to enable independent blending. If set to FALSE,
       only the RenderTarget[0] members are used; RenderTarget[1..7] are
       ignored.
       */
      uint8_t IsIndependentBlend;

      float BlendFactor[4];

      /*
       An array of D3D12_RENDER_TARGET_BLEND_DESC structures that describe the 
       blend states for render targets; these correspond to the eight render 
       targets that can be bound to the output-merger stage at one time.
       */
      BLEND_DESC BlendDescriptions[NUM_RENDER_TARGETS];

      static constexpr uint16_t DEFAULT_FLAGS = 0xEEEE;
   };
}