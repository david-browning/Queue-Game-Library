#pragma once
#include "qgl_graphics_include.h"
#include "qgl_iresident_metric.h"

namespace qgl::graphics::low
{
   class iresident
   {
      public:
      iresident(const winrt::com_ptr<d3d_device>& dev_p) :
         m_metrics(),
         m_isResident(false)
      {
      }

      /*
       This does not perform a deep copy on GPU resources.
       */
      iresident(const iresident& r) :
         m_metrics(r.m_metrics),
         m_isResident(r.m_isResident)
      {

      }

      /*
       Move constructor
       */
      iresident(iresident&& r) :
         m_metrics(std::move(r.m_metrics)),
         m_isResident(r.m_isResident)
      {

      }

      virtual ~iresident() noexcept
      {
         m_isResident = false;
      }

      /*
       Returns true if the resource is on the GPU.
       */
      inline bool resident() const
      {
         return m_isResident;
      }

      void make_resident()
      {
         if (!m_isResident)
         {

         }
      }

      void evict()
      {
         if (m_isResident)
         {

         }
      }

      protected:
      /*
       Metrics used to evaluate how important the resource is.
       The developer should not have to set these parameters. Instead, evict,
       make_resident, and upload, should do it.
       */
      mutable resident_metric m_metrics;

      private:
      mutable bool m_isResident;
   };
}