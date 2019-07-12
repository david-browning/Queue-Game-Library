#pragma once
#include "include/qgl_physics_include.h"

namespace qgl::physics::collision
{
   #ifdef DEBUG

   class test_sphere_fatten
   {
      public:
      void operator()(DirectX::BoundingSphere& toFatten)
      {
         toFatten.Radius *= FatMultiplier;
      }

      float FatMultiplier;
   };

   class test_volume
   {
      public:
      float operator()(const DirectX::BoundingSphere& bounding)
      {
         auto r3 = std::pow(bounding.Radius, 3.0f);
         return r3 * (4.0f / 3.0f) * math::const_pi<float>;
      }
   };

   #endif

   /*
    This is a full binary tree. The tree must be valid at all
    times and thus, the constructor requires the tree root to be instantiated.
    Iterators allow pre-order traversal.
    The height of a leaf node is 0.
    */
   template<class BoundingVolume, class Fatten , class Volume>
   class bvh_tree
   {
      using volume_type = typename BoundingVolume;
      using fatten_func = typename Fatten;
      using volume_func = typename Volume;

      class node_type
      {
         public:

         private:
         volume_type m_volume;
      };

      using container = typename std::vector<node_type>;
      using iterator = typename container::iterator;
      using const_iterator = typename container::const_iterator;

      #pragma region Modifiers
            
      /*

       Returns an iterator to the inserted node.
       */
      const_iterator insert(const volume_type& v);

      /*
       Removes the node, and all of its children, at position "node".
       Throws std::invalid_argument if trying to remove root.
       */
      void erase(const_iterator node);

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
       bounding volume, this returns an iterator that points to the colliding 
       volume. 
       If there is not a leaf node that "bounding" intersects, this returns 
       end().
       O(log(n))
       */
      const_iterator intersects(const volume_type& bounding) const;

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
      fatten_func m_fatOp;
      volume_func m_volOp;
      container m_nodes;
   };
}