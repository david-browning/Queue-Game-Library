#include "pch.h"
#include "include/qgl_version.h"

namespace qgl
{
   qgl_version_t& qgl_version_t::operator=(qgl_version_t r)
   {
      swap(*this, r);
      return *this;
   }
}