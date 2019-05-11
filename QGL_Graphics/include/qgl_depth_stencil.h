#pragma once
#include "qgl_graphics_include.h"
#include "qgl_igpu_buffer.h"
#include "qgl_depth_stencil_buffer.h"

namespace qgl::graphics
{
   class dsv_descriptor_heap;
   class window;

   class LIB_EXPORT depth_stencil :
      public low::igpu_buffer<CD3DX12_RESOURCE_DESC, D3D12_DEPTH_STENCIL_VIEW_DESC, d3d_resource>
   {
      using ResourceDescriptionT = CD3DX12_RESOURCE_DESC;
      using ViewDescriptionT = D3D12_DEPTH_STENCIL_VIEW_DESC;

      public:
      depth_stencil(const DEPTH_STENCIL_BUFFER& buffer,
                    const winrt::com_ptr<d3d_device>& dev_p,
                    const window& wnd,
                    size_t frameIndex,
                    const dsv_descriptor_heap& dsvHeap);

      depth_stencil(const depth_stencil& r) = delete;

      depth_stencil(depth_stencil&& r);

      virtual ~depth_stencil() = default;

      #pragma region Description Getters
      virtual ResourceDescriptionT description() const
      {
         return m_desc;
      }

      virtual ViewDescriptionT view() const
      {
         return m_viewDesc;
      }
      #pragma endregion

      DXGI_FORMAT format() const
      {
         return m_desc.Format;
      }

      inline const D3D12_DEPTH_STENCIL_DESC& depth_state() const
      {
         return m_depthState;
      }

      inline D3D12_DEPTH_STENCIL_DESC& depth_state()
      {
         return m_depthState;
      }

      inline float depth() const
      {
         return m_buffer.depth();
      }

      inline uint8_t stencil() const
      {
         return m_buffer.stencil();
      }

      D3D12_CPU_DESCRIPTOR_HANDLE where() const
      {
         return m_handle;
      }

      const std::vector<D3D12_RECT>* rectangles() const
      {
         return m_rectangles.get();
      }

      void rectangles(std::initializer_list<D3D12_RECT>& rects)
      {
         if (!m_rectangles)
         {
            m_rectangles = std::shared_ptr<std::vector<D3D12_RECT>>();
         }

         m_rectangles->resize(rects.size());
         m_rectangles->clear();
         size_t i = 0;
         for (auto& rect : rects)
         {
            m_rectangles->at(i) = rect;
            i++;
         }
      }

      friend void swap(depth_stencil& first, depth_stencil& second) noexcept
      {
         using std::swap;
         swap(first.m_desc, second.m_desc);
         swap(first.m_viewDesc, second.m_viewDesc);
         swap(first.m_handle, second.m_handle);
         swap(first.m_clearValue, second.m_clearValue);
         swap(first.m_depthState, second.m_depthState);
         swap(first.m_rectangles, second.m_rectangles);
         swap(first.m_buffer, second.m_buffer);
         swap(first.m_width, second.m_width);
         swap(first.m_height, second.m_height);
         swap(first.m_frameIndex, second.m_frameIndex);
         swap(static_cast<low::igpu_buffer<CD3DX12_RESOURCE_DESC, D3D12_DEPTH_STENCIL_VIEW_DESC, d3d_resource>&>(first),
              static_cast<low::igpu_buffer<CD3DX12_RESOURCE_DESC, D3D12_DEPTH_STENCIL_VIEW_DESC, d3d_resource>&>(second));
      }

      depth_stencil& operator=(depth_stencil r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:
      void p_allocate();

      #pragma warning(push)
      #pragma warning(disable: 4251)
      ResourceDescriptionT m_desc;
      ViewDescriptionT m_viewDesc;

      D3D12_CPU_DESCRIPTOR_HANDLE m_handle;

      D3D12_CLEAR_VALUE m_clearValue;

      D3D12_DEPTH_STENCIL_DESC m_depthState;

      std::shared_ptr<std::vector<D3D12_RECT>> m_rectangles;

      #pragma warning(pop)

      DEPTH_STENCIL_BUFFER m_buffer;

      size_t m_width;
      size_t m_height;
      size_t m_frameIndex;
   };
}