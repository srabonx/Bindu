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
        std::list<EventCallbackFn> m_eventListenerFns;
    };




    EventManager::EventManager() : m_impl(std::make_unique<Impl>())
    {
        m_impl->m_eventPool = std::make_unique<EventPool>();
    }

    EventManager::~EventManager()
    {

    }

    void EventManager::PushEvent(EVENT::BND_Event event) const
    {
        if(event.type != EVENT::Type::NONE)
        {
            m_impl->m_eventPool->Push(event);
            DispatchEvents();
        }
            
    }

    void EventManager::DispatchEvents() const
    {
        EVENT::BND_Event event = {};
        while(m_impl->m_eventPool->Poll(event))
        {
            for(const auto& l : m_impl->m_eventListeners)
            {
                l->ProcessEvent(event);
            }

            for(const auto& fn : m_impl->m_eventListenerFns)
            {
                fn(event);
            }
        }
    }

    void EventManager::AddListener(IEventListener *eventListener) const
    {
        m_impl->m_eventListeners.push_back(eventListener);
    }

    void EventManager::RemoveListener(IEventListener *eventListener) const
    {
        m_impl->m_eventListeners.remove(eventListener);
    }

    void EventManager::BindListenerFn(const EventCallbackFn& callback) const
    {
        m_impl->m_eventListenerFns.emplace_back(callback);
    }

    void EventManager::RemoveListenerFn(const EventCallbackFn& callback) const
    {
        m_impl->m_eventListenerFns.remove(callback);
    }

    void EventManager::Clear() const
    {
        while(!m_impl->m_eventListeners.empty())
            m_impl->m_eventListeners.pop_back();

        m_impl->m_eventListeners.clear();

        m_impl->m_eventListenerFns.clear();

        m_impl->m_eventPool->Clear();
    }


} // BINDU