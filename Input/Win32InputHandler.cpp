//
// Created by letsd on 26-Jul-24.
//

#include "Win32InputHandler.h"
#include "Keyboard.h"
#include "../Event/EventStruct.h"
#include "../Window/Win32Window.h"

namespace BINDU {

    class Win32InputHandler::Impl
    {
    public:

        Keyboard* m_keyboard{ nullptr };
        Mouse* m_mouse{ nullptr };

        MouseDelta m_mouseDelta;
        MousePos m_prevMousePos;

        int m_mouseWheelDelta;

    public:
        void UpdateKeyState(BND_Key keyCode, bool pressed) const;
        void UpdateMouseState(BND_Button button, bool pressed) const;
    };

    void Win32InputHandler::Impl::UpdateKeyState(BND_Key keyCode, bool pressed) const
    {
        if (pressed)
        {
            if (m_keyboard->GetKeyState(keyCode) != KeyState::HELD)
                m_keyboard->SetKeyState(keyCode, KeyState::PRESSED);
        }
        else
            m_keyboard->SetKeyState(keyCode, KeyState::RELEASED);
    }

    void Win32InputHandler::Impl::UpdateMouseState(BND_Button button, bool pressed) const
    {
        if(pressed)
        {
            if (m_mouse->GetButtonState(button) != ButtonState::HELD)
                m_mouse->SetButtonState(button, ButtonState::PRESSED);
        }
        else
            m_mouse->SetButtonState(button, ButtonState::RELEASED);
    }

    Win32InputHandler::Win32InputHandler(Keyboard* keyboard, Mouse* mouse) :  m_impl(new Impl)
    {
        m_impl->m_keyboard = keyboard;
        m_impl->m_mouse = mouse;
    }

    Win32InputHandler::~Win32InputHandler()
    {
        delete m_impl;
    }


    bool Win32InputHandler::IsKeyPressed(BND_Key bnd_key) const
    {
        KeyState state = m_impl->m_keyboard->GetKeyState(bnd_key);

        return state == KeyState::PRESSED;
    }

    bool Win32InputHandler::IsKeyReleased(BND_Key bnd_key) const
	{
        KeyState state = m_impl->m_keyboard->GetKeyState(bnd_key);

        return state == KeyState::RELEASED;
    }

    bool Win32InputHandler::IsKeyHeld(BND_Key bnd_key) const {
        KeyState state = m_impl->m_keyboard->GetKeyState(bnd_key);

        return state == KeyState::HELD || state == KeyState::PRESSED;
    }

    bool Win32InputHandler::IsKeyCombinationPressed(std::initializer_list<BND_Key> keys) const
    {
        for(auto key : keys)
        {
            if(!IsKeyHeld(key))
            {
                return false;
            }
        }

        return true;
    }

    MousePos Win32InputHandler::GetMousePosition() const
    {
        return m_impl->m_mouse->GetMousePos();
    }

    int Win32InputHandler::GetMouseX() const
    {
        return m_impl->m_mouse->GetMouseX();
    }

    int Win32InputHandler::GetMouseY() const
    {
        return m_impl->m_mouse->GetMouseY();
    }

    int Win32InputHandler::GetMouseDeltaX(BND_Button button) const
    {
        if(IsMouseBtnHeld(button))
        {
            m_impl->m_mouseDelta.x = GetMouseX() - m_impl->m_prevMousePos.x;
        }
        else
        {
            m_impl->m_prevMousePos.x = GetMouseX();
        }

        return m_impl->m_mouseDelta.x;
    }

    int Win32InputHandler::GetMouseDeltaY(BND_Button button) const
    {

        if (IsMouseBtnHeld(button))
        {
            m_impl->m_mouseDelta.y = GetMouseY() - m_impl->m_prevMousePos.y;
        }
        else
        {
            m_impl->m_prevMousePos.y = GetMouseY();
        }

        return m_impl->m_mouseDelta.y;
    }

    int Win32InputHandler::GetMouseWheelDelta() const
    {
        return m_impl->m_mouseWheelDelta;
    }

    void Win32InputHandler::ResetMouseDeltas()
    {
        m_impl->m_mouseDelta.x = 0;
        m_impl->m_mouseDelta.y = 0;
        m_impl->m_mouseWheelDelta = 0;
    }

    bool Win32InputHandler::IsMouseBtnPressed(BND_Button bnd_button) const
	{
        ButtonState state = m_impl->m_mouse->GetButtonState(bnd_button);

        return state == ButtonState::PRESSED;
    }

    bool Win32InputHandler::IsMouseBtnReleased(BND_Button bnd_button) const
	{
        ButtonState state = m_impl->m_mouse->GetButtonState(bnd_button);

        return state == ButtonState::RELEASED;
    }

    bool Win32InputHandler::IsMouseBtnHeld(BND_Button button) const
    {
        ButtonState state = m_impl->m_mouse->GetButtonState(button);

        return state == ButtonState::HELD || state == ButtonState::PRESSED;
    }


    void Win32InputHandler::ProcessEvents(EVENT::BND_Event event)
    {
        m_impl->m_mouse->Update();
        m_impl->m_keyboard->Update();
        ResetMouseDeltas();

        switch (event.type)
        {
            case EVENT::Type::KEY_DOWN:
            case EVENT::Type::KEY_UP:
                m_impl->UpdateKeyState(event.Ev_Keyboard.key, event.Ev_Keyboard.pressed);
                break;


			case EVENT::Type::MOUSE_DOWN:
            {
                m_impl->UpdateMouseState(event.Ev_Mouse.button, true);
            }
            break;

			case EVENT::Type::MOUSE_UP:
            {
                m_impl->UpdateMouseState(event.Ev_Mouse.button, false);
            }
                break;

			case EVENT::Type::MOUSE_MOVE:

                if (event.Ev_Mouse.button != BND_Button::BND_NONE)
                {
                    m_impl->UpdateMouseState(event.Ev_Mouse.button, true);
                }
                m_impl->m_mouse->SetMousePos(event.Ev_Mouse.position);

                break;

            case EVENT::Type::MOUSE_SCROLL:

                if (event.Ev_Mouse.button != BND_Button::BND_NONE)
                {
                    m_impl->UpdateMouseState(event.Ev_Mouse.button, true);
                }

                m_impl->m_mouseWheelDelta  = event.Ev_Mouse.mouseWheelDelta;

                break;

            default:
                break;
        }

    }

} // BINDU