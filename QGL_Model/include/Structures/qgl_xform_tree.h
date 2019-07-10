#pragma once
#include "include/qgl_model_include.h"
#include "include/Structures/qgl_unordered_tree_map.h"

namespace qgl
{
   #ifdef DEBUG
   class test_xform_func
   {
      public:
      DirectX::XMMATRIX XM_CALLCONV operator()(DirectX::FXMMATRIX parent,
                                               DirectX::CXMMATRIX node)
      {
         return DirectX::XMMatrixMultiply(parent, node);
      }
   };
   #endif

   /*
    An xform_tree is a tree who's operations cascade down the tree. This is
    most useful for scene graphs where each child's transform is a function of
    their parent's transform. Each node in the tree can also be reset so the
    transform is no longer applied.
    KeyT: Key used to look up nodes.
    ValueT: Value that gets stored in each tree node.
    BinaryOperand1: Must be implicitly convertible to ValueT.
    BinaryOperand2: Must be implicitly convertible to ValueT.
    BinaryOperation: A binary operation function that will be applied. The
     signature of the function should be equivalent to this:
     ValueT fun(ValueT parentValue, BinaryOperand2 nodeValue);
     The result of this binary operation overrides the node's data that the
     operation was applied to.
     While the binary operation is meant to work with any data type, its
     primary use is to apply vector and matrix transformations. These data
     types can be passed using SSE registers so the binary operation's
     operators are not reference types.
    */
   template<
      class KeyT,
      class ValueT,
      class BinaryOperation,
      class BinaryOperand1 = ValueT,
      class BinaryOperand2 = ValueT>
   class xform_tree : public qgl::unordered_tree_map<KeyT, ValueT>
   {
      public:
      xform_tree(const KeyT& rootKey, 
                 const ValueT& rootVal,
                 BinaryOperation op = BinaryOperation()) :
         unordered_tree_map<KeyT, ValueT>(rootKey, rootVal),
         m_op(op)
      {

      }

      xform_tree(const KeyT&& rootKey,
                 const ValueT&& rootVal,
                 BinaryOperation op = BinaryOperation()) :
         unordered_tree_map<KeyT, ValueT>(rootKey, rootVal),
         m_op(op)
      {

      }

      xform_tree(const xform_tree&) = default;

      xform_tree(xform_tree&&) = default;

      ~xform_tree() noexcept = default;

      /*
       Applies the root's transform to all children.
       Since all elements must be accessed, the complexity is O(n) where n is 
       the number of nodes.
       */
      void xform()
      {
         //First item is the node's parent. Second is the node to xform.
         using t = std::pair<const xform_tree::node_type*, 
            xform_tree::node_type&>;

         //Enqueue the root's children.
         std::queue<t> toXFrom;
         const auto rootPtr = &xform_tree::root_node();
         for (auto& child : xform_tree::root_node())
         {
            toXFrom.emplace(rootPtr,
                            child.second);
         }

         while (!toXFrom.empty())
         {
            auto& cur = toXFrom.front();
            cur.second.mapped() = m_op(cur.first->mapped(),
                                       cur.second.mapped());

            for (auto& child : cur.second)
            {
               toXFrom.emplace(&cur.second, child);
            }

            toXFrom.pop();
         }
      }

      private:
      BinaryOperation m_op;
   };
}