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

        Keyboard m_keyboard ;
    };

    Win32InputHandler::Win32InputHandler() :  m_impl(new Impl)
    {

    }

    Win32InputHandler::~Win32InputHandler()
    {
        delete m_impl;
    }


    bool Win32InputHandler::IsKeyPressed(BND_Key bnd_key)
    {
        KeyState state = m_impl->m_keyboard.GetKeyState(bnd_key);

	    if (state.isDown && !state.wasDown)
	        {
				m_impl->m_keyboard.SetKeyState(bnd_key, { false,false });
	            return true;
	        }

        return false;
    }

    bool Win32InputHandler::IsKeyReleased(BND_Key bnd_key)
	{
        KeyState state = m_impl->m_keyboard.GetKeyState(bnd_key);

        if (state.wasDown && !state.isDown)
        {
            m_impl->m_keyboard.SetKeyState(bnd_key, { false,false });
            return true;
        }

        return false;
    }

    bool Win32InputHandler::IsKeyHeld(BND_Key bnd_key) {
        KeyState state = m_impl->m_keyboard.GetKeyState(bnd_key);

        return state.isDown && state.wasDown;
    }

    bool Win32InputHandler::IsMouseBtnPressed(BND_Buttons bnd_button) {
        return false;
    }

    bool Win32InputHandler::IsMouseBtnReleased(BND_Buttons bnd_button) {
        return false;
    }

    void Win32InputHandler::ProcessEvents(EVENT::BND_Event event)
    {
        switch (event.type)
        {
            case EVENT::Type::KEY_DOWN:
            case EVENT::Type::KEY_UP:
                //KeyState state = m_impl->m_keyboard.GetKeyState(event.body.Ev_Keyboard.key);
                m_impl->m_keyboard.SetKeyState(event.body.Ev_Keyboard.key, event.body.Ev_Keyboard.state);
                break;
            default:
                break;
        }
    }



} // BINDU