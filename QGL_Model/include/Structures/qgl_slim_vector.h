#pragma once
#include "include/qgl_model_include.h"
#include "include/Threads/qgl_srw_traits.h"

namespace qgl
{
   /*
    A slim vector uses a slim reader writer lock to control access to the vector.
    Read operations will set a shared lock. Write operations, such as those that
    insert or remove elements from the vector, will set an exclusive lock.

    While in exclusive lock mode, no other thread can read or write to the
    vector. While in shared lock mode, any thread can read, but in order to
    modify the vector, all read operations must finish.

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
   class slim_vector final
   {
      public:
      using value_type = typename std::pair<T&, SRWTraits>;
      using const_value_type = typename const std::pair<const T&, SRWTraits>;
      using data_t = typename std::vector<T>;

      static_assert(std::is_default_constructible<SRWTraits>::value,
                    "The traits must be default constructible.");

      class const_iterator
      {
         public:
         const_iterator(const T* data, const SRWTraits& traits) :
            m_data_p(data), m_traits(traits)
         {
            m_traits.share_lock();
         }

         const_iterator(const const_iterator& r) = default;

         const_iterator(const_iterator&& r) noexcept = default;

         ~const_iterator()
         {
            m_traits.share_release();
         }

         friend void swap(const_iterator& l, const_iterator& r) noexcept
         {
            using std::swap;
            swap(l.m_data_p, r.m_data_p);
            swap(l.m_traits, r.m_traits);
         }

         const_iterator& operator=(const_iterator r)
         {
            swap(*this, r);
            return *this;
         }

         const T& operator*() const
         {
            return *m_data_p;
         }

         const_iterator& operator++()
         {
            ++m_data_p;
            return *this;
         }

         const_iterator& operator++(int)
         {
            auto tmp = *this;
            ++(*this);
            return tmp;
         }

         const_iterator& operator--()
         {
            --m_data_p;
            return *this;
         }

         const_iterator& operator--(int)
         {
            auto tmp = *this;
            --(*this);
            return tmp;
         }

         const_iterator& operator+=(ptrdiff_t n)
         {
            m_data_p += n;
            return *this;
         }

         const_iterator& operator-=(ptrdiff_t n)
         {
            m_data_p -= n;
            return *this;
         }

         const_iterator& operator+(ptrdiff_t n) const
         {
            auto tmp = *this;
            return tmp += n;
         }

         const_iterator& operator-(ptrdiff_t n) const
         {
            auto tmp = *this;
            return tmp -= n;
         }

         ptrdiff_t operator-(const const_iterator& r) const
         {
            return m_data_p - r.m_data_p;
         }

         bool operator==(const const_iterator& r) const noexcept
         {
            return m_data_p == r.m_data_p;
         }

         bool operator!=(const const_iterator& r) const noexcept
         {
            return !(*this == r);
         }

         bool operator<(const const_iterator& r) const noexcept
         {
            return m_data_p < r.m_data_p;
         }

         bool operator>(const const_iterator& r) const noexcept
         {
            return !(*this > r);
         }

         bool operator>=(const const_iterator& r) const noexcept
         {
            return !(*this < r);
         }

         bool operator<=(const const_iterator& r) const noexcept
         {
            return !(*this > r);
         }

         private:
         const T* m_data_p;
         SRWTraits m_traits;
      };

      static_assert(std::is_copy_constructible<const_iterator>::value,
                    "Slim Vector Iterator is not copy constructible");
      static_assert(std::is_copy_assignable<const_iterator>::value,
                    "Slim Vector Iterator is not copy assignable");
      static_assert(std::is_destructible<const_iterator>::value,
                    "Slim Vector Iterator is not destructible");

      slim_vector(SRWTraits traits = SRWTraits()) :
         m_traits(traits)
      {

      }

      template<class InputIt>
      slim_vector(InputIt first, InputIt last,
                  SRWTraits traits = SRWTraits()) :
         m_traits(traits),
         m_data(first, last)
      {

      }

      slim_vector(std::initializer_list<T> init,
                  SRWTraits traits = SRWTraits()) :
         m_traits(traits),
         m_data(init)
      {

      }

      slim_vector(const slim_vector& r)
      {
         // Set the vector to read mode and copy its data.
         r.m_traits.share_lock();
         m_data = r.m_data;
         r.m_traits.share_release();
      }

      slim_vector(slim_vector&& r)
      {
         r.m_traits.excl_lock();
         m_data = std::move(r.m_data);
         r.m_traits.excl_release();
      }

      ~slim_vector() noexcept
      {
         m_traits.excl_lock();

         // Underlying vector is disposed.
         // Traits disposed now and will release the exclusive lock.
      }

      /*
       Swaps the contents of the two slim vectors.
       */
      friend void swap(slim_vector& l, slim_vector& r)
      {
         // Set the vectors to exclusive mode.
         l.m_traits.excl_lock();
         r.m_traits.excl_lock();

         // Swap the contents
         using std::swap;
         swap(l.m_traits, r.m_traits);
         swap(l.m_data, r.m_data);

         // Release the exclusive locks.
         r.m_traits.excl_release();
         l.m_traits.excl_release();
      }

