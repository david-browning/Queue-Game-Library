#pragma once
#include "include/qgl_iobserver.h"

namespace qgl
{
    /*
     A callback observer is a concrete version of iobserver. Instead of 
     listening to a subject using the update function, this raises a callback
     when the subject notifies this observer.
     */
    template<class MessageT>
    class callback_observer : public iobserver<MessageT>
    {
        public:
        using CallbackT = std::function<void(MessageT)>;
               
        callback_observer(CallbackT callback) :
            m_callback(callback)
        {

        }

        callback_observer(const callback_observer& c) = default;

        callback_observer(callback_observer&& m) = default;

        virtual ~callback_observer() noexcept = default;

        protected:
        virtual void update(MessageT msg)
        {
            m_callback(msg);
        }

        private:
        CallbackT m_callback;
    };

    template<class MessageT,
             class ClassT>
    class member_callback_observer : public iobserver<MessageT>
    {
        public:
        using MemberCallbackT = std::function<void(ClassT&, MessageT)>;

        member_callback_observer(ClassT& memberclass, 
                                 MemberCallbackT callback) :
            m_callback(callback),
            m_class(memberclass)
        {

        }

        member_callback_observer(const member_callback_observer&) = default;

        member_callback_observer(member_callback_observer&&) = default;

        virtual ~member_callback_observer() noexcept = default;

        protected:
        virtual void update(MessageT msg)
        {
            m_callback(m_class.get(), msg);
        }

        private:
        MemberCallbackT m_callback;
        std::reference_wrapper<ClassT> m_class;
    };
}