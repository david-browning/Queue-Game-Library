#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics
{
   class viewport;

   class LIB_EXPORT scissor
   {
      public:
      scissor(const viewport& vp);

      scissor(const scissor& r);

      scissor(scissor&& r);

      virtual ~scissor();

      inline const auto& get() const noexcept
      {
         return m_scissor;
      }

      inline auto& get() noexcept
      {
         return m_scissor;
      }

      friend void swap(scissor& first, scissor& second) noexcept
      {
         using std::swap;
         swap(first.m_scissor, second.m_scissor);
      }

      scissor& operator=(scissor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:
      D3D12_RECT m_scissor;
   };
}
