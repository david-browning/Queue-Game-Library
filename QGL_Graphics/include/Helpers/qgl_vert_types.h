#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Helpers/qgl_graphics_device_helpers.h"
#include "include/Descriptors/qgl_vertex_element_descriptor.h"

namespace qgl::graphics::vert
{
   /*
    Abstract representation of a semantic that is passed to the graphics
    pipeline.
    */
   class ivert_element
   {
      public:
      /*
       The semantic index for the element. A semantic index modifies a semantic,
       with an integer index number. A semantic index is only needed in a case
       where there is more than one element with the same semantic.
       For example, a 4x4 matrix would have four components each with the
       semantic name matrix, however each of the four component would have
       different semantic indices (0, 1, 2, and 3).
       */
      ivert_element(const std::string& name, size_t idx, size_t inSlot) :
         m_name(name), m_idx(idx), m_slot(inSlot)
      {

      }

      ivert_element(const ivert_element&) noexcept = default;

      ivert_element(ivert_element&&) noexcept = default;

      virtual ~ivert_element() noexcept = default;

      /*
       A DXGI_FORMAT-typed value that specifies the format of the element data.
       This should match the number of bytes returned by size().
       */
      virtual DXGI_FORMAT format() const noexcept = 0;

      /*
       Returns the size of the data needed to store this element.
       Ex: return sizeof(DirectX::XMFLOAT3)
       */
      virtual size_t size() const noexcept = 0;

      /*
       A value that identifies the input data class for a single input slot.
       */
      virtual D3D12_INPUT_CLASSIFICATION classification() const noexcept
      {
         return D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
      }

      /*
       The HLSL semantic associated with this element in a shader
       input-signature.
       */
      const std::string& name() const noexcept
      {
         return m_name;
      }

      /*
       The semantic index for the element. A semantic index modifies a semantic,
       with an integer index number. A semantic index is only needed in a case
       where there is more than one element with the same semantic.
       For example, a 4x4 matrix would have four components each with the
       semantic name matrix, however each of the four component would have
       different semantic indices (0, 1, 2, and 3).
       */
      size_t index() const noexcept
      {
         return m_idx;
      }

      /*
       The IA stage has n input slots, which are designed to accommodate up to
       n vertex buffers that provide input data. Each vertex buffer must be
       assigned to a different slot; this information is stored in the
       input-layout declaration when the input-layout object is created.
       You may also specify an offset from the start of each buffer to the
       first element in the buffer to be read.
       https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-input-assembler-stage-getting-started
       */
      size_t slot() const noexcept
      {
         return m_slot;
      }

      private:
      std::string m_name;
      size_t m_idx;
      size_t m_slot;
   };

   class vert_element : ivert_element
   {
      public:
      vert_element(const descriptors::vertex_element_descriptor& desc) :
         m_fmt(static_cast<DXGI_FORMAT>(desc.format)),
         m_class(static_cast<D3D12_INPUT_CLASSIFICATION>(desc.data_class)),
         ivert_element({ desc.semantic_name.data(), desc.MAX_SEMANTIC_NAME_LEN },
                       desc.semantic_index, 
                       desc.slot)
      {

      }

      vert_element(const vert_element&) = default;

      vert_element(vert_element&&) noexcept = default;

      virtual ~vert_element() noexcept = default;

      virtual DXGI_FORMAT format() const noexcept
      {
         return m_fmt;
      }

      virtual size_t size() const noexcept
      {
         return helpers::fmt_size(format());
      }

      virtual D3D12_INPUT_CLASSIFICATION classification() const noexcept
      {
         return m_class;
      }

      private:
      DXGI_FORMAT m_fmt;
      D3D12_INPUT_CLASSIFICATION m_class;
   };

   /*
    Specialization of a position vector.
    */
   class pos_velement : public ivert_element
   {
      public:
      pos_velement(size_t idx = 0, size_t inSlot = 0) :
         ivert_element("POSITION", idx, inSlot)
      {

      }

      pos_velement(const pos_velement&) = default;

      pos_velement(pos_velement&&) noexcept = default;

      virtual ~pos_velement() noexcept = default;

      virtual DXGI_FORMAT format() const noexcept
      {
         return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
      }

      virtual size_t size() const noexcept
      {
         return 3 * sizeof(float);
      }
   };

   /*
    Specialization of a normal vector.
    */
   class norm_velement : public ivert_element
   {
      public:
      norm_velement(size_t idx = 0, size_t inSlot = 0) :
         ivert_element("NORMAL", idx, inSlot)
      {

      }

      norm_velement(const norm_velement&) = default;

      norm_velement(norm_velement&&) noexcept = default;

      virtual ~norm_velement() noexcept = default;

      virtual DXGI_FORMAT format() const noexcept
      {
         return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
      }

      virtual size_t size() const noexcept
      {
         return 3 * sizeof(float);
      }
   };

   /*
    Specialization of a 4D color (R, G, B, A).
    */
   class col_velement : public ivert_element
   {
      public:
      col_velement(size_t idx = 0, size_t inSlot = 0) :
         ivert_element("COLOR", idx, inSlot)
      {

      }

      col_velement(const col_velement&) = default;

      col_velement(col_velement&&) noexcept = default;

      virtual ~col_velement() noexcept = default;

      virtual DXGI_FORMAT format() const noexcept
      {
         return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
      }

      virtual size_t size() const noexcept
      {
         return 4 * sizeof(float);
      }
   };

   /*
    Specialization of a texture coordinate.
    */
   class tex_velement : public ivert_element
   {
      public:
      tex_velement(size_t idx = 0, size_t inSlot = 0) :
         ivert_element("TEXCOORD", idx, inSlot)
      {

      }

      tex_velement(const tex_velement&) = default;

      tex_velement(tex_velement&&) noexcept = default;

      virtual ~tex_velement() noexcept = default;

      virtual DXGI_FORMAT format() const noexcept
      {
         return DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
      }

      virtual size_t size() const noexcept
      {
         return 2 * sizeof(float);
      }
   };

   /*
    Specialization of a binormal vector.
    */
   class binorm_velement : public ivert_element
   {
      public:
      binorm_velement(size_t idx = 0, size_t inSlot = 0) :
         ivert_element("BINORMAL", idx, inSlot)
      {

      }

      binorm_velement(const binorm_velement&) = default;

      binorm_velement(binorm_velement&&) noexcept = default;

      virtual ~binorm_velement() noexcept = default;

      virtual DXGI_FORMAT format() const noexcept
      {
         return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
      }

      virtual size_t size() const noexcept
      {
         return 3 * sizeof(float);
      }
   };

   /*
    Specialization of a tangent vector.
    */
   class tan_velement : public ivert_element
   {
      public:
      tan_velement(size_t idx = 0, size_t inSlot = 0) :
         ivert_element("TANGENT", idx, inSlot)
      {

      }

      tan_velement(const tan_velement&) = default;

      tan_velement(tan_velement&&) noexcept = default;

      virtual ~tan_velement() noexcept = default;

      virtual DXGI_FORMAT format() const noexcept
      {
         return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
      }

      virtual size_t size() const noexcept
      {
         return 3 * sizeof(float);
      }
   };
}