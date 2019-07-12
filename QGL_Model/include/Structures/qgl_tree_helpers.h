#pragma once

namespace qgl::tree
{
   template<typename IndexT>
   constexpr IndexT parent_index(const IndexT i)
   {
      return (i - 1) / 2;
   }

   template<typename IndexT>
   constexpr IndexT left_index(const IndexT i)
   {
      return 2 * i + 1;
   }

   template<typename IndexT>
   constexpr IndexT right_index(const IndexT i)
   {
      return 2 * i + 2;
   }
}