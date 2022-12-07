#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Descriptors/qgl_multisampler_descriptor.h"

namespace qgl::graphics
{
   class multisampler final
   {
      public:
      constexpr multisampler(const descriptors::multisampler_descriptor& desc) :
         m_count(desc.count),
         m_quality(desc.quality)
      {

      }

      multisampler(const multisampler&) = default;

      multisampler(multisampler&&) = default;

      ~multisampler() noexcept = default;

      constexpr size_t count() const noexcept
      {
         return m_count;
      }

      constexpr size_t quality() const noexcept
      {
         return m_quality;
      }

      private:
      size_t m_count;
      size_t m_quality;
   };
}