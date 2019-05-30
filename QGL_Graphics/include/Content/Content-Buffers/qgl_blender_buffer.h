#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_blender_desc_buffer.h"

namespace qgl::graphics::content::buffers
{
   /*
    Number of render targets that can be bound to the output merger at once.
    */
   static constexpr auto NUM_RENDER_TARGETS =
      sizeof(D3D12_BLEND_DESC::RenderTarget) /
      sizeof(D3D12_RENDER_TARGET_BLEND_DESC);
   
   /*
    Describes a blender which is used by a graphics pipeline state.
    https://docs.microsoft.com/en-us/windows/desktop/api/d3d12/ns-d3d12-d3d12_blend_desc
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

      bool alpha_coverage() const;

      bool independent_blend() const;

      /*
       Returns a pointer to an array of blend descriptions. The array has 
       NUM_RENDER_TARGETS entries.
       */
      const BLEND_DESC* blend_descs() const;

      /*
       Swaps the contents of first with the contents of second.
       */
      friend void swap(BLENDER_BUFFER& first,
                       BLENDER_BUFFER& second) noexcept
      {
         using std::swap;
         swap(first.m_flags, second.m_flags);
         swap(first.m_blend_descs, second.m_blend_descs);
         swap(first.m_isAlphaToCoverage, second.m_isAlphaToCoverage);
         swap(first.m_isIndependentBlend, second.m_isIndependentBlend);
      }

      /*
       Assignment operator.
       */
      BLENDER_BUFFER& operator=(BLENDER_BUFFER r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:

      /*
       Unused right now.
       */
      uint16_t m_flags;

      /*
       Specifies whether to use alpha-to-coverage as a multisampling
       technique when setting a pixel to a render target.
       */
      uint8_t m_isAlphaToCoverage;

      /*
       Specifies whether to enable independent blending in simultaneous render
       targets. Set to TRUE to enable independent blending. If set to FALSE,
       only the RenderTarget[0] members are used; RenderTarget[1..7] are
       ignored.
       */
      uint8_t m_isIndependentBlend;

      /*
       An array of D3D12_RENDER_TARGET_BLEND_DESC structures that describe the 
       blend states for render targets; these correspond to the eight render 
       targets that can be bound to the output-merger stage at one time.
       */
      BLEND_DESC m_blend_descs[NUM_RENDER_TARGETS];

      static constexpr uint16_t DEFAULT_FLAGS = 0xEEEE;
   };
}