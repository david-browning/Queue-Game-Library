#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::gpu
{
   template<typename ValueT>
   class sync_object
   {
      public:
      sync_object() :
         m_value(-1)
      {

      }

      sync_object(const sync_object&) = default;

      sync_object(sync_object&&) noexcept = default;

      virtual ~sync_object() noexcept = default;

      ValueT value() const
      {
         return m_value;
      }

      sync_object(ValueT x) :
         m_value(x)
      {

      }

      friend void swap(sync_object& l, sync_object& r) noexcept
      {
         using std::swap;
         swap(l.m_value, r.m_value);
      }

      sync_object& operator=(sync_object r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:
      ValueT m_value;
   };
}