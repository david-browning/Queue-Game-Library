#pragma once
#include "include/qgl_model_include.h"
#include <QGLPlatform.h>

namespace qgl
{
#pragma warning(push)
#pragma warning(disable : 26110)
   class win32_srw_traits final
   {
      public:
      win32_srw_traits() :
         m_lock(SRWLOCK_INIT),
         m_mode(lock_modes::none)
      {
         
      }

      /*
       Cloning an SRW that is in exclusive mode will put this in "none" mode.
       */
      win32_srw_traits(const win32_srw_traits& r) :
         m_lock(r.m_lock),
         m_mode(r.m_mode)
      {
         switch (m_mode)
         {
            case lock_modes::exclusive:
            {
               m_mode = lock_modes::none;
               break;
            }
            case lock_modes::shared:
            {
               share_lock();
               break;
            }
         }
      }

      win32_srw_traits(win32_srw_traits&& r) noexcept
      {
         m_mode = r.m_mode;
         m_lock = r.m_lock;

         // Make is so when r is disposed, it doesn't free the lock because it
         // was moved.
         r.m_mode = lock_modes::none;
      }

      ~win32_srw_traits()
      {
         check_and_release();
      }

      friend void swap(win32_srw_traits& l, win32_srw_traits& r) noexcept
      {
         using std::swap;
         swap(l.m_lock, r.m_lock);
         swap(l.m_mode, r.m_mode);
      }

      win32_srw_traits& operator=(win32_srw_traits r)
      {
         swap(*this, r);
         return *this;
      }

      void share_lock()
      {
         check_and_release();
         AcquireSRWLockShared(&m_lock);
         m_mode = lock_modes::shared;
      }

      void excl_lock()
      {
         check_and_release();
         AcquireSRWLockExclusive(&m_lock);
         m_mode = lock_modes::exclusive;
      }

      void share_release()
      {
         if (m_mode != lock_modes::shared)
         {
            throw std::runtime_error{
               "The lock was not acquired in shared mode." };
         }

         ReleaseSRWLockShared(&m_lock);
         m_mode = lock_modes::none;
      }

      void excl_release()
      {
         if (m_mode != lock_modes::exclusive)
         {
            throw std::runtime_error{
               "The lock was not acquired in exclusive mode." };
         }

         ReleaseSRWLockExclusive(&m_lock);
         m_mode = lock_modes::none;
      }

      private:
      void check_and_release()
      {
         switch (m_mode)
         {
            case lock_modes::shared:
            {
               share_release();
               break;
            }
            case lock_modes::exclusive:
            {
               excl_release();
               break;
            }
         }

#ifdef DEBUG
         m_mode = lock_modes::none;
#endif
      }

      enum class lock_modes
      {
         none,
         shared,
         exclusive,
      };

      lock_modes m_mode;
      SRWLOCK m_lock;
   };
#pragma warning(pop)
}