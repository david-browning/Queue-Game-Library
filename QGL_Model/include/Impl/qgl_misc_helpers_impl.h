#pragma once
#include "include/qgl_model_include.h"

namespace qgl::impl
{
   template<typename BaseT, typename ExponentT>
   constexpr BaseT power(BaseT base, ExponentT exponent, BaseT result)
   {
      auto two = static_cast<ExponentT>(2);
      return exponent < 1 ? 
         result : 
         qgl::impl::power(base * base, exponent / two, 
            (exponent % two) ? result * base : result);
   }

   static_assert(8 == power(2, 3, 1), "Power function is not correct.");
   static_assert(1000 == power(10, 3, 1), "Power function is not correct.");
   static_assert(81 == power(3, 4, 1), "Power function is not correct.");

   template<
      class ExecutionPolicy,
      class BidirIt1>
      void alternate(
         ExecutionPolicy&& policy,
         BidirIt1 first,
         BidirIt1 last,
         std::bidirectional_iterator_tag)
   {
      auto elements = std::distance(first, last);
      if (elements < 3)
      {
         return;
      }

      // Figure out the types
      using itRefType = decltype(*first);
      using itType = typename std::remove_reference<itRefType>::type;

      static_assert(std::is_trivially_copy_constructible<itType>::value,
         "Iterators must point to a trivial copyable type.");

      static_assert(std::is_swappable<itType>::value,
         "Iterators must point to a swappable type.");

      // Merge the iterator range into a new array.
      std::vector<itType> merged;
      auto firstCopy = first;
      auto lastCopy = last - 1;
      while (firstCopy < lastCopy)
      {
         merged.push_back(*firstCopy);
         merged.push_back(*lastCopy);
         lastCopy--;
         firstCopy++;
      }

      if (elements % 2 != 0)
      {
         merged.push_back(*firstCopy);
      }

      // Swap the old range with the merged range.
      std::swap_ranges(policy, first, last, merged.begin());
         
      return;
   }
}