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

    class EventManager::Impl
    {
    public:
        std::unique_ptr<EventPool> m_eventPool;
        std::list<IEventListener*> m_eventListeners;
    };




    EventManager::EventManager() : m_impl(std::make_unique<Impl>())
    {
        m_impl->m_eventPool = std::make_unique<EventPool>();
    }

    EventManager::~EventManager()
    {

    }

    void EventManager::PushEvent(EVENT::BND_Event event)
    {
        if(event.type != EVENT::Type::NONE)
            m_impl->m_eventPool->Push(event);
    }

    void EventManager::DispatchEvents()
    {
        EVENT::BND_Event event = {};
        while(m_impl->m_eventPool->Poll(event))
        {
            for(const auto& l : m_impl->m_eventListeners)
            {
                l->ProcessEvent(event);
            }
        }
    }

    void EventManager::AddListener(IEventListener *eventListener)
    {
        m_impl->m_eventListeners.push_back(eventListener);
    }

    void EventManager::RemoveListener(IEventListener *eventListener)
    {
        m_impl->m_eventListeners.remove(eventListener);
    }

    void EventManager::Clear()
    {
        while(!m_impl->m_eventListeners.empty())
            m_impl->m_eventListeners.pop_back();

        m_impl->m_eventListeners.clear();

        m_impl->m_eventPool->Clear();
    }


} // BINDU