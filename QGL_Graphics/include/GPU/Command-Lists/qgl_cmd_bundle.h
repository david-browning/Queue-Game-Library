#pragma once
#include "include/GPU/Command-Lists/qgl_icmd_list.h"

namespace qgl::graphics::gpu
{
   template<bool ComputeBundle = false>
   class cmd_bundle : public icommand_list
   {
      public:
      cmd_bundle(d3d_device* dev_p,
                 pipeline_state* pipelineState_p,
                 UINT nodeMask = 0) :
         icommand_list(dev_p, 
                       D3D12_COMMAND_LIST_TYPE_BUNDLE,
                       pipelineState_p,
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

      virtual void root_sig(root_signature* sig)
      {
         if (ComputeBundle)
         {
            get()->SetComputeRootSignature(sig->get());
         }
         else
         {
            get()->SetGraphicsRootSignature(sig->get());
         }
      }

      virtual void table(descriptor_table* tbl)
      {
         if (ComputeBundle)
         {
            get()->SetGraphicsRootDescriptorTable(tbl->root_index(),
                                                  tbl->where());
         }
         else
         {
            get()->SetComputeRootDescriptorTable(tbl->root_index(),
                                                 tbl->where());
         }
      }
   };
}