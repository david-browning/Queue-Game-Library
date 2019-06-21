#include "pch.h"
#include "include/Content/Content-Buffers/qgl_vertex_desc_header.h"

namespace qgl::content::buffers
{
   VERTEX_DESC_HEADER::VERTEX_DESC_HEADER() :
      Elements(0),
      Topology(static_cast<uint8_t>(D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED)),
      StripCut(
         static_cast<uint8_t>(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED)),
      Reserved1(0),
      Reserved2(0)
   {
   }
}