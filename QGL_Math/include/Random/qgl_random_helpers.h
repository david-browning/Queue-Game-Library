#pragma once
#include "include/qgl_math_includes.h"
#include <random>

namespace qgl::math::random
{
   using qgl_default_random_engine = std::mt19937;

   inline qgl_default_random_engine& engine()
   {
      static qgl_default_random_engine eng{ std::random_device{}() };
      return eng;
   }

   inline qgl_default_random_engine& seeded_engine(uint32_t seed)
   {
      static qgl_default_random_engine eng{ seed };
      return eng;
   }

   template<typename T>
   inline T uniform_int(T min, T max)
   {
      static_assert(std::is_integral_v<T>);
      std::uniform_int_distribution<T> dist(min, max);
      return dist(engine());
   }

   template<typename T>
   inline T uniform_int(T min, T max, uint32_t seed)
   {
      static_assert(std::is_integral_v<T>);
      std::uniform_int_distribution<T> dist(min, max);
      return dist(seeded_engine(seed));
   }

   template<typename T>
   inline T uniform_real(T min, T max)
   {
      static_assert(std::is_floating_point_v<T>);
      std::uniform_real_distribution<T> dist(min, max);
      return dist(engine());
   }

   template<typename T>
   inline T uniform_real(T min, T max, uint32_t seed)
   {
      static_assert(std::is_floating_point_v<T>);
      std::uniform_real_distribution<T> dist(min, max);
      return dist(seeded_engine(seed));
   }

   template<typename T>
   inline T normal_real(T mean, T stdDev)
   {
      static_assert(std::is_floating_point_v<T>);
      std::normal_distribution<T> dist(mean, stdDev);
      return dist(engine());
   }

   template<typename ContainerT>
   inline decltype(auto) choose(const ContainerT& container)
   {
      auto it = container.begin();
      std::advance(it, uniform_int<size_t>(0, container.size() - 1));
      return *it;
   }

   template<typename RandomIt>
   inline decltype(auto) choose(RandomIt first, RandomIt last)
   {
      auto size = std::distance(first, last);
      if (size <= 0)
      {
         throw std::out_of_range("Range is empty");
      }

      std::advance(first, uniform_int<ptrdiff_t>(0, size - 1));
      return *first;
   }
}