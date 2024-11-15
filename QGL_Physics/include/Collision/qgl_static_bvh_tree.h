#pragma once
#include "include/qgl_physics_include.h"
#include "include/Collision/qgl_collision_adapters.h"
#include "include/Collision/qgl_bvh_tree_builder.h"
#include <stack>
#include <QGLStruct.h>
#include <QGLModel.h>

namespace qgl::physics
{
   /*
    This compressed, cache-coherent tree is optimized for look ups.
    However, once constructed, nodes cannot be added or removed.
    */
   template<class BoundingVolume, class Intersects, class Cost, class Union>
   class static_bvh_tree
   {
      public:
      class node_t
      {
         public:
         node_t()
         {

         }

         node_t(const BoundingVolume& v) :
            m_volume(v)
         {

         }

         BoundingVolume& volume() noexcept
         {
            return m_volume;
         }

         const BoundingVolume& volume() const noexcept
         {
            return m_volume;
         }

         int& height() noexcept
         {
            return m_height;
         }

         const int& height() const noexcept
         {
            return m_height;
         }

         bool leaf() const noexcept
         {
            return height() == 0;
         }

         friend class static_bvh_tree;

         private:
         BoundingVolume m_volume;
         int m_height = 0;
      };

      static_bvh_tree(const bvh_tree_builder<BoundingVolume, Intersects, Cost, Union>& bldr,
                      Intersects intFunc = Intersects(),
                      Cost costFunc = Cost(),
                      Union unionFunc = Union()) :
         m_intersectsOp(intFunc),
         m_costOp(costFunc),
         m_unionOp(unionFunc)
      {
         auto it = bldr.cbegin();
         while (it != bldr.cend())
         {
            insert(*it);
            ++it;
         }
      }

      /*
       Creates a BVH tree from the supplied bounding volumes.
       This static tree does not do any rotations. Depending on the order of
       [first, last), this may result in a very unbalanced tree.
       */
      template<class InputIterator>
      static_bvh_tree(InputIterator first,
                      InputIterator last,
                      Intersects intFunc = Intersects(),
                      Cost costFunc = Cost(),
                      Union unionFunc = Union()) :
         m_intersectsOp(intFunc),
         m_costOp(costFunc),
         m_unionOp(unionFunc)
      {
         using iterator_type = decltype(*first);
         static_assert(
            std::is_same<
               BoundingVolume,
               std::remove_reference<iterator_type>::type>::value,
            "InputIterator does not point to the same type as BoundingVolume.");

         while (first != last)
         {
            insert(*first);
            first++;
         }
      }

      static_bvh_tree(const static_bvh_tree&) = default;

      static_bvh_tree(static_bvh_tree&&) = default;

      ~static_bvh_tree() noexcept = default;

      /*
       Returns true if the tree is empty.
       */
      [[nodiscard]] bool empty() const noexcept
      {
         return m_nodes.empty();
      }

      /*
       Returns the height of the tree. The height of an empty tree is -1. The
       height of a tree with only one node is 0.
       */
      int height() const noexcept
      {
         if (empty())
         {
            return -1;
         }

         return m_nodes.front().height;
      }

      /*
       Determines if the given bounding volume "v" intersects a bounding volume
       in this tree. If the tree has a leaf that intersects with the given
       bounding volume, this raises the given callback, passing it a const-
       reference to the bounding volume "v" intersects with. This callback may
       be raised multiple times if "v" intersects with multiple leaf nodes.
       The callback can return "true" to instruct this function call to stop.
       Use this if you are only interested in getting the first bounding volume.
       O(log(n))
       */
      template<class Callback>
      bool intersects(const BoundingVolume& v,
                      Callback cb = Callback()) const noexcept
      {
         return intscts(v, cb);
      }

      private:
      using pos_t = typename int;
      static constexpr pos_t NO_NODE = -1;
      using container = typename std::vector<node_t>;
      using iterator = typename container::iterator;
      using const_iterator = typename container::const_iterator;

