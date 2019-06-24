#pragma once
#include "include/qgl_math_includes.h"

namespace qgl::math
{
   class QGL_MATH_API rectangle
   {
      public:
      /*
       Constructs a rectangle with the given upper left and lower right
       coordinates.
       The W component is ignored in calculations.
       */
      rectangle(DirectX::FXMVECTOR upperLeft,
                DirectX::FXMVECTOR bottomRight);

      /*
       Copy constructor.
       */
      rectangle(const rectangle&) = default;

      /*
       Move constructor.
       */
      rectangle(rectangle&&) = default;

      /*
       Destructor.
       */
      ~rectangle() noexcept = default;

      /*
       Swaps the contents of l and r.
       */
      inline friend void swap(rectangle& l, rectangle& r) noexcept
      {
         using std::swap;
         swap(l.m_br, r.m_br);
         swap(l.m_norm, r.m_norm);
         swap(l.m_ul, r.m_ul);
      }

      /*
       Copy assign operator.
       */
      inline rectangle& operator=(rectangle r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Returns true if the two rectangles exist in the same place.
       */
      inline friend bool operator==(const rectangle& l,
                                    const rectangle& r) noexcept
      {
         return DirectX::XMVector3Equal(l.m_br, r.m_br) &&
            DirectX::XMVector3Equal(l.m_ul, r.m_ul);
      }

      /*
       Returns the rectangle's upper left corner.
       */
      DirectX::XMVECTOR XM_CALLCONV ul() const noexcept;

      /*
       Returns the rectangle's bottom right corner.
       */
      DirectX::XMVECTOR XM_CALLCONV br() const noexcept;

      /*
       Sets the rectangle's upper left corner.
       This may invalidate the cached normal vector.
       */
      void XM_CALLCONV ul(DirectX::FXMVECTOR pos) noexcept;

      /*
       Sets the rectangle's bottom right corner.
       This may invalidate the cached normal vector.
       */
      void XM_CALLCONV br(DirectX::FXMVECTOR pos) noexcept;

      /*
       Computes the rectangle's normal vector and returns it.
       Caches the normal so it can be retried again with recomputing it.
       */
      DirectX::XMVECTOR XM_CALLCONV norm() const noexcept;

      /*
       Return's the rectangle's cached normal vector. If the rectangle 
       coordinates change, this may be invalid. The cached normal can be 
       updated by calling norm().
       */
      DirectX::XMVECTOR XM_CALLCONV norm_cache() const noexcept;

      /*
       Returns a 2D rectangle. The Z coordinates in this are discarded.
       */
      D2D_RECT_F rect_2d() const noexcept;

      private:
      /*
       Upper left corner of the rectangle.
       W is ignored in calculations
       */
      DirectX::XMVECTOR m_ul;

      /*
       Bottom right corner of the rectangle.
       W is ignored in calculations.
       */
      DirectX::XMVECTOR m_br;

      /*
       The normal vector.
       */
      mutable DirectX::XMVECTOR m_norm;
   };
}