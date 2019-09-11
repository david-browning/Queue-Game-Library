#pragma once
#include "include/qgl_physics_include.h"
using namespace qgl::math;

namespace qgl::physics::collision
{
   #ifdef DEBUG
   /*
    Modifies a bounding sphere to be slightly larger.
    */
   class test_sphere_fatten
   {
      public:
      void operator()(DirectX::BoundingSphere& toFatten)
      {
         toFatten.Radius *= FatMultiplier;
      }

      float FatMultiplier = 1.05f;
   };

   /*
    Returns the volume of the given bounding sphere.
    */
   class test_volume
   {
      public:
      float operator()(const DirectX::BoundingSphere& bounding)
      {
         auto r3 = std::pow(bounding.Radius, 3.0f);
         return r3 * (4.0f / 3.0f) * math::const_pi<float>;
      }
   };

   /*
    Returns true if two bounding spheres intersect.
    */
   class test_intersects
   {
      public:
      bool operator()(const DirectX::BoundingSphere& child,
                      const DirectX::BoundingSphere& toCheck) const
      {
         return toCheck.Intersects(child);
      }
   };

   /*
    Creates a bounding sphere that contains the two bounding spheres.
    */
   class test_bound
   {
      public:
      DirectX::BoundingSphere operator()(const DirectX::BoundingSphere& v1,
                                         const DirectX::BoundingSphere& v2)
      {
         using namespace DirectX;
         XMVECTOR v1Center = XMLoadFloat3(&v1.Center);
         XMVECTOR v2Center = XMLoadFloat3(&v2.Center);
         auto center = (v1Center + v2Center) / 2.0f;

         auto distanceBetween = v1Center - v2Center;
         auto diameter = v1.Radius + v2.Radius +
            XMVectorGetByIndex(XMVector3Length(distanceBetween), 0);
         auto r = diameter / 2.0f;

         XMFLOAT3 f3Center;
         XMStoreFloat3(&f3Center, center);
         return BoundingSphere(f3Center, r);
      }
   };

   class sphere_equal
   {
      public:
      bool operator()(const DirectX::BoundingSphere& v1,
                      const DirectX::BoundingSphere& v2) const noexcept
      {
         return v1.Radius == v2.Radius &&
            v1.Center.x == v2.Center.x &&
            v1.Center.y == v2.Center.y &&
            v1.Center.z == v2.Center.z;
      }
   };

   class test_intersect_callback
   {
      public:
      bool operator()(const DirectX::BoundingSphere& v)
      {
         return true;
      }
   };

   #endif

   template<
      class BoundingVolume,
      class Intersects,
      class Cost,
      class Union>
      class bvh_tree
   {
      public:
      using node_id = typename int;
      using volume_type = typename BoundingVolume;
      using cost_func = typename Cost;
      using union_func = typename Union;
      using intersect_func = typename Intersects;

      static const node_id NO_NODE = -1;

      class node_type
      {
         public:

         /*
          Default constructor means this is free and has no children.
          */
         node_type() :
            m_leftIdx(NO_NODE),
            m_rightIdx(NO_NODE),
            m_height(0)
         {

         }

         /*
          Creates a node.
          */
         node_type(const volume_type& v) :
            m_leftIdx(NO_NODE),
            m_rightIdx(NO_NODE),
            m_height(0),
            m_volume(v)
         {

         }

         /*
          Creates a node with its right and left children set.
          Sets the height to 1.
          */
         node_type(const volume_type& v,
                   node_id leftID,
                   node_id rightID) :
            m_leftIdx(leftID),
            m_rightIdx(rightID),
            m_height(1),
            m_volume(v)
         {

         }

         /*
          Copy constructor
          */
         node_type(const node_type&) = default;

         /*
          Move constructor
          */
         node_type(node_type&&) = default;

         /*
          Destructor
          */
         ~node_type() = default;

         inline friend void swap(node_type& l, node_type& r) noexcept
         {
            using std::swap;
            swap(l.m_volume, r.m_volume);
            swap(l.m_height, r.m_height);
            swap(l.m_rightIdx, r.m_rightIdx);
            swap(l.m_leftIdx, r.m_leftIdx);
         }

         inline node_type& operator=(node_type r) noexcept
         {
            swap(*this, r);
            return *this;
         }

         /*
          Returns a reference to this node's bounding volume.
          */
         volume_type& volume() noexcept
         {
            return m_volume;
         }

         /*
          Returns a const reference to this node's bounding volume.
          */
         const volume_type& volume() const noexcept
         {
            return m_volume;
         }

         /*
          Returns true if this node has no children.
          */
         bool leaf() const noexcept
         {
            return m_height == 0;
         }

         /*
          Returns a reference to this node's height.
          */
         size_t& height() noexcept
         {
            return m_height;
         }

         /*
          Returns a const reference to this node's height.
          */
         const size_t& height() const noexcept
         {
            return m_height;
         }


