#pragma once
#include "include/qgl_model_include.h"
#include "include/Structures/qgl_ts_umap.h"
#include "include/qgl_map_key_iterator.h"

namespace qgl
{
   using hndlmap_t = typename uintptr_t;

   template<typename T, typename HandleT = hndlmap_t>
   class handle_map final
   {
      public:
      static constexpr HandleT INVALID_HANDLE = static_cast<HandleT>(-1);

      handle_map()
      {

      }

      handle_map(const handle_map&) = delete;

      handle_map(handle_map&& r) noexcept
      {
         // Block r from making changes.
         std::lock_guard l{ r.m_allocationMutex };
         m_handles = std::move(r.m_handles);
         m_nextHandle.store(r.m_nextHandle.load());
      }

      ~handle_map() noexcept
      {
         clear();
      }

      handle_map& operator=(handle_map&& r) noexcept
      {
         std::scoped_lock l{ r.m_allocationMutex };
         m_nextHandle.store(r.m_nextHandle.load());
         m_handles = std::move(r.m_handles);
      }

      void clear()
      {
         std::lock_guard allocationLock{ m_allocationMutex };
         m_handles.clear();
         m_nextHandle.store(0);
      }

      /*
       Moves the object to internal storage and allocates a handle for it.
       */
      HandleT alloc(T&& obj)
      {
         // Do not allow multiple threads to allocate or free at the same time.
         std::lock_guard allocationLock{ m_allocationMutex };
         auto p = m_handles.emplace(m_nextHandle, std::forward<T>(obj));
         if (!p.second)
         {
            throw std::bad_alloc{};
         }

         auto ret = m_nextHandle.load();
         m_nextHandle++;
         return ret;
      }

      /*
       Remove the allocated resource from the handle map. Returns the invalid
       handle value.
       */
      HandleT free(const HandleT& h)
      {
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
      bool allocated(const HandleT& h) const noexcept
      {
         return m_handles.count(h) > 0;
      }

      /*
       Throws std::out_of_range if the handle was not allocated.
       */
      T& get(const HandleT& h)
      {
         return m_handles.at(h);
      }

      /*
       Throws std::out_of_range if the handle was not allocated.
       */
      const T& get(const HandleT& h) const
      {
         return m_handles.at(h);
      }

      auto cbegin() noexcept
      {
         return map_key_const_iterator<HandleT, decltype(m_handles)>{
            m_handles
         };
      }

      auto cend() noexcept
      {
         return map_key_const_iterator<HandleT, decltype(m_handles)>{
            m_handles,
            nullptr
         };
      }

      private:
      /*
       Used to block multiple threads from allocating simultaneously.
       */
      std::mutex m_allocationMutex;

      std::atomic<HandleT> m_nextHandle = 0;

      /*
       Maps handles to the actual resource.
       */
      ts_umap<HandleT, T> m_handles;
   };
}