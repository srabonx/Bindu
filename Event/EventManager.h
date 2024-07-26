//
// Created by letsd on 22-Jul-24.
//

#ifndef BINDU_EVENTMANAGER_H
#define BINDU_EVENTMANAGER_H

#include <memory>

namespace BINDU {
    namespace EVENT
    {
        struct BND_Event;
    }
    class IEventListener;


    class EventManager
    {
    public:
        EventManager();

        ~EventManager();

        void PushEvent(EVENT::BND_Event event);

        void DispatchEvents();

        void AddListener(IEventListener* eventListener);

        void RemoveListener(IEventListener* eventListener);

        void Clear();

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl{nullptr};
    };

} // BINDU

#endif //BINDU_EVENTMANAGER_H
