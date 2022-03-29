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

      sync_object(sync_object&&) = default;

      ~sync_object()
      {

      }

      ValueT value() const
      {
         return m_value;
      }

      sync_object(ValueT x) :
         m_value(x)
      {

      }

      private:
      ValueT m_value;
   };
}