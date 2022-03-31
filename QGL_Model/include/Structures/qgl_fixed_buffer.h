#pragma once

namespace qgl
{
   // Type safe, statically allocated buffer safe for serialization.
   template<typename T, size_t n>
   class fixed_buffer final
   {
      public:
      constexpr fixed_buffer()
      {

      }

      /*
       Copies the buffer.
       */
      constexpr fixed_buffer(const T* buf, size_t len)
      {
         assert(size() >= len);
         mem::copy_elements(m_data, buf, len);
      }

      constexpr size_t size() const noexcept
      {
         return n;
      }

      constexpr const T* data() const noexcept
      {
         return m_data;
      }

      constexpr T* data() noexcept
      {
         return m_data;
      }

      constexpr const T& at(size_t i) const
      {
         if (i > n - 1)
         {
            throw std::out_of_range{"Index is greater than size."};
         }

         return m_data[i];
      }

      constexpr T& at(size_t i)
      {
         if (i > n - 1)
         {
            throw std::out_of_range{ "Index is greater than size." };
         }

         return m_data[i];
      }

      constexpr const T& operator[](size_t i) const
      {
         return at(i);
      }

      constexpr T& operator[](size_t i)
      {
         return at(i);
      }

      friend void swap(fixed_buffer& l, fixed_buffer& r) noexcept
      {
         using std::swap;
         mem::swap_elements(l.data(), r.data(), r.size());
      }

      fixed_buffer& operator=(fixed_buffer r)
      {
         swap(*this, r);
         return *this;
      }

      private:
      T m_data[n] = { 0 };
   };
}