
#include "pch.h"
#include "include/Helpers/qgl_graphics_device_helpers.h"

namespace qgl::graphics::helpers
{
   size_t QGL_CC format_size(DXGI_FORMAT f) noexcept
   {
      return DirectX::BitsPerPixel(f) / size_t(8);
   }

   result_t QGL_CC app_mem(igpu_adapter* dev_p, 
                           gpu_idx_t node,
                           gpu_mem_info* out) noexcept
   {
      if (!out || !dev_p)
      {
         return E_POINTER;
      }

      DXGI_QUERY_VIDEO_MEMORY_INFO info;
      return dev_p->QueryVideoMemoryInfo(
         node, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &info);

      out->available = info.AvailableForReservation;
      out->budget = info.Budget;
      out->reserved = info.CurrentReservation;
      out->used = info.CurrentUsage;
   }

   result_t QGL_CC get_gpu_desc(
      igpu_adapter* adapter_p, gpu_desc* desc_p) noexcept
   {
      DXGI_ADAPTER_DESC3 desc;
      auto ret = adapter_p->GetDesc3(&desc);
      desc_p->dev_id = desc.DeviceId;
      desc_p->name = desc.Description;
      desc_p->smem = desc.DedicatedSystemMemory;
      desc_p->vmem = desc.DedicatedVideoMemory;
      return ret;
   }
}