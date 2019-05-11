#pragma once
#include "include/qgl_model_include.h"
#include "include/Observer-Observable/qgl_iobserver.h"
#include <set>

namespace qgl
{
    /*
     A subject sends messages to observers using the notify method.
     MessageT: Type of message to pass to the observers. Recommend keeping
     MessageT <= 8 bytes because it cannot be passed by reference.
     */
   template<class MessageT>
   class subject
   {
      public:

      /*
       Creates a subject with no observers.
       */
      subject()
      {

      }

      /*
       Uses the first and last iterators to create a list of observers for
       this subject. Dereferencing the iterator must yield a pointer to an
       iobserver.
       */
      template<class InputIterator>
      subject(InputIterator first, InputIterator last)
      {
         while (first != last)
         {
            add(*first);
            first++;
         }
      }

      /*
       Create a list of observers from the initializer list for this subject.
       */
      subject(std::initializer_list<iobserver<MessageT>*> observer_ps)
      {
         for (auto observer_p : observer_ps)
         {
            add(observer_p);
         }
      }

      /*
       Copy Constructor
       */
      subject(const subject& c) :
         m_observer_ps(c.m_observer_ps)
      {
         for (auto observer : m_observer_ps)
         {
            observer->subscribe(this);
         }
      }

      /*
       Move Constructor
       */
      subject(subject&& m) :
         m_observer_ps()
      {
          //This pointer may have changed. Update the observers' links to 
          //this subject.

          //For each observer m had:
         for (auto observer_p : m.m_observer_ps)
         {
             //The observer no longer subscribes to m.
            observer_p->unsubscribe(&m);

            //It subscribes to this now.
            add(observer_p);
         }
      }

      /*
      Destructor
      */
      virtual ~subject() noexcept
      {
          //When this is destroyed, make sure all the observers are no longer
          //linked to this.
         for (auto observer : m_observer_ps)
         {
            observer->unsubscribe(this);
         }
      }

      void add(iobserver<MessageT>* const observer_p)
      {
          //This keeps track of the observer.
         m_observer_ps.insert(observer_p);

         //Observer keeps track of it's subjects.
         observer_p->subscribe(this);
      }

      void remove(iobserver<MessageT>* const observer_p)
      {
          //This no longer notifies the observer.
         m_observer_ps.erase(observer_p);

         //Observer is no longer linked to this.
         observer_p->unsubscribe(this);
      }

      /*
       Sends the message to all observers.
       msg is not a reference so this function can be called across threads.
       */
      void notify(MessageT msg)
      {
         for (auto observer : m_observer_ps)
         {
            observer->update(msg);
         }
      }

      /*
       Returns the number of observers.
       */
      auto count() const noexcept
      {
         return m_observer_ps.size();
      }

      private:

      template<class msgt>
      friend class iobserver;

      void destroy(iobserver<MessageT>* const observer_p)
      {
         m_observer_ps.erase(observer_p);
      }

      std::set<iobserver<MessageT>*> m_observer_ps;
   };
}