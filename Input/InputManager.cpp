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

        Keyboard m_keyboard;
        Mouse m_mouse;

        std::unique_ptr<IInputHandler> m_inputHandler{nullptr};
    };

    InputManager::InputManager() : m_impl(new Impl)
    {
        m_impl->m_inputHandler = std::make_unique<Win32InputHandler>(&m_impl->m_keyboard, &m_impl->m_mouse);
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

    bool InputManager::IsButtonPressed(BND_Button button) const
    {
        return m_impl->m_inputHandler->IsMouseBtnPressed(button);
    }

    bool InputManager::IsButtonReleased(BND_Button button) const
    {
        return m_impl->m_inputHandler->IsMouseBtnReleased(button);
    }

    bool InputManager::IsButtonHeld(BND_Button button) const
    {
        return m_impl->m_inputHandler->IsMouseBtnHeld(button);
    }

    bool InputManager::IsMouseDragged(BND_Button button) const
    {
        return m_impl->m_inputHandler->IsMouseDragStart(button);
    }

    Mouse& InputManager::GetMouse() const
    {
        return m_impl->m_mouse;
    }


    Keyboard &InputManager::GetKeyboard() const
    {
        return m_impl->m_keyboard;
    }

    void InputManager::ProcessEvent(EVENT::BND_Event event)
    {
        m_impl->m_inputHandler->ProcessEvents(event);
    }



} // BINDU