#pragma once
#include "include/qgl_model_include.h"
#include <set>

namespace qgl
{


	/*
	 Abstract class that observes a subject. Aka subscriber, watcher.
	 Overwrite update to implement.
	 Observer keeps track of subjects that notify this so when this observer
	 is destroyed it can remove itself from the subjects that link to it.
	 MessageT: Type of data that gets passed to this observer when a subjects
	 notifies this.
	 */
	template<class MessageT>
	class iobserver
	{
		public:
                
        /*
         Forward declaration of a subject.
         This is so we can mark subject as a friend of iobservable and make
         iobservable::update(), subscribe, and unsubscribe private.
         This prevents programmers from calling update outside of its intended
         scope.
         subject gets complete access to this class so it can call the private
         member functions.
         */
        friend class subject<MessageT>;

		/*
		 Constructor
		 */
		iobserver() = default;

		/*
		 Copy constructor.
		 */
		iobserver(const iobserver& c) :
			m_notifier_s(c.m_notifier_s)
		{
			for (auto& sbj : m_notifier_s)
			{
				sbj.get().add(this);
			}
		}

		/*
		 Move constructor.
		 */
		iobserver(iobserver&& m) :
			m_notifier_s()
		{
			//The pointer of this has probably changed.
			//Need to update the notifiers' collection of observers.

			//For each subject that m was linked to:
			for (auto& sbj : m.m_notifier_s)
			{
				//Remove m from the subject
				sbj.get().remove(&m);

				//Add this to the subject
				sbj.get().add(this);
			}
		}

		/*
		 Destructor
		 */
		virtual ~iobserver()
		{
			//Unsubscribe from all subjects.
			for (auto& sbj : m_notifier_s)
			{
				sbj.get().remove(this);
			}
		}

        protected:
        /*
         Overwrite this function with the desired logic when the subject
         updates this.
         */
        virtual void update(const MessageT& msg) = 0;

		private:
        
		/*
		 Adds this to the subject's list of observers.
		 */
		void subscribe(subject<MessageT>& sbj_p)
		{
			//This observer now holds a reference to the subject that 
			//notifies this.
			m_notifier_s.insert(sbj_p);
		}

		/*
		 Remove this from the subject's list of observers.
		 */
		void unsubscribe(subject<MessageT>& sbj_p)
		{
			//This observer no longer references that subject.
			m_notifier_s.erase(sbj_p);
		}

		/*
		 Keep track of everything that notifies this so when this is destroyed
		 we can remove it from all the subjects.
		 */
		std::set<std::reference_wrapper<subject<MessageT>>> m_notifier_s;
	};
}