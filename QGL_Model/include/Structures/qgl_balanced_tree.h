#pragma once
#include "include/qgl_model_include.h"
#include "include/Structures/qgl_tree_helpers.h"

namespace qgl
{
   /*
    Represents a full, balanced, binary tree. The tree must be valid at all
    times and thus, the constructor requires the tree root to be instantiated.
    Iterators allow pre-order traversal.
    The height of a leaf node is 0.
    Compare: Binary functor that returns bool and who's arguments are  ValueT.
    */
   template<class ValueT, class Compare = std::less<ValueT>>
   class basic_tree
   {
      public:
      using mapped_type = typename ValueT;
      class node_type
      {
         public:
         /*
          Constructs a leaf node using the given value.
          */
         node_type(const mapped_type& v) :
            m_val(v),
            m_height(0)
         {

         }

         /*
          Constructs a leaf node by moving the given value.
          */
         node_type(mapped_type&& v) :
            m_val(v),
            m_height(0)
         {

         }

         /*
          Copy constructor.
          */
         node_type(const node_type&) = default;

         /*
          Move constructor.
          */
         node_type(node_type&&) = default;

         /*
          Swaps the contents of l and r.
          */
         friend void swap(node_type& l, node_type& r) noexcept;

         /*
          Copy swap assignment.
          */
         node_type& operator=(node_type r);

         /*
          Returns true if the left and right hand operands are equal.
          */
         friend bool operator==(const node_type& l,
                                const node_type& r) noexcept;

         /*
          Returns a reference to the stored value.
          */
         mapped_type& value() noexcept
         {
            return m_val;
         }

         /*
          Returns a reference to the stored value.
          */
         const mapped_type& value() const noexcept
         {
            return m_val;
         }

         /*
          Returns the stored height of this node. If this node has no children,
          then it's height is 0.
          */
         size_t& height() noexcept
         {
            return m_height;
         }

         /*
          Returns the stored height of this node. If this node has no children,
          then it's height is 0.
          */
         size_t height() const noexcept
         {
            return m_height;
         }

         private:
         /*
          Value stored in this node.
          */
         mapped_type m_val;

         /*
          The containing tree sets this.
          */
         size_t m_height;
      };

      using container = typename std::vector<node_type>;
      using iterator = typename container::iterator;
      using const_iterator = typename container::const_iterator;
      using key_compare = typename Compare;

      basic_tree(const mapped_type& rootValue,
                 key_compare comp = key_compare()) :
         m_nodes(1, node_type(rootValue)),
         m_compare(comp)
      {

      }

      basic_tree(mapped_type&& rootValue,
                 key_compare comp = key_compare()) :
         m_nodes(1, node_type(rootValue)),
         m_compare(comp)
      {

      }

      basic_tree(const basic_tree&) = default;

      basic_tree(basic_tree&&) = default;

      ~basic_tree() noexcept = default;

      #pragma region Modifiers
      
      /*
       Inserts the value into this tree and re-balances it.
       Returns an iterator to the inserted node.
       */
      const_iterator insert(const mapped_type& v);

      /*
       Removes the node, and all of its children, at position "node".
       Throws std::invalid_argument if trying to remove root.
       */
      void erase(const_iterator node);

      #pragma endregion

      #pragma region Lookup
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

      bool is_leaf(const_iterator node) const noexcept
      {
         return node->height() == 0;
      }

      /*
       Returns an iterator to a node that contains a value equivalent to "v".
       Returns end() if the tree does not contain such a node.
       */
      const_iterator find(const mapped_type& v) const
      {
         return find(begin(), v, std::equal_to<mapped_type>());
      }

      /*
       Returns an iterator to the leaf node that contains value "v". 
       Returns end() if no node contains the value.
       */
      template<class Contain = std::equal_to<mapped_type>>
      const_iterator contains(const mapped_type& v,
                              Contain binaryEquality) const
      {
         return find(begin(), v, binaryEquality);
      }

      #pragma endregion

      /*
       Returns the height of the tree which is the number of edges along the
       farthest path. 
       */
      size_t height() const noexcept
      {
         return height(begin());
      }

      /*
       Returns the height of the node. The height of a leaf node is 0.
       */
      size_t height(const_iterator node) const noexcept
      {
         return node->height();
      }

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

      #pragma region Rotation Operations

      /*
       Assume the iterator points to a valid node in the tree.
       */
      size_t balance(const_iterator node) const noexcept
      {
         return height(left(node)) - height(right(node));
      }

      void rrotate(const_iterator node);

      void lrotate(const_iterator node);

      #pragma endregion


      /*
       Returns an iterator to a node that contains a value equivalent to "v" 
       using the binary equivalence function.
       Returns end() if the tree does not contain such a node.
       */
      template<class EqualityFunction>
      const_iterator find(const_iterator node,
                          const mapped_type& v,
                          EqualityFunction binaryEqual) const
      {
         if (binaryEqual(v, node->value()))
         {
            return node;
         }
         else if (!is_leaf(node))
         {
            if (m_compare(v, node->value()))
            {
               return find(left(node), v, binaryEqual);
            }
            else
            {
               return find(right(node), v, binaryEqual);
            }
         }

         return end();
      }

      container m_nodes;
      key_compare m_compare;
   };
}