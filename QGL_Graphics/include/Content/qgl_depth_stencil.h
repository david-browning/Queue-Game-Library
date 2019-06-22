#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/igpu_buffer.h"
#include "include/Content/Content-Buffers/qgl_depth_stencil_buffer.h"
#include "include/qgl_window.h"

namespace qgl::graphics
{
   class igraphics_device;
}

namespace qgl::graphics::gpu
{
   class dsv_descriptor_heap;
}

namespace qgl::content
{
   /*
    Depth-stencil testing is used by the output merger to determine if a pixel 
    should be drawn or not.
    Depth data is used to determine which pixels lie closest to the camera.
    Stencil data is used to mask which pixels can be updated.
    */
   class QGL_GRAPHICS_API depth_stencil :
      public graphics::gpu::buffers::igpu_buffer<
         CD3DX12_RESOURCE_DESC,
         D3D12_DEPTH_STENCIL_VIEW_DESC,
         graphics::d3d_resource>,
      public content_item
   {
      public:
      using ResourceDescriptionT = CD3DX12_RESOURCE_DESC;
      using ViewDescriptionT = D3D12_DEPTH_STENCIL_VIEW_DESC;

      /*
       Constructs a depth stencil texture.
       buffer: Describes the parameters for the depth stencil texture.
       dev_p: Used to create the depth stencil texture resource.
       dsvHeap: Once constructed, this depth stencil is bound to this depth 
        stencil view descriptor heap.
       wnd_p: Provide the viewport parameters for the depth stencil texture.
       frameIndex: Index where to put the view descriptor in the DSV descriptor
        heap.
       */
      depth_stencil(const buffers::DEPTH_STENCIL_BUFFER* buffer,
                    graphics::igraphics_device* dev_p,
                    const graphics::gpu::dsv_descriptor_heap* dsvHeap,
                    const graphics::window* wnd_p,
                    UINT frameIndex,
                    const wchar_t* name,
                    id_t id);

      /*
       Do not allow copying because only one depth stencil can bind to any one 
       descriptor slot.
       */
      depth_stencil(const depth_stencil&) = delete;

      /*
       Move constructor.
       */
      depth_stencil(depth_stencil&&);

      /*
       Destructor.
       */
      virtual ~depth_stencil() noexcept;

      /*
       Describes the Z-Buffer texture.
       */
      virtual const ResourceDescriptionT* description() const;

      /*
       The view description that gets bound to a depth stencil view descriptor 
       heap.
       */
      virtual const ViewDescriptionT* view() const;

      /*
       Format of the depth stencil.
       */
      DXGI_FORMAT format() const noexcept;

      const D3D12_DEPTH_STENCIL_DESC* depth_desc() const noexcept;

      /*
       Value to use when clearing the depth buffer.
       */
      float depth() const noexcept;

      /*
       Value to use when clearing the stencil buffer.
       */
      uint8_t stencil() const noexcept;

      /*
       Describes where, in the DSV descriptor heap, the depth stencil view is.
       */
      D3D12_CPU_DESCRIPTOR_HANDLE where() const noexcept;

      /*
       Returns a pointer to an array of rectangles.
       */
      const D3D12_RECT* rectangles() const noexcept;

      /*
       Sets the rectangles for the depth stencil.
       */
      void rectangles(const D3D12_RECT* rects, size_t numRects);

      /*
       Returns the number of rectangles.
       */
      size_t rectangle_count() const noexcept;

      private:
      void construct(graphics::d3d_device* dev_p);

      ResourceDescriptionT m_desc;
      ViewDescriptionT m_viewDesc;
      D3D12_CPU_DESCRIPTOR_HANDLE m_cpuHandle;
      D3D12_CLEAR_VALUE m_clearValue;
      D3D12_DEPTH_STENCIL_DESC m_depthDesc;
      D3D12_RECT* m_rects;
      size_t m_numRects;
      buffers::DEPTH_STENCIL_BUFFER m_buffer;
      UINT m_width;
      UINT m_height;
      UINT m_frameIndex;
   };
}