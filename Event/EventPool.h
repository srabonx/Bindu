//
// Created by letsd on 22-Jul-24.
//

#ifndef BINDU_EVENTPOOL_H
#define BINDU_EVENTPOOL_H

#include <memory>

namespace BINDU {

    namespace EVENT{
        struct BND_Event;
    }

    class EventPool
    {
    public:
        EventPool();

        ~EventPool();

        void Push(EVENT::BND_Event event) const;

        bool Poll(EVENT::BND_Event& event) const;

        void Clear() const;


    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };

} // BINDU

#endif //BINDU_EVENTPOOL_H
