#pragma once
#include "include/qgl_model_include.h"
#include "include/qgl_guid.h"

namespace qgl
{
   /*
    Commands execute an action. Commands can be located using a GUID. These are
    basically functors with an undo command and a GUID.
    Override "execute" and "undo". If it is not possible to undo a command, 
    then override "undo" to do nothing.
    Provides ==, !=, and < operators for use in maps.
    */
   template<class ExecuteArgs>
   class icommand
   {
      public:
      /*
       Copies the contents of g. g can be freed or go out of scope.
       */
      icommand(const qgl::guid& g) noexcept :
         m_guid(g)
      {
       
      }

      icommand(qgl::guid&& g) noexcept :
         m_guid(g)
      {

      }

      /*
       Copy constructor.
       */
      icommand(const icommand&) noexcept = default;

      /*
       Move constructor.
       */
      icommand(icommand&&) noexcept = default;

      /*
       Destructor.
       */
      virtual ~icommand() noexcept = default;

      /*
       Executes this command.
       */
      virtual void execute(ExecuteArgs args) = 0;

      /*
       Undoes this command if possible. If not possible, this does nothing.
       */
      virtual void undo() = 0;

      /*
       Executes the command.
       */
      void operator()()
      {
         execute();
      }

      /*
       Returns a const pointer to this component's GUID.
       */
      const qgl::guid& guid() const noexcept
      {
         return m_guid;
      }

      /*
       Returns true if the GUIDs are equal.
       */
      friend bool operator==(const icommand& l, const icommand& r)
      {
         return l.guid() == r.guid();
      }

      /*
       Returns true if the GUIDs are not equal.
       */
      friend bool operator!=(const icommand& l, const icommand& r)
      {
         return !(l == r);
      }

      /*
       Returns true if l's GUID is lexicographically less than r's GUID.
       */
      friend bool operator<(const icommand& l, const icommand& r)
      {
         return l.guid() < r.guid();
      }

      private:
      qgl::guid m_guid;
   };
}