         auto& left() noexcept
         {
            return m_leftIdx;
         }

         /*
          Returns the index of this node's left child.
          */
         auto left() const noexcept
         {
            return m_leftIdx;
         }

         auto& right() noexcept
         {
            return m_rightIdx;
         }

         /*
          Returns the index of this node's right child.
          */
         auto right() const noexcept
         {
            return m_rightIdx;
         }

         private:
         volume_type m_volume;
         size_t m_height;
         node_id m_leftIdx;
         node_id m_rightIdx;
      };

      using free_list = typename std::stack<node_id>;
      using container = typename std::vector<node_type>;
      using iterator = typename container::iterator;
      using const_iterator = typename container::const_iterator;

      #pragma region Constructors
      bvh_tree(intersect_func intFunc = intersect_func(),
               cost_func costFunc = cost_func(),
               union_func unionFunc = union_func()) :
         m_intersectsOp(intFunc),
         m_costOp(costFunc),
         m_unionOp(unionFunc)
      {
      }

      bvh_tree(const bvh_tree&) = default;

      bvh_tree(bvh_tree&&) = default;

      ~bvh_tree() noexcept = default;
      #pragma endregion


      #pragma region Modifiers
      /*
       Inserts the bounding volume into the tree.
       Returns an iterator to the inserted node.
       */
      const_iterator insert(const volume_type& v)
      {
         //If the tree is empty:
         if (empty())
         {
            //Just insert this into the root. The root has node_id 0.
            m_nodes.emplace_back(v);
         }

         //Find the best sibling node.
         auto sibIt = best_sibling(v);
         node_id sibID = sibIt - begin();
         auto sibNode = *sibIt;

         //Allocate a node to put v into.
         auto vID = alloc_node();
         auto vIt = begin() + alloc_node();
         *vIt = node_type(v);

         //Create a parent node that contains the new volume of v and the 
         //sibling.
         auto parentID = alloc_node();
         auto newParentIt = begin() + parentID;
         node_type newParent(m_unionOp(v, sibNode.volume()),
                             sibID,
                             vID);
         *newParentIt = newParent;

         //Fix up the tree
         refit(parentID);

         return right(it(vID));
      }

      /*
       Removes the node, and all of its children, at position "node".
       */
      void erase(const_iterator node)
      {
         //Do nothing if the tree is empty.
         if (empty())
         {
            return;
         }

         //If erasing the whole tree:
         if (node == begin())
         {
            //Just clear everything out.
            free_list emptyFreeList;
            std::swap(m_freeNodes, emptyFreeList);
            m_nodes.clear();
            return;
         }









      }

      #pragma endregion

      #pragma region Lookup and Query
      /*
       Returns true if the tree is empty.
       */
      [[nodiscard]] bool empty() const noexcept
      {
         return m_nodes.empty();
      }

      /*
       Determines if the given "bounding" volume intersects a bounding volume
       in this tree. If the tree has a leaf that intersects with the given
       bounding volume, this raises the given callback, passing it a const-
       reference to the bounding volume "bounding" intersects with. This
       callback may be raised multiple times. The callback can return "true" to
       instruct this function call to stop. Use this if you are only interested
       in getting the first bounding volume.
       O(log(n))
       */
      template<class Callback>
      void intersects(const volume_type& bounding,
                      Callback cb = Calllback()) const
      {
         if (empty())
         {
            return;
         }

         std::stack<const_iterator> toCheck;
         toCheck.push(cbegin());

         while (!toCheck.empty())
         {
            auto it = toCheck.top();
            toCheck.pop();

            if (m_intersectsOp(bounding, it->volume()))
            {
               if (it->leaf())
               {
                  if (cb(it->volume()))
                  {
                     return;
                  }
               }
               else
               {
                  toCheck.push(left(it));
                  toCheck.push(right(it));
               }
            }
         }
      }

      /*
       Searches the tree for the first occurrence of a volume that is equivalent 
       to v.
       Returns cend() if the tree does not contain v.
       Equivalence is tested using the binary predicate. The binary predicate 
       takes two volume_types as arguments.
       */
      template<class BinaryPredicate>
      const_iterator find(const volume_type& v,
                          BinaryPredicate p = BinaryPredicate()) const
      {
         if (empty())
         {
            return cend();
         }

         std::stack<const_iterator> toCheck;
         toCheck.push(cbegin());

         while (!toCheck.empty())
         {
            auto it = toCheck.top();
            toCheck.pop();

            //If this is a leaf node, check if the predicate is true.
            if (it->leaf())
            {
               if (p(it->volume(), v))
               {
                  //Predicate is true. Found the volume we are searching for.
                  return it;
               }
            }
            else
            {
               //Not a leaf.
               //If this is intersecting with v:
               if (m_intersectsOp(v, it->volume()))
               {
                  //Check the right and left children.
                  toCheck.push(left(it));
                  toCheck.push(right(it));
               }
            }
         }

         return cend();
      }

