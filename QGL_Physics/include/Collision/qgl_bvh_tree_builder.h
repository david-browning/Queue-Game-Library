#pragma once
#include "include/qgl_physics_include.h"
#include "include/Collision/qgl_collision_adapters.h"
#include <stack>

namespace qgl::physics
{
   template<class BoundingVolume, class Intersects, class Cost, class Union>
   class bvh_tree_builder
   {
      public:
      class bvh_tree_iterator;
      using iterator = typename bvh_tree_iterator;
      using const_iterator = typename bvh_tree_iterator;
      using node_ptr = typename int;
      static constexpr node_ptr NO_NODE = -1;

      class node_t
      {
         public:
         node_t()
         {

         }

         node_t(const BoundingVolume& v, node_ptr parent = NO_NODE) :
            m_volume(v),
            parent_p(parent)
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

         friend class bvh_tree_builder;

         private:
         BoundingVolume m_volume;
         int m_height = 0;
         node_ptr left_p = NO_NODE;
         node_ptr right_p = NO_NODE;
         node_ptr parent_p = NO_NODE;
      };

      /*
       Iterates through tree nodes in breadth-first order.
       */
      class bvh_tree_iterator
      {
         public:
         bvh_tree_iterator() : m_curIdx(NO_NODE)
         {

         }

         bvh_tree_iterator(node_ptr rootIdx, const std::vector<node_t>& nodes) :
            m_curIdx(0)
         {
            if (nodes.empty())
            {
               m_curIdx = NO_NODE;
            }
            else
            {
               std::queue<node_ptr> toProcess;
               toProcess.push(rootIdx);

               while (!toProcess.empty())
               {
                  auto cur = toProcess.front();
                  toProcess.pop();
                  auto& n = nodes[cur];
                  m_volumes.push_back(n.volume());
                  toProcess.push(n.left_p);
                  toProcess.push(n.right_p);
               }
            }
         }

         BoundingVolume& operator*() noexcept
         {
            assert(m_curIdx >= 0 && m_curIdx < m_volumes.size());
            return m_volumes[m_curIdx];
         }

         const BoundingVolume& operator*() const noexcept
         {
            assert(m_curIdx >= 0 && m_curIdx < m_volumes.size());
            return m_volumes[m_curIdx];
         }

         bvh_tree_iterator& operator++() noexcept
         {
            assert(m_curIdx >= 0 && m_curIdx < m_volumes.size());
            m_curIdx++;
            if (m_curIdx == m_volumes.size())
            {
               m_curIdx = NO_NODE;
            }
         }

         bool operator!=(const bvh_tree_iterator& r) const noexcept
         {
            return m_curIdx != r.m_curIdx;
         }

         private:
         int m_curIdx;
         std::vector<BoundingVolume> m_volumes;
      };

      bvh_tree_builder(Intersects intFunc = Intersects(),
                       Cost costFunc = Cost(),
                       Union unionFunc = Union()) :
         m_intersectsOp(intFunc),
         m_costOp(costFunc),
         m_unionOp(unionFunc)
      {

      }

