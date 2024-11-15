#pragma once
#include "include/qgl_math_includes.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d2d1.h>

namespace qgl::math
{
   /*
    2D rectangle existing in 3D space. The normal is available by calling
    norm() or norm_cache().
    norm() computes and caches the normal vector. The cached vector can be
    retrieved by calling norm_cache().
    If the rectangle coordinates change, the normal will need to be recomputed.
    The constructor calculates the normal vector.
    */
   class rectangle
   {
      public:
      /*
       Constructs a rectangle with the given upper left and lower right
       coordinates.
       The W component is ignored in calculations.
       */
      rectangle(DirectX::FXMVECTOR upperLeft,
                DirectX::FXMVECTOR bottomRight)
      {
         m_ul = upperLeft;
         m_br = bottomRight;
         norm();
      }

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
      DirectX::XMVECTOR XM_CALLCONV ul() const noexcept
      {
         return m_ul;
      }

      /*
       Returns the rectangle's bottom right corner.
       */
      DirectX::XMVECTOR XM_CALLCONV br() const noexcept
      {
         return m_br;
      }

      /*
       Sets the rectangle's upper left corner.
       This may invalidate the cached normal vector.
       */
      void XM_CALLCONV ul(DirectX::FXMVECTOR pos) noexcept
      {
         m_ul = pos;
      }

      /*
       Sets the rectangle's bottom right corner.
       This may invalidate the cached normal vector.
       */
      void XM_CALLCONV br(DirectX::FXMVECTOR pos) noexcept
      {
         m_br = pos;
      }

      /*
       Computes the rectangle's normal vector and returns it.
       Caches the normal so it can be retried again with recomputing it.
       */
      DirectX::XMVECTOR XM_CALLCONV norm() const noexcept
      {
         /*
          U------
          |\    |
          | \   |
          |  \  |
          |   \ |
          |    \|
          a-----B

          N = aU X aB
          aU = a - U
          aB = a - B
          a = min(U, B)
         */
         const DirectX::XMVECTOR a = DirectX::XMVectorMin(m_ul, m_br);
         m_norm = DirectX::XMVector3Cross(
            DirectX::XMVectorSubtract(a, m_ul),
            DirectX::XMVectorSubtract(a, m_br));
         return m_norm;
      }

      /*
       Return's the rectangle's cached normal vector. If the rectangle
       coordinates change, this may be invalid. The cached normal can be
       updated by calling norm().
       */
      DirectX::XMVECTOR XM_CALLCONV norm_cache() const noexcept
      {
         return m_norm;
      }

      /*
       Returns a 2D rectangle. The Z coordinates in this are discarded.
       */
      D2D_RECT_F rect_2d() const noexcept
      {
         D2D_RECT_F ret;
         ret.left = DirectX::XMVectorGetX(m_ul);
         ret.top = DirectX::XMVectorGetY(m_ul);
         ret.right = DirectX::XMVectorGetX(m_br);
         ret.bottom = DirectX::XMVectorGetY(m_br);
         return ret;
      }

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