      /*
       Copy assign
       */
      slim_vector& operator=(slim_vector r)
      {
         swap(*this, r);
         return *this;
      }

      /*
       Returns a pointer to the raw array and an exclusive lock.
       */
      std::pair<T*, const SRWTraits> data() noexcept
      {
         // Make a copy of the lock (That will be returned) and set it to 
         // exclusive mode.
         SRWTraits exclLock{ m_traits };
         exclLock.excl_lock();

         return std::make_pair<T*, const SRWTraits>(
            m_data.data(), std::move(exclLock));
      }

      /*
       Returns a pointer to the raw array and an exclusive lock.
       */
      const std::pair<const T*, const SRWTraits> data() const noexcept
      {
         // Make a copy of the lock (That will be returned) and set it to 
         // exclusive mode.
         SRWTraits exclLock{ m_traits };
         exclLock.excl_lock();

         return std::make_pair<const T*, const SRWTraits>(
            m_data.data(), std::move(exclLock));
      }

      const_iterator begin() const noexcept
      {
         return const_iterator{ m_data.data(), m_traits };
      }

      const_iterator end() const noexcept
      {
         return const_iterator{ m_data.data() + m_data.size(), m_traits };
      }

      const_iterator cbegin() const noexcept
      {
         return const_iterator{ m_data.data(), m_traits };
      }

      const_iterator cend() const noexcept
      {
         return const_iterator{ m_data.data() + m_data.size(), m_traits };
      }

      /*
       Returns a reference to the pos'th element and a shared lock.
       */
      value_type at(size_t pos)
      {
         SRWTraits sharedLock{ m_traits };
         sharedLock.share_lock();
         if (pos > m_data.size() - 1)
         {
            throw std::out_of_range{ "Position is out of range." };
         }

         return std::pair<T&, SRWTraits>(
            m_data[pos], sharedLock);
      }

      /*
       Returns a const reference to the pos'th element and a shared lock.
       */
      const_value_type at(size_t pos) const
      {
         SRWTraits sharedLock{ m_traits };
         sharedLock.share_lock();
         if (pos > m_data.size() - 1)
         {
            throw std::out_of_range{ "Position is out of range." };
         }

         return std::pair<const T&, SRWTraits>(
            m_data[pos], sharedLock);
      }

      /*
       Returns a reference to the pos'th element and a shared lock.
       */
      value_type operator[](size_t pos)
      {
         return at(pos);
      }

      /*
       Returns a const reference to the pos'th element and a shared lock.
       */
      const_value_type operator[](size_t pos) const
      {
         return at(pos);
      }

      /*
       Returns a reference to the first element and a shared lock.
       Throws std::out_of_range if the vector is empty.
       */
      value_type front()
      {
         SRWTraits sharedLock{ m_traits };
         sharedLock.share_lock();
         if (m_data.size() == 0)
         {
            throw std::out_of_range{ "Vector is empty." };
         }

         return std::pair<T&, SRWTraits>(
            m_data.front(), std::move(sharedLock));
      }