      /*
       Creates a BVH tree from the supplied bounding volumes.
       This will rebalance the tree as items are inserted.
       */
      template<class InputIterator>
      bvh_tree_builder(InputIterator first,
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

      bvh_tree_builder(const bvh_tree_builder&) = default;

      bvh_tree_builder(bvh_tree_builder&&) = default;

      ~bvh_tree_builder() noexcept = default;

      /*
       Returns true if the tree is empty.
       */
      [[nodiscard]] bool empty() const noexcept
      {
         return m_parentIdx == NO_NODE;
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

         return m_nodes[m_parentIdx].height();
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

      /*
       Inserts the bounding volume into the tree.
       */
      void insert(const BoundingVolume& v) noexcept
      {
         m_parentIdx = ins(v);
      }

      /*
       Removes the node, and all of its children, at position "node".
       */
      void erase(iterator node) noexcept
      {
         m_parentIdx = erse(node);
      }

      /*
       Erases all elements from the tree.
       */
      void clear() noexcept
      {
         m_parentIdx = NO_NODE;
         m_freeNodes.clear();
         m_nodes.clear();
      }

      const_iterator cbegin() const noexcept
      {
         return bvh_tree_iterator{ m_parentIdx, m_nodes };
      }

      const_iterator cend() const noexcept
      {
         return bvh_tree_iterator{};
      }

      private:
      bool leaf(node_ptr n) const noexcept
      {
         return m_nodes[n].height() == 0;
      }

      template<class Callback>
      bool intscts(const BoundingVolume& v, Callback cb) const noexcept
      {
         if (empty())
         {
            return false;
         }

         bool ret = false;
         std::stack<node_ptr> toCheck;
         toCheck.push(m_parentIdx);

         while (!toCheck.empty())
         {
            auto p = toCheck.top();
            toCheck.pop();

            const auto& n = m_nodes[p];
            if (m_intersectsOp(v, n.volume))
            {
               if (n.leaf())
               {
                  ret = true;
                  if (cb(n.volume))
                  {
                     return true;
                  }
               }
               else
               {
                  toCheck.push(n.left_p);
                  toCheck.push(n.right_p);
               }
            }
         }

         return ret;
      }

      /*
       Inserts the volume and returns the new index of the root node that 
       points to the newly inserted item.
       */
      node_ptr ins(const BoundingVolume& v) noexcept
      {
         if (empty())
         {
            m_nodes.emplace_back(v);
            return 0;
         }

         // Find the best sibling.
         auto sibPtr = best_sibling(v);

         // Allocate nodes for the new parent's children.
         auto leftPtr = alloc_node();
         auto rightPtr = alloc_node();

         // Create a parent node that contains the new volume "v" and the 
         // sibling. Height is 1 since it has children.
         node_t newParent;
         newParent.left_p = leftPtr;
         newParent.right_p = rightPtr;
         newParent.parent_p = m_nodes[sibPtr].parent_p;
         auto parentPtr = sibPtr;

         // Create a new child node for the volume.
         node_t newV{ v };
         newV.parent_p = parentPtr;

         // Put the nodes into the tree
         m_nodes[leftPtr] = m_nodes[sibPtr];
         m_nodes[leftPtr].parent_p = parentPtr;
         m_nodes[rightPtr] = std::move(newV);
         m_nodes[parentPtr] = std::move(newParent);

         // Fix up the tree heights and volumes.
         int lastHeight = 1;
         auto curNode_p = parentPtr;
         while (curNode_p != NO_NODE)
         {
            auto& n = m_nodes[curNode_p];
            auto leftPtr = n.left_p;
            auto rightPtr = n.right_p;

            n.height() = std::max<int>(lastHeight, n.height());
            n.volume() = m_unionOp(m_nodes[leftPtr].volume(),
                                   m_nodes[rightPtr].volume());

            // Fix up the tree if its unbalanced.
            auto b = balance(curNode_p);
            if (b > 1)
            {
               // Tree's left side is taller than its right.
               curNode_p = rrotate(curNode_p, parentPtr);
            }
            else if (b < -1)
            {
               // Tree's right side is taller than its left.
               curNode_p = lrotate(curNode_p, parentPtr);
            }

            curNode_p = m_nodes[curNode_p].parent_p;
            lastHeight++;
         }
      }

      node_ptr erse(iterator node) noexcept;

      node_ptr best_sibling(const BoundingVolume& v) const noexcept
      {
         assert(!empty());

         node_ptr p = m_parentIdx;
         while (!leaf(p))
         {
            // Since the tree is full, non-leaf nodes will always have a right
            // and left child.
            const auto& n = m_nodes[p];
            const auto& leftV = m_nodes[n.left_p].volume();
            const auto& rightV = m_nodes[n.right_p].volume();

            // Cost of inserting the volume to the left.
            auto leftCost = m_costOp(m_unionOp(v, leftV));

            //Cost of inserting the volume to the right.
            auto rightCost = m_costOp(m_unionOp(v, rightV));

            if (leftCost < rightCost)
            {
               p = n.left_p;
            }
            else
            {
               p = n.right_p;
            }
         }

         return p;
      }

      node_ptr alloc_node() noexcept
      {
         if (m_freeNodes.empty())
         {
            auto oldSize = m_nodes.size();
            auto newSize = oldSize * 2 + 1;
            m_nodes.resize(newSize);

            while (oldSize < newSize)
            {
               m_freeNodes.push_back(oldSize);
               oldSize++;
            }
         }

         auto ret = m_freeNodes.front();
         m_freeNodes.pop_front();
         return ret;
      }

      auto balance(node_ptr p) const noexcept
      {
         if (p == NO_NODE)
         {
            return 0;
         }

         const auto& n = m_nodes[p];
         auto lHeight = m_nodes[n.left_p].height();
         auto rHeight = m_nodes[n.right_p].height();
         return lHeight - rHeight;
      }

      /*
       Rotates parent "p" so that it's child nodes are balanced after the "i"
       right grandchild was inserted. This is triggered when the balance factor
       is greater than 1.
       */
      node_ptr rrotate(node_ptr p, node_ptr i) noexcept
      {
         using std::swap;
         auto unbalanced_p = m_nodes[p].left_p;

         // If the tree is left-left unbalanced:
         if (leaf(m_nodes[unbalanced_p].right_p))
         {
            //       1
            //      / \
            //     2   B
            //    / \
            //   3   C
            //  / \
            // A   D
            swap(m_nodes[p].right_p, m_nodes[unbalanced_p].left_p);
         }
         else
         {
            //     1
            //    / \
            //   2   B
            //  / \
            // A   3
            //    / \
            //   C   D
            swap(m_nodes[p].right_p, m_nodes[unbalanced_p].right_p);
         }

         // Fix heights and volumes

         return p;
      }

      /*
       Rotates parent "p" so that it's child nodes are balanced after the "i"
       left grandchild was inserted. This is triggered when the balance factor
       is less than -1.
       */
      node_ptr lrotate(node_ptr p, node_ptr i) noexcept
      {
         using std::swap;
         auto unbalanced_p = m_nodes[p].right_p;

         // If the tree is right-right unbalanced:
         if (leaf(m_nodes[unbalanced_p].left_p))
         {
            //   1
            //  / \
            // A   2
            //    / \
            //   B   3
            //       / \
            //      C   D   
            swap(m_nodes[p].left_p, m_nodes[unbalanced_p].right_p);
         }
         else
         {
            //   1
            //  / \
            // A   2
            //    / \
            //   3   B
            //  / \
            // C   D   
            swap(m_nodes[p].left_p, m_nodes[unbalanced_p].left_p);
         }

         // Fix heights and volumes

         return p;
      }

      std::vector<node_t> m_nodes;
      std::list<node_ptr> m_freeNodes;
      node_ptr m_parentIdx = NO_NODE;
      Cost m_costOp;
      Union m_unionOp;
      Intersects m_intersectsOp;
   };
}