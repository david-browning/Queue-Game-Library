#pragma once
#include "include/qgl_model_include.h"
#include "include/Impl/qgl_misc_helpers_impl.h"
#include "include/Impl/fast_hash_impl.h"
#include <execution>
#include <gamingdeviceinformation.h>

namespace qgl
{
   namespace tree
   {
      /*
       Given a k-d tree stored in a pre-order, returns the index of the i'th
       child of the parent node. The first child's index is 1 and the k'th
       child's index is k.
       */
      template<typename IndexT, IndexT K>
      constexpr IndexT kd_ichild(const IndexT parent, const IndexT ith)
      {
         static_assert(K != 0, "K cannot be zero.");
         return K * parent + ith;
      }

      /*
       Given a k-d tree stored in a pre-order, returns the index of the node's
       parent node.
       K cannot be 0.
       */
      template<typename IndexT, IndexT K>
      constexpr IndexT kd_parent(const IndexT node)
      {
         static_assert(K != 0, "K cannot be zero.");
         return (node - 1) / K;
      }

      /*
       Specialization of kd_parent for a binary tree.
       */
      template<typename IndexT>
      constexpr IndexT parent_index(const IndexT i)
      {
         return kd_parent<IndexT, 2>(i);
      }

      /*
       Specialization of kd_ichild for a binary tree.
       */
      template<typename IndexT>
      constexpr IndexT left_index(const IndexT i)
      {
         return kd_ichild<IndexT, 2>(i, static_cast<IndexT>(1));
      }

      /*
       Specialization of kd_ichild for a binary tree.
       */
      template<typename IndexT>
      constexpr IndexT right_index(const IndexT i)
      {
         return kd_ichild<IndexT, 2>(i, static_cast<IndexT>(2));
      }
   }

   /*
    Alternates the items in a sorted list such that the first element is the
    largest and second element is the smallest.
    { 0, 1, 2, 3, 4, 5 } -> { 0, 5, 1, 4, 2, 3 }

    If specifying an execution policy, the iterators should point to a type
    that is move constructable.
    */
   template<
      class ExecutionPolicy,
      class BidirIt1>
   void alternate(ExecutionPolicy&& policy, BidirIt1 first, BidirIt1 last)
   {
      impl::alternate(policy, first, last,
         typename std::iterator_traits<BidirIt1>::iterator_category());
   }

   template<class BidirIt1>
   void alternate(BidirIt1 first, BidirIt1 last)
   {
      impl::alternate(std::execution::seq, first, last,
         typename std::iterator_traits<BidirIt1>::iterator_category());
   }

   /*
    64-bit hash adapted from Zilong Tan.
    https://github.com/ztanml/fast-hash
    */
   template<typename T>
   constexpr uint64_t fast_hash(
      const T* buffer,
      size_t length,
      uint64_t seed)
   {
      return impl::fast_hash_64(buffer, length, seed);
   }

   template<typename T>
   constexpr uint32_t fast_hash_32(
      const T* buffer,
      size_t length,
      uint32_t seed)
   {
      return impl::fast_hash_32(buffer, length, seed);
   }

#ifndef WIN32
   typedef enum GAMING_DEVICE_DEVICE_ID
   {
      GAMING_DEVICE_DEVICE_ID_NONE = 0,
      GAMING_DEVICE_DEVICE_ID_XBOX_ONE = 0x768BAE26,
      GAMING_DEVICE_DEVICE_ID_XBOX_ONE_S = 0x2A7361D9,
      GAMING_DEVICE_DEVICE_ID_XBOX_ONE_X = 0x5AD617C7,
      GAMING_DEVICE_DEVICE_ID_XBOX_ONE_X_DEVKIT = 0x10F7CDE3
   } GAMING_DEVICE_DEVICE_ID;
#endif

   enum class hw_types : int32_t
   {
      hw_type_error = -1,
      hw_type_pc = GAMING_DEVICE_DEVICE_ID_NONE,
      hw_type_xbox_one = GAMING_DEVICE_DEVICE_ID_XBOX_ONE,
      hw_type_xbox_one_s = GAMING_DEVICE_DEVICE_ID_XBOX_ONE_S,
      hw_type_xbox_one_x = GAMING_DEVICE_DEVICE_ID_XBOX_ONE_X,
      hw_type_xbox_one_x_devkit = GAMING_DEVICE_DEVICE_ID_XBOX_ONE_X_DEVKIT,
   };

   /*
    Returns the type of hardware the engine is running on.
    */
   extern "C" QGL_MODEL_API hw_types QGL_CC hw_type() noexcept;
}