#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Descriptors/qgl_multisampler_descriptor.h"

namespace qgl::graphics
{
   class multisampler
   {
      public:
      constexpr multisampler(const descriptors::multisampler_descriptor& desc) :
         m_count(desc.count),
         m_quality(desc.quality)
      {

      }

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