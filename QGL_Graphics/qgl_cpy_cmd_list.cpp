#include "pch.h"
#include "include/GPU/Command-Lists/qgl_cpy_cmd_list.h"

namespace qgl::graphics::gpu
{
   struct copy_command_list::impl
   {

   };

   copy_command_list::copy_command_list(d3d_device* dev_p,
                                        pipeline_state* pipelineState_p,
                                        UINT nodeMask) :
      icommand_list(dev_p,
                    D3D12_COMMAND_LIST_TYPE_COPY,
                    pipelineState_p,
                    nodeMask)
   {
   }

   copy_command_list::copy_command_list(copy_command_list&& r) :
      icommand_list(std::move(r))
   {
      delete m_impl_p;
      m_impl_p = r.m_impl_p;
      r.m_impl_p = nullptr;
   }

   copy_command_list::~copy_command_list()
   {
      delete m_impl_p;
      m_impl_p = nullptr;
   }

   void copy_command_list::root_sig(root_signature* sig)
   {
      throw std::runtime_error("Copy command lists do not support root "
                               "signatures.");
   }

   void copy_command_list::table(descriptor_table * tbl)
   {
      throw std::runtime_error("Copy command lists do not support descriptor "
                               "tables.");
   }

   void copy_command_list::begin()
   {
      reset();
   }

   void copy_command_list::end()
   {
      
   }

}