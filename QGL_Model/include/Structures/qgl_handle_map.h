#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   template<typename T>
   class handle_map
   {
      public:
      using handle_t = typename uintptr_t;
      static constexpr handle_t INVALID_HANDLE = static_cast<handle_t>(-1);

      handle_map()
      {

      }

      handle_map(const handle_map&) = delete;

      handle_map(handle_map&& x) noexcept :
         m_handles(std::move(x.m_handles)),
         m_nextHandle(m_nextHandle.load())
      {

      }

      ~handle_map() noexcept = default;

      /*
       Moves the object to internal storage and allocates a handle for it.
       */
      handle_t alloc(T&& obj)
      {
         // Do not allow multiple threads to allocate or free at the same time.
         std::lock_guard allocationLock{ m_allocationMutex };
         auto p = m_handles.emplace(m_nextHandle, std::forward<T>(obj));
         if (!p.second)
         {
            throw std::bad_alloc();
         }

         auto ret = m_nextHandle.load();
         m_nextHandle++;
         return ret;
      }

      /*
       Remove the allocated resource from the handle map.
       */
      handle_t free(handle_t h)
      {
         // Do not allow multiple threads to allocate or free at the same time.
         std::lock_guard allocationLock{ m_allocationMutex };
         m_handles.erase(h);
         return INVALID_HANDLE;
      }

      /*
       Number of handles currently allocated.
       */
      size_t size() const noexcept
      {
         return m_handles.size();
      }

      /*
       True if the given handle is valid.
       */
      bool allocated(handle_t h) const noexcept
      {
         return m_handles.count(h) > 0;
      }

      /*
       Throws std::out_of_range if the handle was not allocated.
       */
      T& get(handle_t h)
      {
         return m_handles.at(h);
      }

      /*
       Throws std::out_of_range if the handle was not allocated.
       */
      const T& get(handle_t h) const
      {
         return m_handles.at(h);
      }

      private:
      /*
       Used to block multiple threads from allocating simultaneously.
       */
      std::mutex m_allocationMutex;

      std::atomic<handle_t> m_nextHandle = 0;

      /*
       Maps handles to the actual resource.
       */
      std::unordered_map<handle_t, T> m_handles;
   };
}