      #pragma endregion

      #pragma region Iterators
      iterator begin() noexcept
      {
         return m_nodes.begin();
      }

      iterator end() noexcept
      {
         return m_nodes.end();
      }

      const_iterator begin() const noexcept
      {
         return m_nodes.cbegin();
      }

      const_iterator end() const noexcept
      {
         return m_nodes.cend();
      }

      const_iterator cbegin() const noexcept
      {
         return m_nodes.cbegin();
      }

      const_iterator cend() const noexcept
      {
         return m_nodes.cend();
      }
      #pragma endregion

      private:
      #pragma region Private Lookup and Query
      /*
       Returns the iterator to the tree node, given it's ID.
       */
      const_iterator it(node_id id) const
      {
         return cbegin() + id;
      }

      /*
       Returns an iterator to a node's left child. If the node does not have a
       left child, this returns end().
       */
      const_iterator left(const_iterator parent) const noexcept
      {
         auto lIdx = parent->left();
         if (lIdx == NO_NODE)
         {
            return cend();
         }

         return it(lIdx);
      }

      /*
       Returns an iterator to a node's right child. If the node does not have a
       right child, this returns end().
       */
      const_iterator right(const_iterator parent) const noexcept
      {
         auto rIdx = parent->right();
         if (rIdx == NO_NODE)
         {
            return cend();
         }

         return it(rIdx);
      }

      /*
       Returns an iterator to a node's left child. If the node does not have a
       left child, this returns end().
       */
      iterator left(iterator parent) noexcept
      {
         auto lIdx = parent->left();
         if (lIdx == NO_NODE)
         {
            return end();
         }

         return it(lIdx);
      }

      /*
       Returns an iterator to a node's right child. If the node does not have a
       right child, this returns end().
       */
      iterator right(iterator parent) noexcept
      {
         auto rIdx = parent->right();
         if (rIdx == NO_NODE)
         {
            return end();
         }

         return it(rIdx);
      }

      /*
       Returns the iterator to the tree node, given it's ID.
       */
      iterator it(node_id id)
      {
         return begin() + id;
      }

      /*
       Returns an iterator to a leaf node.
       Inserting v next to the returned sibling will yield the lowest cost.
       */
      iterator best_sibling(const volume_type& v)
      {
         auto iter = begin();
         while (!iter->leaf())
         {
            //Since the tree is full, non-leaf nodes will always have a right
            //and left child.

            //Cost of inserting the volume to the left.
            float leftCost = m_costOp(m_unionOp(v, left(iter)->volume()));

            //Cost of inserting the volume to the right.
            float rightCost = m_costOp(m_unionOp(v, right(iter)->volume()));

            if (leftCost < rightCost)
            {
               iter = left(iter);
            }
            else
            {
               iter = right(iter);
            }
         }

         return iter;
      }

      #pragma endregion

      #pragma region Node Allocation and Deallocation
      /*
       Allocates storage for a node by returning the index where the node 
       should be stored. Uses a free list to keep track of nodes that may have 
       been previously freed. If the free list is empty, this will allocate 
       more nodes.
       */
      node_id alloc_node()
      {
         //If the free list is empty:
         if(m_freeNodes.empty())
         {
            //Allocate more nodes.
            auto numNodes = m_nodes.size();
            auto newSize = numNodes * 2 + 1;
            m_nodes.resize(newSize);

            //Populate the free list with the newly allocated node IDs.
            while (newSize > numNodes)
            {
               m_freeNodes.push(--newSize);
            }
         }

         auto ret = m_freeNodes.top();
         m_freeNodes.pop();
         return ret;
      }

      /*
       Marks this node, and its children as free. The freed nodes are returned
       to the free list.
       Keep in mind that this will leave holes in the array of nodes.
       */
      void free_node(node_id nID)
      {
         #ifdef DEBUG
         if (nID >= m_nodes.size())
         {
            throw std::out_of_range("Node ID is out of range.");
         }
         #endif

         auto& n = m_nodes[nID];
         if (!n.leaf())
         {
            free_node(n.left());
            free_node(n.right());
         }

         n.height() = 0;
         n.left() = NO_NODE;
         n.right() = NO_NODE;

         //Push this node last so the stack is in descending order.
         m_freeNodes.push(nID);
      }
      #pragma endregion

      /*
       Returns a node that the parent should be assigned.
       */
      void refit(node_id parentID)
      {

      }

      node_id lrotate(node_id n);

      node_id rrotate(node_id n);

      /*
       Nodes in the tree.
       */
      container m_nodes;

      /*
       List of indices in the tree that are free nodes.
       */
      free_list m_freeNodes;

      cost_func m_costOp;
      union_func m_unionOp;
      const intersect_func m_intersectsOp;
   };
}