#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/igpu_buffer.h"
#include "include/Content/Content-Buffers/qgl_rasterizer_buffer.h"

namespace qgl::content
{
   class QGL_GRAPHICS_API rasterizer : public qgl::content::content_item
   {
      public:
      /*
       Constructs a default rasterizer.
       */
      rasterizer(const wchar_t* name,
                 qgl::content::content_id id);

      /*
       Constructs a rasterizer from the buffer.
       */
      rasterizer(const content::buffers::RASTERIZER_BUFFER* buff,
                 const wchar_t* name,
                 qgl::content::content_id id);

      /*
       Copy constructor.
       */
      rasterizer(const rasterizer&) = default;

      /*
       Move constructor.
       */
      rasterizer(rasterizer&&) = default;

      /*
       Destructor.
       */
      virtual ~rasterizer() noexcept = default;

      /*
       Returns a pointer to the D3D rasterizer description.
       Used by the pipeline state object.
       */
      const D3D12_RASTERIZER_DESC* description() const;

      private:
      D3D12_RASTERIZER_DESC m_desc;
   };
}