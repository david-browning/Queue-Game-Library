#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   /*
    An either-pointer encapsulates either a shared pointer or a unique pointer.
    */
   template<typename T>
   class eptr final
   {
      public:
      using sp = typename std::shared_ptr<T>;
      using up = typename std::unique_ptr<T>;

      eptr(const sp& ptr)
      {
         m_underlying.emplace<sp>(ptr);
      }

      eptr(sp&& ptr)
      {
         m_underlying.emplace<sp>(std::forward<sp>(ptr));
      }

      eptr(up&& ptr)
      {
         m_underlying.emplace<up>(std::forward<up>(ptr));
      }

      eptr(const T& obj)
      {
         m_underlying.emplace<up>(std::make_unique<T>(obj));
      }

      eptr(T&& obj)
      {
         m_underlying.emplace<up>(
            std::make_unique<T>(
               std::forward<T>(obj)));
      }

      eptr(const eptr&) = default;

      eptr(eptr&&) = delete;

      ~eptr() noexcept = default;

      constexpr bool shared() const noexcept
      {
         return std::holds_alternative<sp>(m_underlying);
      }

      constexpr bool unique() const noexcept
      {
         return std::holds_alternative<up>(m_underlying);
      }

      const T* get() const noexcept
      {
         if (shared())
         {
            return std::get<sp>(m_underlying).get();
         }
         else
         {
            return std::get<up>(m_underlying).get();
         }
      }

      T* get() noexcept
      {
         if (shared())
         {
            return std::get<sp>(m_underlying).get();
         }
         else
         {
            return std::get<up>(m_underlying).get();
         }
      }

      eptr& operator=(eptr&& other) noexcept
      {
         if (this != &other)
         {
            m_underlying = std::move(other.m_underlying);
         }

         return *this;
      }

      explicit constexpr operator bool() const noexcept
      {
         if (shared())
         {
            return std::get<sp>(m_underlying).operator bool();
         }
         else
         {
            return std::get<up>(m_underlying).operator bool();
         }
      }

      T* operator->() noexcept
      {
         if (shared())
         {
            return std::get<sp>(m_underlying).get();
         }
         else
         {
            return std::get<up>(m_underlying).get();
         }
      }

      const T* operator->() const noexcept
      {
         if (shared())
         {
            return std::get<sp>(m_underlying).get();
         }
         else
         {
            return std::get<up>(m_underlying).get();
         }
      }

      T& operator*() noexcept
      {
         if (shared())
         {
            return *std::get<sp>(m_underlying);
         }
         else
         {
            return *std::get<up>(m_underlying);
         }
      }

      const T& operator*() const noexcept
      {
         if (shared())
         {
            return *std::get<sp>(m_underlying);
         }
         else
         {
            return *std::get<up>(m_underlying);
         }
      }

      friend void swap(eptr& left, eptr& right) noexcept
      {
         std::swap(left.m_underlying, right.m_underlying);
      }

      private:
      std::variant<sp, up> m_underlying;
   };
}