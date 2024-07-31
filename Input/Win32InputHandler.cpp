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
    };

    Win32InputHandler::Win32InputHandler(Keyboard* keyboard, Mouse* mouse) :  m_impl(new Impl)
    {
        m_impl->m_keyboard = keyboard;
        m_impl->m_mouse = mouse;
    }

    Win32InputHandler::~Win32InputHandler()
    {
        delete m_impl;
    }


    bool Win32InputHandler::IsKeyPressed(BND_Key bnd_key)
    {
        KeyState state = m_impl->m_keyboard->GetKeyState(bnd_key);

	    if (state.isDown && !state.wasDown)
	        {
				m_impl->m_keyboard->SetKeyState(bnd_key, { false,false });
	            return true;
	        }

        return false;
    }

    bool Win32InputHandler::IsKeyReleased(BND_Key bnd_key)
	{
        KeyState state = m_impl->m_keyboard->GetKeyState(bnd_key);

        if (state.wasDown && !state.isDown)
        {
            m_impl->m_keyboard->SetKeyState(bnd_key, { false,false });
            return true;
        }

        return false;
    }

    bool Win32InputHandler::IsKeyHeld(BND_Key bnd_key) {
        KeyState state = m_impl->m_keyboard->GetKeyState(bnd_key);

        return state.isDown && state.wasDown;
    }

    bool Win32InputHandler::IsMouseBtnPressed(BND_Button bnd_button) {
        ButtonState state = m_impl->m_mouse->GetButtonState(bnd_button);

        if(state.isDown && !state.wasDown)
        {
            m_impl->m_mouse->SetButtonState(bnd_button, { true,true });
            return true;
        }

        return false;
    }

    bool Win32InputHandler::IsMouseBtnReleased(BND_Button bnd_button) {
        ButtonState state = m_impl->m_mouse->GetButtonState(bnd_button);

        if(state.wasDown && !state.isDown)
        {
            m_impl->m_mouse->SetButtonState(bnd_button, { false,false });
            return true;
        }


        return false;
    }

    bool Win32InputHandler::IsMouseBtnHeld(BND_Button button)
    {
        ButtonState state = m_impl->m_mouse->GetButtonState(button);

        return state.isDown && state.wasDown;
    }

    bool Win32InputHandler::IsMouseDragStart(BND_Button button)
    {
    	if(m_impl->m_mouse->IsMouseDragged())
    	{
            ButtonState state = m_impl->m_mouse->GetButtonState(button);

            if (state.isDown)
                return true;
    	}

        return false;
    }

    MousePos Win32InputHandler::GetMousePosition()
    {
        return m_impl->m_mouse->GetMousePos();
    }

    bool Win32InputHandler::IsMouseMoved()
    {
        return m_impl->m_mouse->IsMouseMoved();
    }

    void Win32InputHandler::ProcessEvents(EVENT::BND_Event event)
    {
        m_impl->m_mouse->SetMouseMove(false);

        switch (event.type)
        {
            case EVENT::Type::KEY_DOWN:
            case EVENT::Type::KEY_UP:
                //KeyState state = m_impl->m_keyboard.GetKeyState(event.body.Ev_Keyboard.key);
                m_impl->m_keyboard->SetKeyState(event.body.Ev_Keyboard.key, event.body.Ev_Keyboard.state);
                break;

			case EVENT::Type::MOUSE_DOWN:
            {
                ButtonState state = m_impl->m_mouse->GetButtonState(event.body.Ev_Mouse.button);
                state.isDown = true;

                m_impl->m_mouse->SetButtonState(event.body.Ev_Mouse.button, state);

                m_impl->m_mouse->SetMousePos(event.body.Ev_Mouse.position);
            }
            break;

			case EVENT::Type::MOUSE_UP:
            {
                ButtonState state = m_impl->m_mouse->GetButtonState(event.body.Ev_Mouse.button);
                state.isDown = false;

                m_impl->m_mouse->SetButtonState(event.body.Ev_Mouse.button, state);

                m_impl->m_mouse->SetMousePos(event.body.Ev_Mouse.position);
            }
                break;

			case EVENT::Type::MOUSE_MOVE:

                if (event.body.Ev_Mouse.button != BND_Button::BND_NONE)
                {
                    m_impl->m_mouse->SetMouseDragged(true);

                    ButtonState state = m_impl->m_mouse->GetButtonState(event.body.Ev_Mouse.button);
                    state.wasDown = state.isDown;
                    state.isDown = true;
                    m_impl->m_mouse->SetButtonState(event.body.Ev_Mouse.button, state);
                }
                else
                {
                    m_impl->m_mouse->SetMouseDragged(false);

                    m_impl->m_mouse->SetMouseMove(true);
                }
                    

                m_impl->m_mouse->SetMousePos(event.body.Ev_Mouse.position);

                break;

            default:
                break;
        }
    }

 
} // BINDU