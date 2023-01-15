#pragma once
#include "include/GPU/Command-Lists/qgl_icmd_list.h"

namespace qgl::graphics::gpu
{
   template<bool ComputeBundle = false>
   class cmd_bundle : public icommand_list
   {
      public:
      cmd_bundle(graphics_device& dev,
                 gpu::ipso& pso,
                 gpu_idx_t nodeMask = 0) :
         icommand_list(dev,
                       pso,
                       D3D12_COMMAND_LIST_TYPE_BUNDLE,
                       nodeMask)
      {

      }

      cmd_bundle(const cmd_bundle&) = default;

      cmd_bundle(cmd_bundle&&) = default;

      virtual ~cmd_bundle() noexcept = default;

      virtual void begin()
      {
         reset();
      }

      virtual typename std::enable_if<ComputeBundle>::type root_sig(
         root_signature& sig)
      {
         get()->SetComputeRootSignature(sig.get());
      }

      virtual typename std::enable_if<!ComputeBundle>::type root_sig(
         root_signature& sig)
      {
         get()->SetGraphicsRootSignature(sig.get());
      }

      virtual typename std::enable_if<ComputeBundle>::type table(
         descriptor_table& tbl)
      {
         get()->SetGraphicsRootDescriptorTable(tbl.root_index(), tbl.where());
      }

      virtual typename std::enable_if<!ComputeBundle>::type table(
         descriptor_table& tbl)
      {
         get()->SetComputeRootDescriptorTable(tbl.root_index(), tbl.where());
      }
   };
}