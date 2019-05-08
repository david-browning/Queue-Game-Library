#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics::low
{
   class resident_metric
   {
      public:
      /*
       Number of times the resource has been accessed.
       The resource is "accessed" whenever it is updated, read on the CPU,
       or bound using a graphics command list.
       */
      inline uint64_t accesses() const
      {
         return m_accesses;
      }

      /*
       Number of times the resource has been uploaded. This should be the same or
       1 greater than the number of times evicted.
       */
      inline uint64_t uploads() const
      {
         return m_uploads;
      }

      /*
       Number of times the resource has been evicted.
       */
      inline uint64_t evicts() const
      {
         return m_evicts;
      }

      /*
       Time (in milliseconds) that the resource is resident.
       */
      inline uint64_t time() const
      {
         return m_time;
      }

      inline void verify() const
      {
         #ifdef DEBUG
         if (m_evicts == m_uploads ||
             m_evicts + 1 == m_uploads)
         {
            return;
         }

         throw std::bad_function_call();
         #endif
      }

      /*
       Adds the delta to the number of times the resources has been accessed.
       The resource is "accessed" whenever it is updated, read on the CPU,
       or bound using a graphics command list.
       iresident calls this function whenever those parameters are met.
       */
      inline void accesses(uint64_t delta)
      {
         m_accesses += delta;
      }

      /*
       Adds the delta to the number of times the resources has been uploaded.
       */
      inline void uploads(uint64_t delta)
      {
         m_uploads += delta;
      }

      /*
       Adds the delta to the number of times the resource has been evicted.
       */
      inline void evicts(uint64_t delta)
      {
         m_evicts += delta;
      }

      /*
       Adds the delta (measured in milliseconds) to the amount of time the resource
       has been resident.
       */
      inline void time(uint64_t delta)
      {
         m_time += delta;
      }

      private:
      uint64_t m_accesses;
      uint64_t m_uploads;
      uint64_t m_evicts;
      uint64_t m_time;
   };
}