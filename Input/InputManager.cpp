//
// Created by letsd on 26-Jul-24.
//

#include "InputManager.h"
#include "../Event/EventStruct.h"
#include "IInputHandler.h"
#include "Win32InputHandler.h"

namespace BINDU {

    class InputManager::Impl
    {
    public:
        ~Impl() = default;

        std::unique_ptr<IInputHandler> m_inputHandler{nullptr};
    };

    InputManager::InputManager() : m_impl(std::make_unique<Impl>())
    {
        m_impl->m_inputHandler = std::make_unique<Win32InputHandler>();
    }

    void InputManager::ProcessEvent(EVENT::BND_Event event)
    {
        m_impl->m_inputHandler->ProcessEvents(event);
    }


} // BINDU