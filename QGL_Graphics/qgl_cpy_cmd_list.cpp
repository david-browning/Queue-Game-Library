#include "pch.h"
#include "include/qgl_cpy_cmd_list.h"

qgl::graphics::low::copy_command_list::copy_command_list(const winrt::com_ptr<d3d_device>& dev_p,
                                                         pipeline_state& pipelineState_p,
                                                         UINT nodeMask) :
   m_bufferAllocator(dev_p),
   m_textureAllocator(dev_p),
   m_pendingTransitions(0),
   icommand_list(dev_p, pipelineState_p, nodeMask)
{
}

qgl::graphics::low::copy_command_list::~copy_command_list()
{
}

void qgl::graphics::low::copy_command_list::begin()
{
   m_pendingTransitions.resize(0);
   reset();
}

void qgl::graphics::low::copy_command_list::flush()
{
   p_flush_pending_transitions();
   winrt::check_hresult(m_cmdList_p->Close());

   //Now that all of the pending heaps are done, deallocate all of them.
   m_bufferAllocator.clear();
   m_textureAllocator.clear();
}

void qgl::graphics::low::copy_command_list::p_flush_pending_transitions()
{
   m_cmdList_p->ResourceBarrier(static_cast<UINT>(m_pendingTransitions.size()),
                                m_pendingTransitions.data());
}
