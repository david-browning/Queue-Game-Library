#pragma once
#include "include/qgl_iobserver.h"

namespace qgl
{
    /*
     A callback observer is a concrete version of iobserver. Instead of 
     listening to a subject using the update function, this raises a callback
     when the subject notifies this observer.
     */
    template<class MessageT,
    std::function<void(MessageT)>& Callback>
    class callback_observer : public iobserver<MessageT>
    {
        public:
        callback_observer()
        {

        }

        callback_observer(const callback_observer& c) = default;

        callback_observer(callback_observer&& m) = default;

        virtual ~callback_observer() noexcept = default;

        protected:
        virtual void update(MessageT msg)
        {
            Callback(msg);
        }
    };
}