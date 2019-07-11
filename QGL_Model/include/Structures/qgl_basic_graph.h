#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   template<
      class Key,
      class T,
      class Edge>
      class basic_graph_map
   {
      public:
      using key_type = Key;
      using mapped_type = T;
      using edge_type = Edge;

      static_assert(std::is_default_constructible<T>::value,
                    "T must be default constructible.");

      static_assert(std::is_default_constructible<Edge>::value,
                    "Edge must be default constructible.");

      class vertex_type
      {
         public:
         /*
          Maps a key to an edge.
          */
         using adj_list =
            typename std::unordered_map<typename key_type, typename edge_type>;
         using neighbor_iterator = typename adj_list::iterator;
         using const_neighbor_iterator = typename adj_list::const_iterator;

         vertex_type() = default;

         explicit vertex_type(const mapped_type& val) :
            m_value(val)
         {

         }

         explicit vertex_type(mapped_type&& val) :
            m_value(val)
         {

         }

         vertex_type(const vertex_type&) = default;

         vertex_type(vertex_type&&) = default;

         ~vertex_type() noexcept = default;

         /*
          Returns a reference to this vertex's mapped value.
          */
         mapped_type& mapped() noexcept
         {
            return m_value;
         }

         /*
          Returns a reference to this vertex's mapped value.
          */
         const mapped_type& mapped() const noexcept
         {
            return m_value;
         }

         /*
          Returns a reference to the outgoing edge from this to the vertex
          whose key is equivalent to k. If there is no such edge, this inserts
          a new one and returns a reference to the new edge's value.
          */
         edge_type& edge(const key_type& k)
         {
            return m_neighbors[k];
         }

         /*
          Removes the outgoing edge from this to the node with key equivalent
          to "k". Does nothing if there is no edge connecting the two nodes.
          */
         void unlink(const key_type& k)
         {
            m_neighbors.erase(k);
         }

         neighbor_iterator begin() noexcept
         {
            return m_neighbors.begin();
         }

         neighbor_iterator end() noexcept
         {
            return m_neighbors.end();
         }

         const_neighbor_iterator begin() const noexcept
         {
            return m_neighbors.cbegin();
         }

         const_neighbor_iterator end() const noexcept
         {
            return m_neighbors.cend();
         }

         const_neighbor_iterator cbegin() const noexcept
         {
            return m_neighbors.cbegin();
         }

         const_neighbor_iterator cend() const noexcept
         {
            return m_neighbors.cend();
         }

         private:
         adj_list m_neighbors;
         mapped_type m_value;
      };

      /*
       Maps keys to vertices.
       */
      using container = std::unordered_map<key_type, vertex_type>;

      /*
       Iterator is a pair. First is the vertex's key. Second is the vertex.
       */
      using iterator = typename container::iterator;
      using const_iterator = typename container::const_iterator;
      using size_type = typename container::size_type;

      #pragma region Accessors
      /*
       Searches the graph returns a reference to the mapped value of the
       vertex with key equivalent to k. If no such vertex exists, this throws
       std::out_of_range.
       */
      mapped_type& at(const key_type& k)
      {
         return m_verts.at(k).mapped();
      }

      /*
       Searches the graph returns a reference to the mapped value of the
       vertex with key equivalent to k. If no such vertex exists, this throws
       std::out_of_range.
       */
      const mapped_type& at(const key_type& k) const
      {
         return m_verts.at(k).mapped();
      }

      /*
       Searches the graph and returns a reference to the vertex with key
       equivalent to k. If no such vertex exists, throws std::out_of_range.
       */
      vertex_type& vertex(const key_type& k)
      {
         return m_verts.at(k);
      }

      /*
       Searches the graph and returns a reference to the vertex with key
       equivalent to k. If no such vertex exists, throws std::out_of_range.
       */
      const vertex_type& vertex(const key_type& k) const
      {
         return m_verts.at(k);
      }
      #pragma endregion

      /*
       Returns a reference to the mapped value in vertex with key "k".
       If "k" does not map to a vertex, a new one is inserted and this returns
       a reference to the vertex's mapped value. If a new vertex is inserted,
       it has no inbound or outbound edges.
       */
      mapped_type& operator[](const key_type& k)
      {
         return m_verts[k].mapped();
      }

      /*
       Removes the vertex, if it exists, with key equivalent to "k";
       When this returns, no other vertex in the graph will link to the removed
       vertex.
       */
      void erase(const key_type& k)
      {
         //Get the vertex to erase.
         vertex_type& toRemove = vertex(k);

         //For each neighbor, unlink it and the vertex to erase.
         typename vertex_type::adj_list neighbors(toRemove.begin(),
                                                  toRemove.end());
         for (auto& neighbor : neighbors)
         {
            unlink(neighbor.first, k);
         }

         m_verts.erase(k);
      }

      /*
       Removes the outgoing edge from the vertex whose key is equivalent to
       "from" to the vertex whose key is equivalent to "to".
       Throws std::out_of_range if from or to do not exist.
       */
      void nlink(const key_type& from,
             const key_type& to)
      {
         if (m_verts.count(from) == 0 || m_verts.count(to) == 0)
         {
            throw std::out_of_range("No vertex exists.");
         }

         vertex(from).unlink(to);
      }


      /*
       Returns a reference to the value of the edge that connects the vertex
       with key "from"  with the vertex with key "to".
       If no edge exists, this inserts a new one and returns the reference to
       the new edge's value.
       Throws std::out_of_range if from or to do not exist.
       */
      edge_type& edge(const key_type& from,
                      const key_type& to)
      {
         if (m_verts.count(from) == 0 || m_verts.count(to) == 0)
         {
            throw std::out_of_range("No vertex exists.");
         }

         return vertex(from).edge(to);
      }

      /*
       Returns the number of vertices in the graph.
       */
      size_type size() const noexcept
      {
         return m_verts.size();
      }

      /*
       Returns true if the graph has no vertices.
       */
      [[nodiscard]] bool empty() const noexcept
      {
         return size() == 0;
      }

      #pragma region Iterators
      iterator begin() noexcept
      {
         return m_verts.begin();
      }

      iterator end() noexcept
      {
         return m_verts.end();
      }

      const_iterator begin() const noexcept
      {
         return m_verts.cbegin();
      }

      const_iterator end() const noexcept
      {
         return m_verts.cend();
      }

      const_iterator cbegin() const noexcept
      {
         return m_verts.cbegin();
      }

      const_iterator cend() const noexcept
      {
         return m_verts.cend();
      }
      #pragma endregion

      private:
      container m_verts;
   };
}