      /*
       Returns a const reference to the first element and a shared lock.
       Throws std::out_of_range if the vector is empty.
       */
      const_value_type front() const
      {
         SRWTraits sharedLock{ m_traits };
         sharedLock.share_lock();
         if (m_data.size() == 0)
         {
            throw std::out_of_range{ "Vector is empty." };
         }

         return std::pair<const T&, SRWTraits>(
            m_data.front(), std::move(sharedLock));
      }

      /*
       Returns a reference to the last element and a shared lock.
       Throws std::out_of_range if the vector is empty.
       */
      value_type back()
      {
         SRWTraits sharedLock{ m_traits };
         sharedLock.share_lock();
         if (m_data.size() == 0)
         {
            throw std::out_of_range{ "Vector is empty." };
         }

         return std::pair<T&, SRWTraits>(
            m_data.back(), std::move(sharedLock));
      }

      /*
       Returns a const reference to the last element and a shared lock.
       Throws std::out_of_range if the vector is empty.
       */
      const_value_type back() const
      {
         SRWTraits sharedLock{ m_traits };
         sharedLock.share_lock();
         if (m_data.size() == 0)
         {
            throw std::out_of_range{ "Vector is empty." };
         }

         return std::pair<const T&, SRWTraits>(
            m_data.back(), std::move(sharedLock));
      }

      /*
       Obtains a shared lock and checks if the vector is empty.
       */
      [[nodiscard]] bool empty() const
      {
         m_traits.share_lock();
         auto ret = m_data.empty();
         m_traits.share_release();

         return ret;
      }

      /*
       Obtains a shared lock and returns the number of elements in the vector.
       */
      [[nodiscard]] size_t size() const
      {
         m_traits.share_lock();
         auto ret = m_data.size();
         m_traits.share_release();

         return ret;
      }

      /*
       Acquires an exclusive lock and clears the vector.
       */
      void clear()
      {
         m_traits.excl_lock();
         m_data.clear();
         m_traits.excl_release();
      }

      /*
       Acquires an exclusive lock and appends value to the end of the vector.
       */
      void push_back(const T& value)
      {
         m_traits.excl_lock();
         m_data.push_back(value);
         m_traits.excl_release();
      }

      /*
       Acquires an exclusive lock and moves value to the end of the vector.
       */
      void push_back(T&& value)
      {
         m_traits.excl_lock();
         m_data.push_back(value);
         m_traits.excl_release();
      }

      /*
       Acquires an exclusive lock and removes the last element in the vector.
       */
      void pop_back()
      {
         m_traits.excl_lock();
         m_data.pop_back();
         m_traits.excl_release();
      }

      /*
       Acquires an exclusive lock and removes the element at the given position.
       */
      void erase(const_iterator pos)
      {
         m_traits.excl_lock();
         // Convert the const iterator position to an iterator that the vector
         // can understand
         auto itPos = end() - pos;

         // Erase using the vector iterator.
         m_data.erase(m_data.begin() + itPos);
         m_traits.excl_release();
      }

      /*
       Acquires an exclusive lock and removes the elements in the give range.
       */
      void erase(const_iterator first, const_iterator last)
      {
         m_traits.excl_lock();
         auto firstItPos = end() - first;
         auto lastItPos = end() - last;
         m_data.erase(m_data.begin() + firstItPos, m_data.begin() + lastItPos);
         m_traits.excl_release();
      }

      /*
       Acquires an exclusive lock and resizes the vector to the new size.
       New elements are default constructed.
       */
      void resize(size_t count)
      {
         m_traits.excl_lock();
         m_data.resize(count);
         m_traits.excl_release();
      }

      /*
       Acquires an exclusive lock and resizes the vector to the new size and
       fills any new elements with copies of value.
       */
      void resize(size_t count, const T& value)
      {
         m_traits.excl_lock();
         m_data.resize(count, value);
         m_traits.excl_release();
      }

      private:
      data_t m_data;

      /*
       In exclusive mode, this controls access to the entire vector.
       In shared mode, this controls access to individual elements in the
       vector.
       */
      mutable SRWTraits m_traits;
   };
}