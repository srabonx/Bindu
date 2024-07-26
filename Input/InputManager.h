//
// Created by letsd on 26-Jul-24.
//

#ifndef BINDU_INPUTMANAGER_H
#define BINDU_INPUTMANAGER_H

#include <memory>
#include "../Event/IEventListener.h"

namespace BINDU {

    // The manager class for managing inputs
    class InputManager final : public IEventListener
    {
    public:
        InputManager();
        ~InputManager() = default;



        void ProcessEvent(EVENT::BND_Event event) override;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl{nullptr};
    };

} // BINDU

#endif //BINDU_INPUTMANAGER_H
