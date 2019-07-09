#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   template<template<class, class> class ContainerT, class ValueT, class KeyT>
   class basic_tree_map
   {
      public:
      class basic_tree_map_node
      {
         public:
         using container = ContainerT<KeyT, basic_tree_map_node>;
         using iterator = container::iterator;
         using const_iterator = container::const_iterator;
         using size_type = container::size_type;

         basic_tree_map_node(const KeyT& k, const ValueT& v) :
            m_key(k),
            m_val(v),
            m_set(true)
         {

         }

         basic_tree_map_node(const KeyT&& k, const ValueT&& v) :
            m_key(k),
            m_val(v),
            m_set(true)
         {

         }

         basic_tree_map_node(const basic_tree_map_node&) = default;

         basic_tree_map_node(basic_tree_map_node&&) = default;

         ~basic_tree_map_node() noexcept = default;

         /*
          Returns a reference to the mapped value.
          Throws std::invalid_argument if there is no root node.
          */
         ValueT& mapped() noexcept
         {
            if (empty())
            {
               throw std::invalid_argument("No Root");
            }

            return m_val;
         }

         /*
          Returns a reference to the mapped value.
          Throws std::invalid_argument if there is no root node.
          */
         const ValueT& mapped() const noexcept
         {
            if (empty())
            {
               throw std::invalid_argument("No Root");
            }

            return m_val;
         }

         /*
          Removes the node with the given key, and all its children.
          */
         void erase(const KeyT& k)
         {
            m_children.erase(k);
         }

         /*
          Returns a reference to the value that is mapped to a key equivalent
          to key, performing an insertion if such key does not already exist.
          */
         ValueT& operator[](const KeyT& k)
         {
            return m_children[k].mapped();
         }

         /*
          Returns a reference to the mapped value of the element with key
          equivalent to key. If no such element exists, an exception of type
          std::out_of_range is thrown.
          */
         ValueT& at(const KeyT& k)
         {
            return m_children.at(k).mapped();
         }

         /*
          Returns a reference to the mapped value of the element with key
          equivalent to key. If no such element exists, an exception of type
          std::out_of_range is thrown.
          */
         const ValueT& at(const KeyT& k) const
         {
            return m_children.at(k).mapped();
         }

         /*
          Returns the number of children this node has. This only includes the 
          direct children of this node. It does not include grandchildren, 
          great-grandchildren, etc.
          */
         size_type size() const noexcept
         {
            return m_children.size();
         }

         /*
          Erases all child elements. After this call, size() returns zero.
          */
         void clear() noexcept
         {
            m_children.clear();
         }

         const_iterator begin() const noexcept
         {
            return m_children.cbegin();
         }

         const_iterator end() const noexcept
         {
            return m_children.cend();
         }

         const_iterator cbegin() const noexcept
         {
            return m_children.cbegin();
         }

         const_iterator cend() const noexcept
         {
            return m_children.cend();
         }

         iterator begin() noexcept
         {
            return m_children.begin();
         }

         iterator end() noexcept
         {
            return m_children.end();
         }

         /*
          Searches the tree to find a node with the given key.
          Returns end() if no child has the key.
          Incrementing the returned iterator will return its sibling.
          */
         const_iterator find(const KeyT& k) const noexcept
         {
            //Queue of nodes.
            std::queue<decltype(begin())> q;

            //Queue all the children.
            for (auto it = begin(); it != end(); ++it)
            {
               q.push(it);
            }

            while (!q.empty())
            {
               auto it = q.front();
               if (it->first == k)
               {
                  return it;
               }

               auto& node = it->second;
               for (auto childIt = node.begin(); 
                    childIt != node.end(); 
                    ++childIt)
               {
                  q.push(childIt);
               }

               q.pop();
            }

            return end();
         }

         /*
          Returns true if this node has a child with the given key. Performs a
          depth-first search to examine all children.
          */
         [[nodiscard]] bool contains(const KeyT& k) const noexcept
         {
            return find(k) != end();
         }

         /*
          Returns true if the node handle is empty, false otherwise.
          */
         [[nodiscard]] bool empty() const noexcept
         {
            return !m_set;
         }

         /*
          Converts to false if the node handle is empty, true otherwise. 
          */
         explicit operator bool() const noexcept
         {
            return !empty();
         }

         private:
         /*
          Value this node holds.
          */
         ValueT m_val;

         /*
          Key for this node.
          */
         KeyT m_key;

         /*
          Map a key to a tree node.
          */
         container m_children;

         bool m_set;
      };

      using node_type = basic_tree_map_node;

      /*
       Constructs the root node with the given root key and root value. 
       */
      basic_tree_map(const KeyT& rootKey, const ValueT& rootVal) :
         m_root(rootKey, rootVal)
      {
      }

      /*
       Constructs the root node by moving the root key and root value.
       */
      basic_tree_map(const KeyT&& rootKey, const ValueT&& rootVal) :
         m_root(rootKey, rootVal)
      {

      }

      /*
       Copy constructor
       */
      basic_tree_map(const basic_tree_map&) = default;

      /*
       Move constructor
       */
      basic_tree_map(basic_tree_map&&) = default;

      /*
       Destructor
       */
      ~basic_tree_map() noexcept = default;

      /*
       Returns a reference to the value stored in the root node.
       Throws std::invalid_argument if there is no root node.
       */
      ValueT& root()
      {
         return m_root.mapped();
      }

      /*
       Returns a reference to the value stored in the root node.
       Throws std::invalid_argument if there is no root node.
       */
      const ValueT& root() const
      {
         return m_root.mapped();
      }

      /*
       Returns a reference to the root node.
       */
      node_type& root_node() noexcept
      {
         return m_root;
      }

      /*
       Returns a reference to the root node.
       */
      const node_type& root_node() const noexcept
      {
         return m_root;
      }

      private:
      node_type m_root;
   };
}