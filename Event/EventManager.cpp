//
// Created by letsd on 22-Jul-24.
//

#include <list>
#include "EventManager.h"
#include "EventPool.h"
#include "EventStruct.h"
#include "IEventListener.h"

namespace BINDU
{

    EventPool                       EventManager::m_eventPool;
    std::list<IEventListener*>      EventManager::m_eventListeners;
    std::vector<std::pair<EventCallbackToken, EventCallbackFn>>      EventManager::m_eventListenerFns;
    EventCallbackToken              EventManager::m_tokens;



    void EventManager::PushEvent(const EVENT::BND_Event& event)
    {
        if(event.type != EVENT::Type::NONE)
        {
            m_eventPool.Push(event);
            DispatchEvents();
        }
            
    }

    void EventManager::DispatchEvents()
    {
        EVENT::BND_Event event = {};
        while(m_eventPool.Poll(event))
        {
            for(const auto& l : m_eventListeners)
            {
                l->ProcessEvent(event);
            }

            for(const auto& [token,fn] : m_eventListenerFns)
            {
                fn(event);
            }
        }
    }

    void EventManager::AddListener(IEventListener *eventListener)
    {
        m_eventListeners.push_back(eventListener);
    }

    void EventManager::RemoveListener(IEventListener *eventListener)
    {
        m_eventListeners.remove(eventListener);
    }

    EventCallbackToken EventManager::BindListenerFn(const EventCallbackFn& callback)
    {
        auto token = m_tokens++;
        m_eventListenerFns.emplace_back(token, callback);
        return token;
    }

    void EventManager::RemoveListenerFn(EventCallbackToken token)
    {
        auto it = std::find_if(m_eventListenerFns.begin(), m_eventListenerFns.end(),
            [token](const auto& pair) {return pair.first == token; });
        if (it != m_eventListenerFns.end())
            m_eventListenerFns.erase(it);
    }

    void EventManager::Clear()
    {
        while (!m_eventListeners.empty())
            m_eventListeners.pop_back();

        m_eventListeners.clear();

        m_eventListenerFns.clear();

        m_eventPool.Clear();
    }


} // BINDU