      /*
       Inserts the bounding volume into the tree.
       */
      void insert(const BoundingVolume& v) noexcept
      {
         // If the tree is empty.
         if (empty())
         {
            // Insert this volume as the first element.
            m_nodes.emplace_back(v);
         }

         // Find the best sibling.
         auto sib_i = bestSibling(v);

         // Create a parent node that contains the new volume "v" and the 
         // sibling. Height is 1 since it has children.
         node_t newParent{ m_unionOp(v, m_nodes[sib_i].volume()) };
         newParent.height() = 1;
         auto parent_i = sib_i;

         // Create a new child node for the volume.
         node_t newV{ v };

         // Allocate nodes and put the children into the tree.
         auto left_i = alloc_node(left(parent_i));
         auto right_i = alloc_node(right(parent_i));
         m_nodes[left_i] = m_nodes[sib_i];
         m_nodes[right_i] = std::move(newV);
         m_nodes[parent_i] = std::move(newParent);

         // Fix up the tree heights and volumes.
         int lastHeight = 1;
         auto curNode_i = parent(parent_i);
         while (curNode_i != NO_NODE)
         {
            lastHeight++;
            auto& n = m_nodes[curNode_i];
            n.height() = std::max<int>(lastHeight, n.height());
            n.volume() = m_unionOp(m_nodes[left(curNode_i)].volume(),
                                   m_nodes[right(curNode_i)].volume());
            curNode_i = parent(curNode_i);
         }
      }

      pos_t left(pos_t i) const noexcept
      {
         return tree::left_index(i);
      }

      pos_t right(pos_t i) const noexcept
      {
         return tree::right_index(i);
      }

      pos_t parent(pos_t i) const noexcept
      {
         if (i == 0 || i == NO_NODE)
         {
            return NO_NODE;
         }

         return tree::parent_index(i);
      }

      const_iterator it(pos_t i) const noexcept
      {
         return m_nodes.begin() + i;
      }

      bool leaf(pos_t i) const noexcept
      {
         return m_nodes[i].leaf();
      }

      pos_t bestSibling(const BoundingVolume& v) const noexcept
      {
         assert(!empty());

         pos_t cur_i = 0;
         while (!leaf(cur_i))
         {
            // Since the tree is full, non-leaf nodes will always have a right
            // and left child.

            // Cost of inserting the volume to the left.
            auto leftCost = m_costOp(m_unionOp(
               v, m_nodes[left(cur_i)].volume()));

            //Cost of inserting the volume to the right.
            auto rightCost = m_costOp(m_unionOp(
               v, m_nodes[right(cur_i)].volume()));

            if (leftCost < rightCost)
            {
               cur_i = left(cur_i);
            }
            else
            {
               cur_i = right(cur_i);
            }
         }

         return cur_i;
      }

      pos_t alloc_node(pos_t p)
      {
         // If p <= node count, then no need to resize
         if (p > m_nodes.size())
         {
            m_nodes.resize(m_nodes.size() * 2 + 1);
         }

         return p;
      }

      template<class Callback>
      bool intscts(const BoundingVolume& v, Callback cb) const noexcept
      {
         if (empty())
         {
            return false;
         }

         bool ret = false;
         std::stack<pos_t> toCheck;
         toCheck.push(0);

         while (!toCheck.empty())
         {
            auto pos = toCheck.top();
            toCheck.pop();

            const auto& volume = m_nodes[pos].volume();
            if (m_intersectsOp(v, volume))
            {
               if (leaf(pos))
               {
                  ret = true;
                  // Leaf means we've found the intersection.
                  if (cb(volume))
                  {
                     return true;
                  }
               }
               else
               {
                  toCheck.push(left(pos));
                  toCheck.push(right(pos));
               }
            }
         }

         return ret;
      }

      container m_nodes;
      Cost m_costOp;
      Union m_unionOp;
      Intersects m_intersectsOp;
   };
}