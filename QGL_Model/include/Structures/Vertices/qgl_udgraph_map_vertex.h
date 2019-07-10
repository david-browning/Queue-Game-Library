#pragma once
#include "include/qgl_model_include.h"

namespace qgl::graph_vertex
{
   /*
    Represents a vertex in an unweighted, directed graph.
    KeyT: The key used to look up vertices.
    ValueT: The type of
    */
   template<
      class KeyT,
      class ValueT>
   class udgraph_map_vertex
   {
      public:
      static_assert(std::is_default_constructible<ValueT>::value,
                    "ValueT must be default constructible.");

      using adjacency_list = std::unordered_set<KeyT>;

      /*
       Construct a new vertex.
       */
      udgraph_map_vertex() :
         m_value()
      {

      }

      /*
       Copies val.
       */
      udgraph_map_vertex(const ValueT& val) :
         m_value(val)
      {

      }

      /*
       Moves val.
       */
      udgraph_map_vertex(ValueT&& val) :
         m_value(val)
      {

      }

      /*
       Copy constructor.
       */
      udgraph_map_vertex(const udgraph_map_vertex&) = default;

      /*
       Move constructor
       */
      udgraph_map_vertex(udgraph_map_vertex&&) = default;

      /*
       Destructor
       */
      ~udgraph_map_vertex() noexcept = default;

      /*
       Swaps the contents of this and other.
       */
      void swap(udgraph_map_vertex& other) noexcept
      {
         using std::swap;
         swap(m_value, other.m_value);
         swap(m_neighbors, other.m_neighbors);
      }

      /*
       Returns a reference to this vertex's mapped value.
       */
      ValueT& mapped() noexcept
      {
         return m_value;
      }

      /*
       Returns a reference to this vertex's mapped value.
       */
      const ValueT& mapped() const noexcept
      {
         return m_value;
      }

      /*
       Removes any outgoing link between this vertex and a vertex with key
       equivalent to "k". The vertex with key "k" still links to this vertex.
       Does nothing if there is no link.
       */
      void erase(const KeyT& k) noexcept
      {
         m_neighbors.erase(k);
      }

      /*
       Adds a key to the adjacency list. Does nothing if the adjacency list 
       already contains the key.
       */
      void insert(const KeyT& k)
      {
         m_neighbors.insert(k);
      }

      /*
       Returns the number of neighbors.
       */
      size_t size() const noexcept
      {
         return m_neighbors.size();
      }

      /*
       Returns true if this vertex has no neighbors
       */
      [[nodiscard]] bool empty() const noexcept
      {
         return size() == 0;
      }

      auto begin() noexcept
      {
         return m_neighbors.begin();
      }

      auto end() noexcept
      {
         return m_neighbors.end();
      }

      auto begin() const noexcept
      {
         return m_neighbors.cbegin();
      }

      auto end() const noexcept
      {
         return m_neighbors.cend();
      }

      auto cbegin() const noexcept
      {
         return m_neighbors.cbegin();
      }

      auto cend() const noexcept
      {
         return m_neighbors.cend();
      }

      private:
      ValueT m_value;
      adjacency_list m_neighbors;
   };
}