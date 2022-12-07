#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::descriptors
{
#pragma pack(push, 1)
   struct depth_stencil_operator_descriptor final
   {
      constexpr depth_stencil_operator_descriptor()
      {

      }

      constexpr depth_stencil_operator_descriptor(
         const depth_stencil_operator_descriptor&) = default;

      constexpr depth_stencil_operator_descriptor(
         depth_stencil_operator_descriptor&&) noexcept = default;

      ~depth_stencil_operator_descriptor() noexcept = default;

      friend void swap(depth_stencil_operator_descriptor& l,
                       depth_stencil_operator_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.fail_op, r.fail_op);
         swap(l.depth_fail_op, r.depth_fail_op);
         swap(l.pass_op, r.pass_op);
         swap(l.func, r.func);
      }

      uint8_t fail_op = D3D12_STENCIL_OP_KEEP;
      uint8_t depth_fail_op = D3D12_STENCIL_OP_KEEP;
      uint8_t pass_op = D3D12_STENCIL_OP_KEEP;
      uint8_t func = D3D12_COMPARISON_FUNC_ALWAYS;
   };

   /*
    Describes the parameters for Z-Buffering.
    */
   struct depth_stencil_descriptor final
   {
      public:
      /*
       Creates default depth stencil parameters.
       */
      constexpr depth_stencil_descriptor()
      {
      }

      constexpr depth_stencil_descriptor(
         const depth_stencil_descriptor&) = default;

      constexpr depth_stencil_descriptor(
         depth_stencil_descriptor&&) noexcept = default;

      ~depth_stencil_descriptor() noexcept = default;

      friend void swap(depth_stencil_descriptor& l,
         depth_stencil_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.enable_flags, r.enable_flags);
         swap(l.stencil, r.stencil);
         swap(l.write_mask, r.write_mask);
         swap(l.depth_func, r.depth_func);
         swap(l.stencil_read_mask, r.stencil_read_mask);
         swap(l.stencil_write_mask, r.stencil_write_mask);
         swap(l.format, r.format);
         swap(l.depth, r.depth);
         swap(l.front_face_op, r.front_face_op);
         swap(l.back_face_op, r.back_face_op);
      }

      depth_stencil_descriptor& operator=(depth_stencil_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      constexpr bool depth_enabled() const noexcept
      {
         return enable_flags[0];
      }

      constexpr bool stencil_enabled() const noexcept
      {
         return enable_flags[1];
      }

      void depth_enabled(bool enable) noexcept
      {
         if (enable)
         {
            enable_flags.set(0);
         }
         else
         {
            enable_flags.reset(0);
         }
      }

      void stencil_enabled(bool enable) noexcept
      {
         if (enable)
         {
            enable_flags.set(1);
         }
         else
         {
            enable_flags.reset(1);
         }
      }

      /*
       First bit set means depth functions enabled.
       Second bit mean stencil functions enabled.
       */
      mem::flags<8> enable_flags = 0;

      /*
       When clearing the stencil buffer, this is the value that gets used.
       Default value is 0.
       */
      uint8_t stencil = 0;

      /*
       A D3D12_DEPTH_WRITE_MASK-typed value that identifies a portion of the
       depth-stencil buffer that can be modified by depth data.
       */
      uint8_t write_mask = D3D12_DEPTH_WRITE_MASK_ALL;

      /*
       A D3D12_COMPARISON_FUNC-typed value that identifies a function that 
       compares depth data against existing depth data.
       */
      uint8_t depth_func = D3D12_COMPARISON_FUNC_LESS_EQUAL;

      /*
       Identify a portion of the depth-stencil buffer for reading stencil data.
       */
      uint8_t stencil_read_mask = D3D12_DEFAULT_STENCIL_READ_MASK;

      /*
       Identify a portion of the depth-stencil buffer for writing stencil data.
       */
      uint8_t stencil_write_mask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

      /*
       DXGI_FORMAT format to use for the depth stencil texture.
       Default is a single, 32-bit float, component. The only supported values
       are:
         DXGI_FORMAT_D16_UNORM
         DXGI_FORMAT_D24_UNORM_S8_UINT
         DXGI_FORMAT_D32_FLOAT
         DXGI_FORMAT_D32_FLOAT_S8X24_UINT
       QGL does not verify a that a valid format is set!
       */
      uint16_t format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;

      /*
       This is the value to clear the depth buffer with. It must be between 0
       and 1.
       Default value is 0.
       */
      math::rational<int32_t> depth{ 0, 1 };

      /*
       A D3D12_DEPTH_STENCILOP_DESC structure that describes how to use the
       results of the depth test and the stencil test for pixels whose surface
       normal is facing towards the camera.
       */
      depth_stencil_operator_descriptor front_face_op;

      /*
       A D3D12_DEPTH_STENCILOP_DESC structure that describes how to use the
       results of the depth test and the stencil test for pixels whose surface
       normal is facing away from the camera.
       */
      depth_stencil_operator_descriptor back_face_op;

   };
#pragma pack(pop)
}