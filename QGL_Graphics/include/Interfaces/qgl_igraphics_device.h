#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/qgl_graphics_config.h"

namespace qgl::graphics
{
   struct igraphics_device : public iqgl
   {




      virtual const d3d_device* d3d12_device() const noexcept = 0;

      virtual d3d_device* d3d12_device() noexcept = 0;

      virtual const d3d11_device* d3d11on12_device() const noexcept = 0;

      virtual d3d11_device* d3d11on12_device() noexcept = 0;

      virtual const d2d_context* d2d1_context() const noexcept = 0;

      virtual d2d_context* d2d1_context() noexcept = 0;

      virtual const d3d11_context* d3d11on12_context() const noexcept = 0;

      virtual d3d11_context* d3d11on12_context() noexcept = 0;

      virtual const d2d_factory* d2d1_factory() const noexcept = 0;

      virtual d2d_factory* d2d1_factory() noexcept = 0;

      virtual const d3d_cmd_queue* cmd_queue() const noexcept = 0;

      virtual d3d_cmd_queue* cmd_queue() noexcept = 0;

      virtual const d3d_swap_chain* swap_chain() const noexcept = 0;

      virtual d3d_swap_chain* swap_chain() noexcept = 0;

      virtual const dxgi_factory* dx_factory() const noexcept = 0;

      virtual dxgi_factory* dx_factory() noexcept = 0;

      virtual const IDXGIAdapter4* adapter() const noexcept = 0;

      virtual IDXGIAdapter4* adapter() noexcept = 0;
   };
}