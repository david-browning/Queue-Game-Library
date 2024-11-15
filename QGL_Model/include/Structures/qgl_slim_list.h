#pragma once
#include "include/qgl_model_include.h"
#include "include/Threads/qgl_srw_traits.h"
#include <stdexcept>

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

         friend void swap(node_t& l, node_t& r)
         {
            using std::swap;
            swap(l.data, r.data);
            swap(l.next_p, r.next_p);
            swap(l.prev_p, r.prev_p);
         }

         bool operator==(const node_t& r) const noexcept
         {
            return next_p == r.next_p && prev_p == r.prev_p;
         }

         T data;
         node_t* next_p;
         node_t* prev_p;
      };

      class const_iterator final
      {
         public:
         const_iterator(const node_t* n_p, const SRWTraits& traits) :
            m_node_p(n_p), m_traits(traits)
         {
            m_traits.share_lock();
         }

         const_iterator(const const_iterator&) = default;

         const_iterator(const_iterator&&) noexcept = default;

         ~const_iterator() noexcept
         {
            m_traits.share_release();
         }

         friend void swap(const_iterator& l, const_iterator& r)
         {
            using std::swap;
            swap(l.m_node_p, r.m_node_p);
            swap(l.m_traits, r.m_traits);
         }

         const_iterator& operator=(const_iterator r)
         {
            swap(*this, r);
            return *this;
         }

         const T& operator*() const
         {
            if (!m_node_p)
            {
               throw std::out_of_range {"Node is null."};
            }

            return m_node_p->data;
         }

         const_iterator& operator++()
         {
            if (!m_node_p || !m_node_p->next_p)
            {
               throw std::out_of_range{"Next pointer is null."};
            }

            m_node_p = m_node_p->next_p;
            return *this;
         }

         const_iterator& operator--()
         {
            if (!m_node_p || !m_node_p->prev_p)
            {
               throw std::out_of_range{"Previous pointer is null."};
            }

            m_node_p = m_node_p->prev_p;
            return *this;
         }

         bool operator==(const const_iterator& r) const noexcept
         {
            return *m_node_p == *r.m_node;
         }

         bool operator!=(const const_iterator& r) const noexcept
         {
            return !(*this == r);
         }

         private:
         const node_t* m_node_p;
         SRWTraits m_traits;
      };

      class iterator final
      {
         public:
         iterator(node_t* n_p, const SRWTraits& traits) :
            m_node_p(n_p), m_traits(traits)
         {
            m_traits.share_lock();
         }

         iterator(const iterator&) = default;

         iterator(iterator&&) noexcept = default;

         ~iterator()
         {
            m_traits.share_release();
         }

         friend void swap(iterator& l, iterator& r)
         {
            using std::swap;
            swap(l.m_node_p, r.m_node_p);
            swap(l.m_traits, r.m_traits);
         }

         iterator& operator=(iterator r)
         {
            swap(*this, r);
            return *this;
         }

         T& operator*()
         {
            if (!m_node_p)
            {
               throw std::out_of_range {"Node is null."};
            }

            return m_node_p->data;
         }

         iterator& operator++()
         {
            if (!m_node_p || !m_node_p->next_p)
            {
               throw std::out_of_range{"Next pointer is null."};
            }

            m_node_p = m_node_p->next_p;
            return *this;
         }

         iterator& operator--()
         {
            if (!m_node_p || !m_node_p->prev_p)
            {
               throw std::out_of_range{"Previous pointer is null."};
            }

            m_node_p = m_node_p->prev_p;
            return *this;
         }

         bool operator==(const iterator& r) const noexcept
         {
            return *m_node_p == *r.m_node_p;
         }

         bool operator!=(const iterator& r) const noexcept
         {
            return !(*this == r);
         }

         private:
         node_t* m_node_p;
         SRWTraits m_traits;
      };

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
         m_traits(traits)
      {
         m_size = 0;
         m_head_p = nullptr;

         // If the range is not empty:
         if (first != last)
         {
            // Set up the head
            m_head_p = new node_t{ *first };
            first++;
            m_size = 1;
         }

         node_t* last_p = m_head_p;
         node_t* cur_p = nullptr;

         while (first != last)
         {
            cur_p = new node_t{ *first };
            cur_p->prev_p = last_p;
            last_p->next_p = cur_p;
            last_p = cur_p;
            m_size++;
            first++;
         }

         // Set the tail.
         m_tail_p = cur_p;
      }

      slim_list(std::initializer_list<T> init,
                SRWTraits traits = SRWTraits()) :
         slim_list(init.begin(), init.end(), traits)
      {

      }

      /*
       Copy constructor
       */
      slim_list(const slim_list& r) :
         m_traits(r.m_traits)
      {
         r.m_traits.share_lock();

         m_size = 0;
         m_head_p = nullptr;

         auto first = r.begin();
         auto last = r.end();

         // If the range is not empty:
         if (first != last)
         {
            // Set up the head
            m_head_p = new node_t{ *first };
            first++;
            m_size = 1;
         }

         node_t* last_p = m_head_p;
         node_t* cur_p = nullptr;

         while (first != last)
         {
            cur_p = new node_t{ *first };
            cur_p->prev_p = last_p;
            last_p->next_p = cur_p;
            last_p = cur_p;
            m_size++;
            first++;
         }

         // Set the tail.
         m_tail_p = cur_p;

         r.m_traits.share_release();
      }

      /*
       Move construct
       */
      slim_list(slim_list&& r)
      {
         r.m_traits.excl_lock();
         m_size = r.m_size;
         m_head_p = r.m_head_p;
         m_tail_p = r.m_tail_p;
         r.m_head_p = nullptr;
         r.m_tail_p = nullptr;
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
         swap(l.m_head_p, r.m_heap_p);
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

      iterator begin()
      {
         return iterator{ m_head_p, m_traits };
      }

      iterator end()
      {
         return iterator{ nullptr, m_traits };
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
         return m_size == 0;
         m_traits.share_release();
      }

      /*
       Obtains a shared lock and returns the number of elements in the list.
       */
      [[nodiscard]] size_t size() const
      {
         m_traits.share_lock();
         return m_size;
         m_traits.share_release();
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
       Acquires an exclusive lock and copies value to a new element before pos.
       */
      iterator insert(const_iterator pos, const T& value);

      /*
       Acquires an exclusive lock and moves value to before pos;
       */
      iterator insert(const_iterator pos, T&& value);

      /*
       Acquires an exclusive lock and emplaces a new value in list before pos
       by forwarding the arguments.
       */
      template<class... Args>
      void emplace(const_iterator pos, Args&&... args);

      /*
       Acquires an exclusive lock and removes the element at the give position.
       */
      void erase(const_iterator pos);

      /*
       Acquires an exclusive lock and removes the elements in the range
       [first, last)
       */
      void erase(const_iterator first, const_iterator last);

      /*
       Acquires an exclusive lock and copies value to the end of the list.
       */
      void push_back(const T& value);

      /*
       Acquires an exclusive lock and moves value to the end of the list.
       */
      void push_back(T&& value);

      /*
       Acquires an exclusive lock and constructs an element at the end of the
       list by forwarding the arguments.
       */
      template<class... Args>
      void emplace_back(Args&&... args);

      /*
       Acquires an exclusive lock and removes the first element in the list.
       */
      void pop_back();

      /*
       Acquires an exclusive lock and copies value to the front of the list.
       */
      void push_front(const T& value);

      /*
       Acquires an exclusive lock and moves value to the front of the list.
       */
      void push_front(T&& value);

      /*
       Acquires an exclusive lock and constructs an element at the front of
       the list by forwarding the arguments.
       */
      template<class... Args>
      void emplace_front(Args&&... args);

      /*
       Acquires an exclusive lock and removes the first element in the list.
       */
      void pop_front();

      /*
       Acquires an exclusive lock and resizes the list to the new size.
       New elements are default constructed.
       */
      void resize(size_t count)
      {
         resize(count, T{});
      }

      /*
       Acquires an exclusive lock and resizes the list to the new size and
       fills any new elements with copies of value.
       */
      void resize(size_t count, const T& value);

      void merge(slim_list& other);

      void merge(slim_list&& other);

      template<class Compare>
      void merge(slim_list& other);

      template<class Compare>
      void merge(slim_list&& other);

      void splice(const_iterator pos, slim_list& other);

      void splice(const_iterator pos, slim_list&& other);

      void splice(const_iterator pos, slim_list& other, const_iterator it);

      void splice(const_iterator pos, slim_list&& other, const_iterator it);

      /*
       Acquires an exclusive lock and reverses the elements in this list.
       */
      void reverse();

      /*
       Acquires an exclusive lock and removes any consecutive duplicate
       elements.
       Returns the number of elements removed.
       */
      size_t unique()
      {
         return unique(std::equal_to<T>{});
      }

      /*
       Acquires an exclusive lock and removes any consecutive duplicate
       elements according to the binary predicate.
       Returns the number of elements removed.
       */
      template<class BinaryPredicate>
      size_t unique(BinaryPredicate p)
      {
         size_t ret = 0;
         m_traits.excl_lock();

         auto prev_p = nullptr;
         auto cur_p = m_head_p;
         while (cur_p != nullptr)
         {
            if (prev_p && p(cur_p->data, prev_p->data))
            {
               prev_p->next_p = cur_p->next_p;
               delete cur_p;
               ret++;
            }

            prev_p = cur_p;
            cur_p = cur_p->next_p;
         }

         // Cur is null at this point.

         m_tail_p = prev_p->next_p;
         m_traits.excl_release();
         return ret;
      }

      /*
       Acquires an exclusive lock and swaps the contents of this list and
       other.
       */
      void swap(slim_list& other)
      {
         // Put an exclusive lock on this and the other.
         m_traits.excl_lock();
         other.m_traits.lock();

         using std::swap;
         swap(m_head_p, other.m_head_p);
         swap(m_tail_p, other.m_tail_p);
         swap(m_size, other.m_size);

         other.m_traits.excl_release();
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