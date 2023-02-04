#pragma once
#include "include/GPU/Command-Lists/qgl_icmd_list.h"

namespace qgl::graphics::gpu
{
   template<bool ComputeBundle = false>
   class cmd_bundle : public icommand_list<D3D12_COMMAND_LIST_TYPE_BUNDLE>
   {
      public:
      cmd_bundle(graphics_device& dev,
                 gpu::ipso& pso,
                 gpu_idx_t nodeMask = 0) :
         icommand_list(dev,
                       pso,
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

      virtual void root_sig(root_signature& sig) override
      {
         root_sig_sfinae<ComputeBundle>(sig);
      }

      virtual void table(descriptor_table& tbl) override
      {
         table_sfinae<ComputeBundle>(tbl);
      }

      private:
      template<bool compute = ComputeBundle>
      typename std::enable_if<compute>::type root_sig_sfinae(
         root_signature& sig)
      {
         get()->SetComputeRootSignature(sig.get());
      }

      template<bool compute = ComputeBundle>
      typename std::enable_if<!compute>::type root_sig_sfinae(
         root_signature& sig)
      {
         get()->SetGraphicsRootSignature(sig.get());
      }

      template<bool compute = ComputeBundle>
      typename std::enable_if<compute>::type table_sfinae(descriptor_table& tbl)
      {
         get()->SetGraphicsRootDescriptorTable(tbl.root_index(), tbl.where());
      }

      template<bool compute = ComputeBundle>
      typename std::enable_if<!compute>::type table_sfinae(descriptor_table& tbl)
      {
         get()->SetComputeRootDescriptorTable(tbl.root_index(), tbl.where());
      }
   };
}