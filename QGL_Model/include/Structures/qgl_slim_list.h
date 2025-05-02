#pragma once
#include "include/qgl_model_include.h"
#include "include/Threads/qgl_srw_traits.h"

namespace qgl
{
   /*
    A slim list uses a slim reader writer lock to control access to the list.
    Read operations will set a shared lock. Write operations, such as those that
    insert or remove elements from the list, will set an exclusive lock.

    While in exclusive lock mode, no other thread can read or write to the
    list. While in shared lock mode, any thread can read, but in order to
    modify the list, all read operations must finish.

    Attempting to acquire an exclusive lock will block the current thread until
    all shared and exclusive operations are finished.

    Attempting to acquire a shared lock will block the current thread until
    all exclusive operations are finished.

    Accessing elements will return an std::pair that includes a reference to
    the element and a copy of the SRW lock. A copy of the lock is returned so
    that once it goes out of scope, the shared lock is decremented automatically.
    The caller should not modify the returned lock and let the destructor
    handle cleaning up access to the resource.
    */
   template<class T, class SRWTraits = qgl::srw_traits>
   class slim_list final
   {
      public:
      using value_type = typename std::pair<T&, SRWTraits>;
      using const_value_type = typename const std::pair<const T&, SRWTraits>;

      static_assert(std::is_default_constructible<SRWTraits>::value,
                    "The traits must be default constructible.");

      struct node_t final
      {
         node_t(const T& value) :
            data(value)
         {
            prev_p = next_p = nullptr;
         }

         node_t(T&& value) :
            data(std::forward<T>(value))
         {
            prev_p = next_p = nullptr;
         }

         node_t(node_t&& r) :
            data(std::move(r.data)),
            next_p(r.next_p),
            prev_p(r.prev_p)
         {
            r.next_p = nullptr;
            r.prev_p = nullptr;
         }

         ~node_t() noexcept = default;

         T data;
         node_t* next_p;
         node_t* prev_p;
      };

      template<bool IsConst>
      class basic_iterator final
      {
         public:
         using value_type = typename T;
         using reference = typename std::conditional<IsConst, const value_type&, value_type&>::type;
         using pointer = typename std::conditional<IsConst, const value_type*, value_type*>::type;

         basic_iterator(const node_t* n_p, const SRWTraits& traits) :
            m_node_p(n_p), m_traits(traits)
         {
            m_traits.share_lock();
         }

         basic_iterator(const basic_iterator&) = default;

         basic_iterator(basic_iterator&&) noexcept = default;

         ~basic_iterator() noexcept
         {
            m_traits.share_release();
         }

         friend void swap(basic_iterator& l, basic_iterator& r)
         {
            using std::swap;
            swap(l.m_node_p, r.m_node_p);
            swap(l.m_traits, r.m_traits);
         }

         basic_iterator& operator=(basic_iterator r)
         {
            swap(*this, r);
            return *this;
         }

         reference operator*() const
         {
            if (!m_node_p)
            {
               throw std::out_of_range{ "Node is null." };
            }

            return m_node_p->data;
         }

         basic_iterator& operator++()
         {
            // Only advance the pointer if its not null
            if (m_node_p)
            {
               m_node_p = m_node_p->next_p;
            }

            return *this;
         }

         basic_iterator& operator--()
         {
            if (!m_node_p || !m_node_p->prev_p)
            {
               throw std::out_of_range{ "Previous pointer is null." };
            }

            m_node_p = m_node_p->prev_p;
            return *this;
         }

         bool operator==(const basic_iterator& r) const noexcept
         {
            // Iterators are the same if they point to the same node.
            return m_node_p == r.m_node_p;
         }

         bool operator!=(const basic_iterator& r) const noexcept
         {
            return !(*this == r);
         }

         private:
         const node_t* m_node_p;
         SRWTraits m_traits;
      };

      using iterator = basic_iterator<false>;
      using const_iterator = basic_iterator<true>;

      static_assert(std::is_copy_constructible<const_iterator>::value,
                    "Slim List Iterator is not copy constructible");
      static_assert(std::is_copy_assignable<const_iterator>::value,
                    "Slim List Iterator is not copy assignable");
      static_assert(std::is_destructible<const_iterator>::value,
                    "Slim List Iterator is not destructible");

