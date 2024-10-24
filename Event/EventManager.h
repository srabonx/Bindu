//
// Created by letsd on 22-Jul-24.
//

#ifndef BINDU_EVENTMANAGER_H
#define BINDU_EVENTMANAGER_H

#include <functional>

#include "EventPool.h"


namespace BINDU {
    namespace EVENT
    {
        struct BND_Event;
    }
    class IEventListener;

    using EventCallbackFn = std::function<void(EVENT::BND_Event&)>;

    using EventCallbackToken = std::uint32_t;

    class EventManager
    {
    public:

        static void PushEvent(const EVENT::BND_Event& event);

        static void DispatchEvents();

        static void AddListener(IEventListener* eventListener) ;

        static void RemoveListener(IEventListener* eventListener);

        [[nodiscard]]static EventCallbackToken BindListenerFn(const EventCallbackFn& callback);

        static void RemoveListenerFn(EventCallbackToken token);

        static void Clear();

    private:

        static EventPool                   m_eventPool;
        static std::list<IEventListener*>  m_eventListeners;
        static std::vector<std::pair<EventCallbackToken,EventCallbackFn>>  m_eventListenerFns;
        static EventCallbackToken          m_tokens;
    };

#define BINDU_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) {return this->fn(std::forward<decltype(args)>(args)...);}


} // BINDU

#endif //BINDU_EVENTMANAGER_H
