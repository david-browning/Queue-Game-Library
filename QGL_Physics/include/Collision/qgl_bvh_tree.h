#pragma once
#include "include/qgl_physics_include.h"

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
      DirectX::BoundingSphere operator()(
         const DirectX::BoundingSphere& v1,
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

   class test_intersect_callback
   {
      public:
      bool operator()(const DirectX::BoundingSphere& v)
      {
         return true;
      }
   };

   #endif

   /*
    This is a full binary tree.
    Iterators allow pre-order traversal.
    The height of a leaf node is 0.
    BoundingVolume: Geometric structure to use.
    Intersects: Returns a bool indicating if two BoundingVolume intersect.
    Cost: Returns the cost of a BoundingVolume. This could be the volume or
     surface area of the BoundingVolume.
    Union: Given two BoundingVolumes, this returns a BoundingVolume that
     encapsulates both volumes.
    Fatten: Modifies a BoundingVolume to be slightly larger.
    */
   template<
      class BoundingVolume,
      class Intersects,
      class Cost,
      class Union>
      class bvh_tree
   {
      using volume_type = typename BoundingVolume;
      using volume_func = typename Cost;
      using union_func = typename Union;
      using intersect_func = typename Intersects;
      class node_type
      {
         public:
         node_type() :
            m_height(0),
            m_inUse(false)
         {

         }

         node_type(const volume_type& v) :
            m_volume(v),
            m_height(0),
            m_inUse(true)
         {

         }

         node_type(volume_type&& v) :
            m_volume(v),
            m_height(0),
            m_inUse(true)
         {

         }

         node_type(const node_type&) = default;

         node_type(node_type&&) = default;

         inline friend void swap(node_type& l, node_type& r) noexcept
         {
            using std::swap;
            swap(l.m_volume, r.m_volume);
            swap(l.m_height, r.m_height);
            swap(l.m_inUse, r.m_inUse);
         }

         inline node_type& operator=(node_type r) noexcept
         {
            swap(*this, r);
            return *this;
         }

         size_t& height() noexcept
         {
            return m_height;
         }

         size_t height() const noexcept
         {
            return m_height;
         }

         bool leaf() const noexcept
         {
            return m_height == 0;
         }

         volume_type& volume() noexcept
         {
            return m_volume;
         }

         const volume_type& volume() const noexcept
         {
            return m_volume;
         }

         const bool used() const noexcept
         {
            return m_inUse;
         }

         bool& used() noexcept
         {
            return m_inUse;
         }

         private:
         volume_type m_volume;
         size_t m_height;
         bool m_inUse;
      };

      using container = typename std::vector<node_type>;
      using iterator = typename container::iterator;
      using const_iterator = typename container::const_iterator;

      #pragma region Constructors
      bvh_tree(intersect_func intFunc = intersect_func()) :
         m_intersectsOp(intFunc)
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
         //Check if the tree is empty. 
         if (m_nodes.size() == 0)
         {
            //It is, just assign the volume to the root and return it.
            m_nodes.push_back(v);
            return cbegin();
         }

         //Find the best sibling node
         auto sib = best_sibling(v);
         auto sibNode = *sib;

         //Create a parent node that contains the new volume and the best 
         //sibling.
         auto newParent = sib;
         node_type newParentNode(m_unionOp(v, sibNode.volume()));

         //Replace the sibling we found with this new parent node.
         *newParent = newParentNode;

         //Insert the new and old siblings to the new parent node.
         insertLeft(newParent, sibNode);
         insertRight(newParent, node_type(v));

         //Fix up the tree.
         refit(newParent);

         return right(newParent);
      }

      /*
       Removes the node, and all of its children, at position "node".
       */
      void erase(const_iterator node)
      {
         if (node == cbegin())
         {
            m_nodes.clear();
            return;
         }




         //Fix up the tree.
      }

      #pragma endregion

      #pragma region Lookup and Query
      /*
       Returns an iterator to a node's left child. If the node does not have a
       left child, this returns end().
       */
      const_iterator left(const_iterator parent) const noexcept
      {
         auto index = tree::left_index(parent - begin());
         if (index >= m_nodes.size())
         {
            return end();
         }

         return begin() + index;
      }

      /*
       Returns an iterator to a node's right child. If the node does not have a
       right child, this returns end().
       */
      const_iterator right(const_iterator parent) const noexcept
      {
         auto index = tree::right_index(parent - begin());
         if (index >= m_nodes.size())
         {
            return end();
         }

         return begin() + index;
      }

      /*
       Returns an iterator to a node's parent. If the node is the root node,
       this returns end().
       */
      const_iterator parent(const_iterator node) const noexcept
      {
         if (node == begin())
         {
            return end();
         }

         return begin() + tree::parent_index(node - begin());
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
      void intersects(const volume_type& bounding, Callback cb) const
      {
         std::stack<const_iterator> toCheck;
         toCheck.push(begin());

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
      #pragma region Non-Const Lookup
      /*
       Returns an iterator to a node's parent. If the node is the root node,
       this returns end().
       */
      iterator parent(iterator node) noexcept
      {
         if (node == begin())
         {
            return end();
         }

         return begin() + tree::parent_index(node - begin());
      }

      iterator left(iterator node) noexcept
      {
         auto index = tree::left_index(node - begin());
         if (index >= m_nodes.size())
         {
            return end();
         }

         return begin() + index;
      }

      iterator right(iterator node) noexcept
      {
         auto index = tree::right_index(node - begin());
         if (index >= m_nodes.size())
         {
            return end();
         }

         return begin() + index;
      }
      #pragma endregion

      void refit(iterator lastParent)
      {
         while (lastParent != cend())
         {
            auto leftChildIt = left(lastParent);
            auto rightChildIt = right(lastParent);

            //Check the balance
            auto balance = static_cast<int>(leftChildIt->height()) -
               static_cast<int>(rightChildIt->height());

            if (balance > 1)
            {
               //Left heavy
               auto pivot = leftChildIt;

               //Which side of the pivot is heavier.
               auto subBalance = static_cast<int>(left(pivot)->height()) -
                  static_cast<int>(right(pivot)->height());

               if (subBalance < 0)
               {
                  //Tree is left right heavy.
                  
               }

               rotate_right(lastParent);
            }
            else if (balance < -1)
            {
               //Right heavy
               auto pivot = rightChildIt;

               auto subBalance = static_cast<int>(left(pivot)->height()) -
                  static_cast<int>(right(pivot)->height());

               if (subBalance > 0)
               {
                  //Tree is right left heavy
               }

               rotate_left(lastParent);
            }

            lastParent->volume() = m_unionOp(leftChildIt->volume(),
                                             rightChildIt->volume());

            lastParent->height() = 1 + std::max(leftChildIt->height(),
                                                rightChildIt->height());

            lastParent = parent(lastParent);
         }
      }

      /*
       Returns an iterator to a leaf node.
       Inserting v next to the returned sibling will yield the lowest cost.
       */
      iterator best_sibling(const volume_type& v)
      {
         auto it = begin();
         while (!it->leaf())
         {
            //Since the tree is full, non-leaf nodes will always have a right
            //and left child.

            //Cost of inserting the volume to the left.
            float leftCost = m_costOp(m_unionOp(v, it->volume()));

            //Cost of inserting the volume to the right.
            float rightCost = m_costOp(m_unionOp(v, it->volume()));

            if (leftCost < rightCost)
            {
               it = tree::left_index(it);
            }
            else
            {
               it = tree::right_index(it);
            }
         }

         return it;
      }

      /*
       Inserts the node into the parent's left child.
       Expands the tree if necessary.
       */
      void insertLeft(iterator parentNode, const node_type& n)
      {
         auto leftIdx = tree::left_index(parentNode - begin());
         if (leftIdx >= m_nodes.size())
         {
            //Add two extra entries for the left and right child.
            m_nodes.resize(leftIdx + 2);
         }

         m_nodes[leftIdx] = n;
      }

      /*
       Inserts the node into the parent's right child.
       Expands the tree if necessary.
       */
      void insertRight(iterator parentNode, const node_type& n)
      {
         auto rightIdx = tree::right_index(parentNode - begin());
         if (rightIdx >= m_nodes.size())
         {
            throw std::runtime_error("Left did not resize enough.");
            m_nodes.resize(rightIdx + 1);
         }

         m_nodes[rightIdx] = n;
      }

      void rotate_right(iterator r)
      {
         auto pivotIt = left(r);
         
         
      }

      void rotate_left(iterator r)
      {
         auto pivotIt = right(r);

      }

      container m_nodes;
      volume_func m_costOp;
      union_func m_unionOp;
      const intersect_func m_intersectsOp;
   };
}