      slim_list(SRWTraits traits = SRWTraits()) :
         m_head_p(nullptr),
         m_tail_p(nullptr),
         m_traits(traits),
         m_size(0)
      {

      }

      template<class InputIt>
      slim_list(InputIt first, InputIt last,
                SRWTraits traits = SRWTraits()) :
         m_head_p(nullptr),
         m_tail_p(nullptr),
         m_traits(traits),
         m_size(0)
      {
         while (first != last)
         {
            push_back(*first);
            ++first;
         }
      }

      slim_list(std::initializer_list<T> init,
                SRWTraits traits = SRWTraits()) :
         slim_list(init.begin(), init.end(), traits)
      {

      }

      /*
       Copy constructor
       */
      slim_list(const slim_list& r)
      {
         r.m_traits.excl_lock();
         auto cur = r.m_head_p;
         while (cur)
         {
            push_back(cur->data);
            cur = cur->next_p;
         }

         r.m_traits.excl_release();
      }

      /*
       Move constructor
       */
      slim_list(slim_list&& r)
      {
         r.m_traits.excl_lock();
         m_size = r.m_size;
         m_head_p = r.m_head_p;
         m_tail_p = r.m_tail_p;
         r.m_head_p = nullptr;
         r.m_tail_p = nullptr;
         r.m_size = 0;
         r.m_traits.excl_release();
      }

      /*
       Destructor
       */
      ~slim_list() noexcept
      {
         m_traits.excl_lock();
         delete_list();

         // Traits disposed now and will release the exclusive lock.
      }

      /*
       Swaps the contents of the two lists.
       */
      friend void swap(slim_list& l, slim_list& r)
      {
         l.m_traits.excl_lock();
         r.m_traits.excl_lock();

         // Swap the contents
         using std::swap;
         swap(l.m_traits, r.m_traits);
         swap(l.m_size, r.m_size);
         swap(l.m_head_p, r.m_head_p);
         swap(l.m_tail_p, r.m_tail_p);

         // Release the exclusive locks.
         r.m_traits.excl_release();
         l.m_traits.excl_release();
      }

      /*
       Copy assign
       */
      slim_list& operator=(slim_list r)
      {
         swap(*this, r);
         return *this;
      }

      /*
       Returns a reference to the first element and a shared lock.
       Throws std::out_of_range if the list is empty.
       */
      value_type front()
      {
         SRWTraits sharedLock{ m_traits };
         sharedLock.share_lock();
         if (m_size == 0)
         {
            throw std::out_of_range{ "List is empty." };
         }

         return std::pair<T&, SRWTraits>(
            m_head_p->data, std::move(sharedLock));
      }

      /*
       Returns a const reference to the first element and a shared lock.
       Throws std::out_of_range if the list is empty.
       */
      const_value_type front() const
      {
         SRWTraits sharedLock{ m_traits };
         sharedLock.share_lock();
         if (m_size == 0)
         {
            throw std::out_of_range{ "List is empty." };
         }

         return std::pair<const T&, SRWTraits>(
            m_head_p->data, std::move(sharedLock));
      }

      /*
       Returns a reference to the list element and a shared lock.
       Throws std::out_of_range if the list is empty.
       */
      value_type back()
      {
         SRWTraits sharedLock{ m_traits };
         sharedLock.share_lock();
         if (m_size == 0)
         {
            throw std::out_of_range{ "List is empty." };
         }

         return std::pair<T&, SRWTraits>(
            m_tail_p->data, std::move(sharedLock));
      }


      /*
       Returns a const reference to the last element and a shared lock.
       Throws std::out_of_range if the list is empty.
       */
      const_value_type back() const
      {
         SRWTraits sharedLock{ m_traits };
         sharedLock.share_lock();
         if (m_size == 0)
         {
            throw std::out_of_range{ "List is empty." };
         }

         return std::pair<const T&, SRWTraits>(
            m_tail_p->data, std::move(sharedLock));
      }

      const_iterator begin() const
      {
         return cbegin();
      }

      const_iterator end() const
      {
         return cend();
      }

      const_iterator cbegin() const
      {
         return const_iterator{ m_head_p, m_traits };
      }

      const_iterator cend() const
      {
         return const_iterator{ nullptr, m_traits };
      }

