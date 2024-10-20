//
// Created by letsd on 22-Jul-24.
//

#ifndef BINDU_EVENTMANAGER_H
#define BINDU_EVENTMANAGER_H

#include <functional>
#include <memory>


#define BINDU_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) {return this->fn(std::forward<decltype(args)>(args)...);}

namespace BINDU {
    namespace EVENT
    {
        struct BND_Event;
    }
    class IEventListener;

    using EventCallbackFn = std::function<void(EVENT::BND_Event&)>;

    class EventManager
    {
    public:
        EventManager();

        ~EventManager();

        void PushEvent(EVENT::BND_Event event) const;

        void DispatchEvents() const;

        void AddListener(IEventListener* eventListener) const;

        void RemoveListener(IEventListener* eventListener) const;

        void BindListenerFn(const EventCallbackFn& callback) const;

        void RemoveListenerFn(const EventCallbackFn& callback) const;

        void Clear() const;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl{nullptr};
    };

} // BINDU

#endif //BINDU_EVENTMANAGER_H
