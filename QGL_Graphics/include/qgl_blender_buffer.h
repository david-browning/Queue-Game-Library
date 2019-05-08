#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics
{
   static constexpr auto NUM_RENDER_TARGETS =
      sizeof(D3D12_BLEND_DESC::RenderTarget) / sizeof(D3D12_RENDER_TARGET_BLEND_DESC);

   struct LIB_EXPORT BLEND_DESC
   {
      public:
      BLEND_DESC();

      BLEND_DESC(const BLEND_DESC& r);

      BLEND_DESC(BLEND_DESC&& r);

      BLEND_DESC& operator=(const BLEND_DESC& r);

      BLEND_DESC& operator=(const D3D12_RENDER_TARGET_BLEND_DESC& r);

      D3D12_RENDER_TARGET_BLEND_DESC d3d_version() const;

      uint8_t BlendEnable;
      uint8_t LogicOpEnable;
      uint8_t SrcBlend;
      uint8_t DestBlend;
      uint8_t BlendOp;
      uint8_t SrcBlendAlpha;
      uint8_t DestBlendAlpha;
      uint8_t BlendOpAlpha;
      uint8_t LogicOp;
      uint8_t RenderTargetWriteMask;
   };

   struct LIB_EXPORT BLENDER_BUFFER
   {
      public:
      BLENDER_BUFFER();

      BLENDER_BUFFER(const BLENDER_BUFFER& r);

      BLENDER_BUFFER(BLENDER_BUFFER&& r);

      inline bool alpha_coverage() const
      {
         return static_cast<bool>(m_isAlphaToCoverage);
      }

      inline bool independent_blend() const
      {
         return static_cast<bool>(m_isIndependentBlend);
      }

      inline size_t description_count() const
      {
         return static_cast<size_t>(m_numTargets);
      }

      inline const BLEND_DESC* blend_descs() const
      {
         return m_blend_descs;
      }

      private:
      uint8_t m_flags;
      uint8_t m_isAlphaToCoverage;
      uint8_t m_isIndependentBlend;
      uint8_t m_numTargets;
      BLEND_DESC m_blend_descs[NUM_RENDER_TARGETS];
   };
}