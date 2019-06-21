#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content::buffers
{
   struct QGL_GRAPHICS_API VERTEX_DESC_HEADER final
   {
      public:
      VERTEX_DESC_HEADER();

      VERTEX_DESC_HEADER(const VERTEX_DESC_HEADER&) = default;

      VERTEX_DESC_HEADER(VERTEX_DESC_HEADER&&) = default;

      ~VERTEX_DESC_HEADER() noexcept = default;

      inline friend void swap(VERTEX_DESC_HEADER& l,
                              VERTEX_DESC_HEADER& r) noexcept
      {
         using std::swap;
         swap(l.Elements, r.Elements);
         swap(l.Topology, r.Topology);
         swap(l.StripCut, r.StripCut);
         swap(l.Reserved1, r.Reserved1);
         swap(l.Reserved2, r.Reserved2);
      }

      inline VERTEX_DESC_HEADER& operator=(VERTEX_DESC_HEADER r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      inline friend bool operator==(const VERTEX_DESC_HEADER& l,
                                    const VERTEX_DESC_HEADER& r) noexcept
      {
         return l.Elements == r.Elements &&
            l.Topology == r.Topology &&
            l.StripCut == r.StripCut;
      }

      uint8_t Elements;
      uint8_t Topology;
      uint8_t StripCut;
      uint8_t Reserved1;
      uint32_t Reserved2;
   };

   static_assert(sizeof(VERTEX_DESC_HEADER) == 8,
                 "VERTEX_DESC_HEADER should be 8 bytes.");
}