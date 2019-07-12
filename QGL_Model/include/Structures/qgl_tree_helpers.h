#pragma once

namespace qgl::tree
{
   /*
    Given a k-d tree stored in a pre-order, returns the index of the i'th 
    child of the parent node. The first child's index is 1 and the k'th 
    child's index is k.
    */
   template<typename IndexT, IndexT K>
   constexpr IndexT kd_ichild(const IndexT parent, const IndexT ith)
   {
      return K * parent + ith;
   }

   /*
    Given a k-d tree stored in a pre-order, returns the index of the node's 
    parent node.
    */
   template<typename IndexT, IndexT K>
   constexpr IndexT kd_parent(const IndexT node)
   {
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
      return kd_ichild<IndexT, 2>(i, 1);
   }

   /*
    Specialization of kd_ichild for a binary tree.
    */
   template<typename IndexT>
   constexpr IndexT right_index(const IndexT i)
   {
      return kd_ichild<IndexT, 2>(i, 2);
   }
}