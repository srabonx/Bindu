//
// Created by letsd on 22-Jul-24.
//

#include "EventPool.h"
#include "EventStruct.h"
#include <queue>


namespace BINDU {

    using namespace EVENT;

    class EventPool::Impl
    {
    public:
        std::queue<BND_Event> m_events;
    };


    EventPool::EventPool() : m_impl(std::make_unique<Impl>())
    {

    }

    EventPool::~EventPool()
    {

    }

    void EventPool::Push(BND_Event event)
    {
        m_impl->m_events.push(event);
    }

    bool EventPool::Poll(BND_Event &event)
    {
        if(m_impl->m_events.empty())
            return false;

        event = m_impl->m_events.front();
        m_impl->m_events.pop();
        return true;
    }

    void EventPool::Clear()
    {
        while(!m_impl->m_events.empty())
            m_impl->m_events.pop();

    }
} // BINDU