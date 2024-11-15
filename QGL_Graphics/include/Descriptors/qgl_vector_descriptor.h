#pragma once
#include "include/qgl_graphics_include.h"
#include <QGLMath.h>
#include <QGLStruct.h>

namespace qgl::descriptors
{
   /*
    Describes a 4 component vector.
    Each element is a rational number of two 32-bit values.
    By default, the vector is zero.
    */
#pragma pack(push, 1)
   struct vector_descriptor final
   {
      using element_t = typename math::rational<int32_t>;


      constexpr vector_descriptor()
      {
          
      }

      vector_descriptor(const element_t& x,
                        const element_t& y,
                        const element_t& z,
                        const element_t& w)
      {
         vector[0] = x;
         vector[1] = y;
         vector[2] = z;
         vector[3] = w;
      }

      constexpr vector_descriptor(const vector_descriptor&) = default;

      constexpr vector_descriptor(vector_descriptor&&) noexcept = default;

      ~vector_descriptor() noexcept = default;

      friend void swap(vector_descriptor& l, vector_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.vector, r.vector);
      }

      vector_descriptor& operator=(vector_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      explicit XM_CALLCONV operator DirectX::XMVECTOR() const noexcept
      {
         return DirectX::XMVectorSet(
            vector[0].operator float(),
            vector[1].operator float(),
            vector[2].operator float(),
            vector[3].operator float());
      }

      element_t& operator[](size_t i)
      {
         return vector[i];
      }

      const element_t& operator[](size_t i) const
      {
         return vector[i];
      }

      fixed_buffer<element_t, 4> vector;
   };
#pragma pack(pop)
}