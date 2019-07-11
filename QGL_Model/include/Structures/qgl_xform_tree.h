#pragma once
#include "include/qgl_model_include.h"
#include "include/Structures/qgl_basic_tree_map.h"

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
    Transforms a tree who's operations cascade down the tree. This is
    most useful for scene graphs where each child's transform is a function of
    their parent's transform. Each node in the tree can also be reset so the
    transform is no longer applied.
    TreeT: Type of tree. Must have TreeT::node_type.
    BinaryOperation: A binary operation function that will be applied. The
     signature of the function should be equivalent to this:
     ValueT fun(ValueT parentValue, ValueT nodeValue);
     The result of this binary operation overrides the node's data that the
     operation was applied to.
     While the binary operation is meant to work with any data type, its
     primary use is to apply vector and matrix transformations. These data
     types can be passed using SSE registers so the binary operation's
     operators are not reference types.
    */
   template<class TreeT, class BinaryOperation>
   void xform(TreeT& tree, BinaryOperation op)
   {
       //First item is the node's parent. Second is the node to xform.
      using t = std::pair<const TreeT::node_type&, TreeT::node_type&>;

      //Enqueue the root's children.
      std::queue<t> toXFrom;
      for (auto& child : tree.root_node())
      {
         toXFrom.emplace(tree.root_node(),
                         child.second);
      }

      while (!toXFrom.empty())
      {
         auto& cur = toXFrom.front();
         cur.second.mapped() = op(cur.first.mapped(),
                                  cur.second.mapped());

         for (auto& child : cur.second)
         {
            toXFrom.emplace(&cur.second, child);
         }

         toXFrom.pop();
      }
   }
}