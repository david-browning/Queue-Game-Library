#include "pch.h"
#include "include\Interfaces\qgl_icommand.h"

namespace qgl
{
   icommand::icommand(const GUID* g) :
      m_guid(*g)
   {
   }

   const GUID* icommand::guid() const noexcept
   {
      return &m_guid;
   }
}