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

    InputManager::InputManager() : m_impl(new Impl)
    {
        m_impl->m_inputHandler = std::make_unique<Win32InputHandler>();
    }

    InputManager::~InputManager()
    {
        delete m_impl;
    }

    bool InputManager::IsKeyPressed(BND_Key key) const
    {
        return m_impl->m_inputHandler->IsKeyPressed(key);
    }

    bool InputManager::IsKeyReleased(BND_Key key) const
    {
        return m_impl->m_inputHandler->IsKeyReleased(key);
    }

    bool InputManager::IsKeyHeld(BND_Key key) const
    {
        return m_impl->m_inputHandler->IsKeyHeld(key);
    }

    void InputManager::ProcessEvent(EVENT::BND_Event event)
    {
        m_impl->m_inputHandler->ProcessEvents(event);
    }


} // BINDU