#pragma once
#include "qgl_graphics_include.h"
#include "qgl_icmd_list.h"
#include "qgl_descriptor_heap.h"

namespace qgl::graphics::low
{
   template<bool ComputeBundle = false>
   class LIB_EXPORT cmd_bundle : public icommand_list<D3D12_COMMAND_LIST_TYPE_BUNDLE>
   {
      public:
      cmd_bundle(winrt::com_ptr<d3d_device>& dev_p,
                 pipeline_state& pipelineState_p,
                 UINT nodeMask = 0) :
         icommand_list(dev_p, pipelineState_p, nodeMask)
      {
      }

      cmd_bundle(const cmd_bundle& r) = delete;

      cmd_bundle(cmd_bundle&& r) :
         icommand_list(std::move(r))
      {
      }

      virtual ~cmd_bundle();

      virtual void begin()
      {
         reset();
      }

      virtual void root_sig(const root_signature& sig)
      {
         if (!ComputeBundle)
         {
            m_cmdList_p->SetGraphicsRootSignature(sig.com_get().get());
         }
         else
         {
            m_cmdList_p->SetComputeRootSignature(sig.com_get().get());
         }
      }

      virtual void table(const descriptor_table& tbl)
      {
         if (!ComputeBundle)
         {
            m_cmdList_p->SetGraphicsRootDescriptorTable(tbl.root_index(), tbl.where());
         }
         else
         {
            m_cmdList_p->SetComputeRootDescriptorTable(tbl.root_index(), tbl.where());
         }
      }
   };
}