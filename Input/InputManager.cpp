//
// Created by letsd on 26-Jul-24.
//

#include "InputManager.h"
#include "../Event/EventStruct.h"
#include "IInputHandler.h"
#include "Win32InputHandler.h"
#include "../Event/EventManager.h"

namespace BINDU {

    std::unique_ptr<IInputHandler> InputManager::m_inputHandler = nullptr;
    Keyboard InputManager::m_keyboard;
    Mouse InputManager::m_mouse;


    void InputManager::Initialize()
    {
        m_inputHandler = std::make_unique<Win32InputHandler>(&m_keyboard, &m_mouse);
    }

    bool InputManager::IsKeyPressed(BND_Key key)
    {
        return m_inputHandler->IsKeyPressed(key);
    }

    bool InputManager::IsKeyReleased(BND_Key key)
    {
        return m_inputHandler->IsKeyReleased(key);
    }

    bool InputManager::IsKeyHeld(BND_Key key)
    {
        return m_inputHandler->IsKeyHeld(key);
    }

    bool InputManager::IsButtonPressed(BND_Button button)
    {
        return m_inputHandler->IsMouseBtnPressed(button);
    }

    bool InputManager::IsButtonReleased(BND_Button button)
    {
        return m_inputHandler->IsMouseBtnReleased(button);
    }

    bool InputManager::IsButtonHeld(BND_Button button)
    {
        return m_inputHandler->IsMouseBtnHeld(button);
    }

    bool InputManager::IsMouseDragged(BND_Button button)
    {
        return m_inputHandler->IsMouseDragStart(button);
    }

    Mouse& InputManager::GetMouse()
    {
        return m_mouse;
    }


    Keyboard &InputManager::GetKeyboard()
    {
        return m_keyboard;
    }

    void InputManager::ProcessEvent(EVENT::BND_Event event)
    {
        m_inputHandler->ProcessEvents(event);
    }



} // BINDU