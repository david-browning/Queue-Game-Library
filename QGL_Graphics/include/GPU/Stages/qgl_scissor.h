#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::stages
{
   class viewport;

   class QGL_GRAPHICS_API scissor
   {
      public:
      scissor(const viewport* vp);

      scissor(const scissor&) = default;

      scissor(scissor&&) = default;

      virtual ~scissor() noexcept = default;

      const D3D12_RECT* get() const noexcept;

      D3D12_RECT* get() noexcept;

      friend void swap(scissor& l, scissor& r) noexcept
      {
         using std::swap;
         swap(l.m_scissor, r.m_scissor);
      }

      scissor& operator=(scissor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      friend bool operator==(const scissor& l, const scissor& r) noexcept
      {
         return l.m_scissor == r.m_scissor;
      }

      private:
      D3D12_RECT m_scissor;
   };
}