#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   /*
    ContainerT: Should be std::map or std::unordered_map
    KeyT: Hashable key used to find tree nodes.
    ValueT: the type of object to store in each node.
    */
   template<template<class, class> class ContainerT, class KeyT, class ValueT>
   class basic_tree_map
   {
      public:
      class basic_tree_map_node
      {
         public:
         using container = ContainerT<KeyT, basic_tree_map_node>;

         basic_tree_map_node(const KeyT& k, const ValueT& v) :
            m_key(k),
            m_val(v),
            m_set(true)
         {

         }

         basic_tree_map_node(KeyT&& k, ValueT&& v) :
            m_key(k),
            m_val(v),
            m_set(true)
         {

         }

         basic_tree_map_node(const basic_tree_map_node&) = default;

         basic_tree_map_node(basic_tree_map_node&&) = default;

         ~basic_tree_map_node() noexcept = default;

         const KeyT& key() const noexcept
         {
            return m_key;
         }

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
         size_t size() const noexcept
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

         auto begin() const noexcept
         {
            return m_children.cbegin();
         }

         auto end() const noexcept
         {
            return m_children.cend();
         }

         auto cbegin() const noexcept
         {
            return m_children.cbegin();
         }

         auto cend() const noexcept
         {
            return m_children.cend();
         }

         auto begin() noexcept
         {
            return m_children.begin();
         }

         auto end() noexcept
         {
            return m_children.end();
         }

         /*
          Searches the tree to find a node with the given key.
          Returns end() if no child has the key.
          Incrementing the returned iterator will return its sibling.
          */
         auto find(const KeyT& k) const noexcept
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
      basic_tree_map(KeyT&& rootKey, ValueT&& rootVal) :
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
       Searches the tree and returns a reference to the mapped value of the 
       element with key equivalent to k. If no such element exists, this throws
       std::out_of_range.
       O(|V|)
       */
      ValueT& at(const KeyT& k)
      {
         return node(k).mapped();
      }

      /*
       Searches the tree and returns a reference to the mapped value of the
       element with key equivalent to k. If no such element exists, this throws
       std::out_of_range.
       O(|V|)
       */
      const ValueT& at(const KeyT& k) const
      {
         return node(k).mapped();
      }

      /*
       Searches the tree and returns a reference to the node with key 
       equivalent to k. If no such element exists, throws std::out_of_range.
       O(|V|)
       */
      node_type& node(const KeyT& k)
      {
         return const_cast<node_type&>(
            static_cast<const node_type&>((*this).node(k)));
      }

      /*
       Searches the tree and returns a reference to the node with key
       equivalent to k. If no such element exists, throws std::out_of_range.
       O(|V|)
       */
      const node_type& node(const KeyT& k) const
      {
         if (m_root.key() == k)
         {
            return m_root;
         }

         std::queue<const node_type&> toSearch;
         toSearch.push(m_root);

         while (!toSearch.empty())
         {
            auto& node = toSearch.front();
            if (node.key() == k)
            {
               return node;
            }

            for (auto& child : node)
            {
               toSearch.push(child.second);
            }

            toSearch.pop();
         }

         throw std::out_of_range("No node with key exists.");
      }

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

      /*
       Removes the node, and all of that node's children, who's key is
       equivalent to k.
       Does nothing if there is no node with key k.
       Throws std::invalid_argument if the root node's key is equivalent to k.
       */
      void erase(const KeyT& k)
      {
         //Cannot remove the root.
         if (m_root.key() == k)
         {
            throw std::invalid_argument("Cannot remove the root.");
         }

         std::queue<node_type&> parentsToSeach;
         parentsToSeach.push(m_root);

         while (!parentsToSeach.empty())
         {
            auto& node = parentsToSeach.front();
            
            for (auto& child : node)
            {
               if (child.second.key() == k)
               {
                  node.erase(k);
                  return;
               }
               else
               {
                  parentsToSeach.push(child.second);
               }
            }

            parentsToSeach.pop();
         }
      }

      private:
      node_type m_root;
   };
}