      /*
       Obtains a shared lock and checks if the list is empty.
       */
      [[nodiscard]] bool empty() const
      {
         m_traits.share_lock();
         auto ret = m_size == 0;
         m_traits.share_release();
         return ret;
      }

      /*
       Obtains a shared lock and returns the number of elements in the list.
       */
      [[nodiscard]] size_t size() const
      {
         m_traits.share_lock();
         auto ret = m_size;
         m_traits.share_release();
         return ret;
      }

      /*
       Acquires an exclusive lock and clears the vector.
       */
      void clear()
      {
         m_traits.excl_lock();
         delete_list();
         m_traits.excl_release();
      }

      /*
       Acquires an exclusive lock and copies value to the front of the list.
       */
      void push_front(const T& value)
      {
         m_traits.excl_lock();
         if (!m_head_p)
         {
            m_head_p = new node_t{ value };
            m_tail_p = m_head_p;
         }
         else
         {
            auto node = new node_t{ value };
            node->next_p = m_head_p;
            m_head_p->prev_p = node;
            m_head_p = node;
         }

         m_size++;
         m_traits.excl_release();
      }

      /*
       Acquires an exclusive lock and moves value to the front of the list.
       */
      void push_front(T&& value)
      {
         m_traits.excl_lock();
         auto node = new node_t{ std::forward<T>(value) };
         if (!m_head_p)
         {
            m_head_p = m_tail_p = node;
         }
         else
         {
            node->next_p = m_head_p;
            m_head_p->prev_p = node;
            m_head_p = node;
         }

         m_size++;
         m_traits.excl_release();
      }

      /*
       Acquires an exclusive lock and copies value to the end of the list.
       */
      void push_back(const T& value)
      {
         m_traits.excl_lock();
         auto newNode = new node_t{ value };
         if (m_tail_p)
         {
            auto oldTail = m_tail_p;
            oldTail->next_p = newNode;
            m_tail_p = newNode;
            m_tail_p->prev_p = oldTail;
         }
         else
         {
            m_head_p = m_tail_p = newNode;

         }

         m_size++;
         m_traits.excl_release();
      }

      /*
       Acquires an exclusive lock and moves value to the end of the list.
       */
      void push_back(T&& value)
      {
         m_traits.excl_lock();
         auto newNode = new node_t{ std::forward<T>(value) };
         if (m_tail_p)
         {
            auto oldTail = m_tail_p;
            oldTail->next_p = newNode;
            m_tail_p = newNode;
            m_tail_p->prev_p = oldTail;
         }
         else
         {
            m_head_p = m_tail_p = newNode;

         }

         m_size++;
         m_traits.excl_release();
      }

      /*
       Acquires an exclusive lock and removes the first element in the list.
       */
      void pop_front()
      {
         m_traits.excl_lock();
         if (m_head_p)
         {
            auto toDelete = m_head_p;
            m_head_p = m_head_p->next_p;

            if (m_head_p) // Only if there *is* a new head
            {
               m_head_p->prev_p = nullptr;
            }
            else
            {
               // List is now empty
               m_tail_p = nullptr;
            }

            delete toDelete;
            m_size--;
         }

         m_traits.excl_release();
      }

      /*
       Acquires an exclusive lock and removes the first element in the list.
       */
      void pop_back()
      {
         m_traits.excl_lock();
         if (m_tail_p)
         {
            auto toDelete = m_tail_p;
            auto newTail = m_tail_p->prev_p;
            if (newTail)
            {
               newTail->next_p = nullptr;
            }
            else
            {
               m_head_p = nullptr;
            }

            m_tail_p = newTail;
            delete toDelete;
            m_size--;
         }

         m_traits.excl_release();
      }

      private:
      void delete_list()
      {
         while (m_head_p != nullptr)
         {
            auto next = m_head_p->next_p;
            delete m_head_p;
            m_head_p = next;
         }

         m_tail_p = nullptr;
         m_size = 0;
      }

      node_t* m_head_p;
      node_t* m_tail_p;
      size_t m_size;

      /*
       In exclusive mode, this controls access to the entire list.
       In shared mode, this controls access to individual elements in the
       list.
       */
      mutable SRWTraits m_traits;
   };
}