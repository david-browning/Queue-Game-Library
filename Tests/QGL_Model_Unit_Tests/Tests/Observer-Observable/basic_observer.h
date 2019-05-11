#pragma once
#include <QGLModel.h>

using namespace qgl;

namespace QGL_Model_Unit_Tests
{
   template<class MessageT>
   class basic_observer : public iobserver<MessageT>
   {
      public:
      basic_observer()
      {

      }

      basic_observer(const basic_observer& c) = default;

      basic_observer(basic_observer&& m) = default;

      virtual ~basic_observer() noexcept
      {

      }

      const MessageT& state() const noexcept
      {
         return m_state;
      }

      protected:
      virtual void update(MessageT msg)
      {
         m_state = msg;
      }

      private:
      MessageT m_state;
   };
}