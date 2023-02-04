#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   template<class IntrinsicT, class ContextT>
   class flyweight
   {
      public:
      flyweight(const IntrinsicT& intrinsic, const ContextT& cnxt) :
         m_intrinsicData(intrinsic),
         m_extrinsicData(cnxt)
      {

      }

      
      flyweight(const IntrinsicT& intrinsic, ContextT&& cnxt) :
         m_intrinsicData(intrinsic),
         m_extrinsicData(std::forward<ContextT>(cnxt))
      {

      }

      flyweight(const flyweight&) = default;

      flyweight(flyweight&&) noexcept = default;

      virtual ~flyweight() noexcept = default;

      friend void swap(flyweight& l, flyweight& r) noexcept
      {
         using std::swap;
         swap(l.m_intrinsicData, r.m_intrinsicData);
         swap(l.m_extrinsicData, r.m_extrinsicData);
      }

      flyweight& operator=(flyweight r)
      {
         swap(*this, r);
         return *this;
      }

      const IntrinsicT& intrinsic() const noexcept
      {
         return m_extrinsicData.get();
      }

      ContextT& context() noexcept
      {
         return m_extrinsicData;
      }

      const ContextT& context() const noexcept
      {
         return m_extrinsicData;
      }

      private:
      std::reference_wrapper<IntrinsicT> m_intrinsicData;
      ContextT m_extrinsicData;
   };
}