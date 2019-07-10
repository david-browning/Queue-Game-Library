#pragma once
#include "include/qgl_model_include.h"
#include "include/Structures/Vertices/qgl_udgraph_map_vertex.h"

namespace qgl
{
   /*
    Represents an undirected, unweighted graph where each vertex can be 
    accessed using a key.
    The vertices are not ordered.
    */
   template<class KeyT, class ValueT>
   class uugraph_map
   {
      static_assert(std::is_default_constructible<ValueT>::value,
                    "ValueT must be default constructible.");

      public:
      using vertex_t = graph_vertex::udgraph_map_vertex<KeyT, ValueT>;
      using container_t = std::unordered_map<KeyT, vertex_t>;

      /*
       Constructs an empty unweighted, undirected graph.
       */
      uugraph_map()
      {

      }

      /*
       Copy constructor
       */
      uugraph_map(const uugraph_map&) = default;

      /*
       Move constructor
       */
      uugraph_map(uugraph_map&&) = default;

      /*
       Destructor
       */
      ~uugraph_map() noexcept = default;

      /*
       Establishes an edge between the two vertices.
       Does nothing if there is already an edge connected the two vertices.
       Throws std::out_of_range if there is no vertex with key "from" or "to".
       */
      void link(const KeyT& from,
                const KeyT& to)
      {
         vertex(from).insert(to);
         vertex(to).insert(from);
      }

      /*
       Removes the edge that connects the two vertices.
       Does nothing if the two vertices do not have an edge connecting each 
       other.
       Throws std::out_of_range if there is no vertex with key "from" or "to".
       */
      void unlink(const KeyT& from,
                  const KeyT& to)
      {
         vertex(from).erase(to);
         vertex(to).erase(from);
      }

      /*
       Returns a reference to the mapped value in vertex with key "k".
       If "k" does not map to a vertex, a new one is inserted and this returns
       a reference to the vertex's mapped value. If a new vertex is inserted, 
       it has no inbound or outbound edges.
       */
      ValueT& operator[](const KeyT& k)
      {
         return m_vertices[k].mapped();
      }

      /*
       Searches the graph returns a reference to the mapped value of the
       vertex with key equivalent to k. If no such vertex exists, this throws
       std::out_of_range.
       O(|V|)
       */
      ValueT& at(const KeyT& k)
      {
         return m_vertices.at(k).mapped();
      }

      /*
       Searches the graph returns a reference to the mapped value of the
       vertex with key equivalent to k. If no such vertex exists, this throws
       std::out_of_range.
       O(|V|)
       */
      const ValueT& at(const KeyT& k) const
      {
         return m_vertices.at(k).mapped();
      }

      /*
       Searches the graph and returns a reference to the vertex with key
       equivalent to k. If no such vertex exists, throws std::out_of_range.
       O(|V|)
       */
      vertex_t& vertex(const KeyT& k)
      {
         return m_vertices.at(k);
      }

      /*
       Searches the graph and returns a reference to the vertex with key
       equivalent to k. If no such vertex exists, throws std::out_of_range.
       O(|V|)
       */
      const vertex_t& vertex(const KeyT& k) const
      {
         return m_vertices.at(k);
      }

      /*
       Returns the number of nodes in the graph.
       */
      size_t size() const noexcept
      {
         return m_vertices.size();
      }

      /*
       Returns true if the graph has no nodes.
       */
      [[nodiscard]] bool empty() const noexcept
      {
         return size() == 0;
      }

      auto begin() noexcept
      {
         return m_vertices.begin();
      }

      auto end() noexcept
      {
         return m_vertices.end();
      }

      auto begin() const noexcept
      {
         return m_vertices.cbegin();
      }

      auto end() const noexcept
      {
         return m_vertices.cend();
      }

      auto cbegin() const noexcept
      {
         return m_vertices.cbegin();
      }

      auto cend() const noexcept
      {
         return m_vertices.cend();
      }

      /*
       Removes the vertex whose key is equivalent to k.
       Updates all other vertices so that they no longer are connected to the 
       vertex.
       */
      void erase(const KeyT& k)
      {
         //Get the vertex to erase.
         vertex_t& toRemove = vertex(k);

         //For each neighbor, unlink it and the vertex to erase.
         typename vertex_t::adjacency_list neighbors(toRemove.begin(),
                                                     toRemove.end());
         for (auto& neighbor : neighbors)
         {
            unlink(k, neighbor);
         }

         m_vertices.erase(k);
      }

      private:

      /*
       Throws std::out_of_range if the graph does not contain a vertex with key
       "k".
       */
      void exists_throw(const KeyT& k)
      {
         if (m_vertices.count(k) == 0)
         {
            throw std::out_of_range("No vertex with key.");
         }
      }

      container_t m_vertices;
   };
}