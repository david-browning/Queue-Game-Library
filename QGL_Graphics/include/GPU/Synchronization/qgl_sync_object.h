#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::sync
{
   template<typename ValueT>
   class fence;

   template<typename ValueT>
   class sync_object
   {
      friend class fence<ValueT>;

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

      private:
      sync_object(ValueT x) :
         m_value(x)
      {

      }

      ValueT m_value;
   };
}