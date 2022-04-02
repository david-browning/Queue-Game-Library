#pragma once
#include "include/qgl_math_includes.h"

namespace qgl::math
{
   enum boundary_y_axis
   {
      y_down_positive,
      y_down_negative,
   };

   template<typename T, boundary_y_axis YDown>
   class boundary
   {
      public:
      /*
       Constructs a boundary where all values are 0.
       */
      constexpr boundary()
      {
      }

      /*
       Constructs a boundary from a win32 rectangle.
       In debug mode, this asserts that the left is less than the right and
       top is less than the bottom.
       */
      constexpr boundary(const RECT& r) :
         boundary(T(r.left), T(r.top), T(r.right), T(r.bottom))
      {
      }

      /*
       Creates a boundary that is the intersection of the two.
       */
      boundary(const RECT& r1, const RECT& r2)
      {
         static_assert(YDown == boundary_y_axis::y_down_positive,
                       "Y axis must increment downward for RECT constructor.");
         intersection(r1.left, r1.top, r1.right, r1.bottom,
                      r2.left, r2.top, r2.right, r2.bottom);
      }

      /*
       Creates a boundary that is the intersection of the two.
       */
      boundary(const boundary& r1, const boundary& r2)
      {
         assert(r1.intersects(r2));
         intersection(r1.left, r1.top, r1.right, r1.bottom,
                      r2.left, r2.top, r2.right, r2.bottom);
      }

      /*
       Constructs a boundary from the left (X) axis, top (Y) axis, right (X)
       axis, and bottom (Y) axis.
       In debug mode, this asserts that the left is less than the right and
       top is less than the bottom.
       */
      constexpr boundary(T l, T t, T r, T b) :
         top(t), left(l), bottom(b), right(r)
      {
         assert(l < r);
         if constexpr (YDown == boundary_y_axis::y_down_positive)
         {
            assert(t <= b);
         }
         else
         {
            assert(t >= b);
         }
      }

      boundary(const boundary&) = default;

      boundary(boundary&&) = default;

      ~boundary() noexcept = default;

      T left = 0;
      T top = 0;
      T right = 0;
      T bottom = 0;

      constexpr T width() const noexcept
      {
         return right - left;
      }

      constexpr T height() const noexcept
      {
         if constexpr (YDown == boundary_y_axis::y_down_positive)
         {
            return bottom - top;
         }

         return top - bottom;
      }

      constexpr T area() const noexcept
      {
         return width() * height();
      }

      constexpr bool intersects(const boundary& b) const noexcept
      {
         return left < b.right&& right > b.left &&
            y_op<YDown>::great_op()(top, b.bottom) &&
            y_op<YDown>::less_op()(bottom, b.top);
      }

      friend void swap(boundary& l, boundary& r) noexcept
      {
         using std::swap;
         swap(l.top, r.top);
         swap(l.left, r.left);
         swap(l.right, r.right);
         swap(l.bottom, r.bottom);
      }

      boundary& operator=(boundary r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:


      struct min
      {
         constexpr auto operator()(const T& l, const T& r) const noexcept
         {
            return std::min<T>(l, r);
         }
      };

      struct max
      {
         constexpr auto operator()(const T& l, const T& r) const noexcept
         {
            return std::max<T>(l, r);
         }
      };

      struct less_than
      {
         constexpr auto operator()(const T& l, const T& r) const noexcept
         {
            return l < r;
         }
      };

      struct greater_than
      {
         constexpr auto operator()(const T& l, const T& r) const noexcept
         {
            return l > r;
         }
      };

      template<boundary_y_axis y_down>
      struct y_op
      {
         typedef boundary::less_than less_op;
         typedef boundary::greater_than great_op;
         typedef boundary::min min_op;
         typedef boundary::max max_op;
      };

      template<>
      struct y_op<y_down_positive>
      {
         typedef boundary::greater_than less_op;
         typedef boundary::less_than great_op;
         typedef boundary::max min_op;
         typedef boundary::min max_op;
      };

      /*
       Assume that the rectangles are intersecting. The caller checks this.
       */
      void intersection(T r1L, T r1T, T r1R, T r1B,
                        T r2L, T r2T, T r2R, T r2B) noexcept
      {
         left = std::max<T>(r1L, r2L);
         right = std::min<T>(r1R, r2R);

         top = y_op<YDown>::min_op()(r1T, r2T);
         bottom = y_op<YDown>::max_op()(r1B, r2B);
      }
   };

   template<typename T>
   using window_boundary = typename math::boundary<
      T, 
      math::boundary_y_axis::y_down_positive>;
}