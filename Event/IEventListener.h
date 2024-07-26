//
// Created by letsd on 22-Jul-24.
//

#ifndef BINDU_IEVENTLISTENER_H
#define BINDU_IEVENTLISTENER_H

namespace BINDU {
    namespace EVENT
    {
        struct BND_Event;
    }

    class IEventListener
    {
    public:
        virtual ~IEventListener() {}
        virtual void ProcessEvent(EVENT::BND_Event event) = 0;
    };

} // BINDU

#endif //BINDU_IEVENTLISTENER_H
