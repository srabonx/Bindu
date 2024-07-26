//
// Created by letsd on 26-Jul-24.
//

#include "Win32InputHandler.h"
#include "Keyboard.h"
#include "../Event/EventStruct.h"

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


    bool Win32InputHandler::IsKeyPressed(BND_Keys bnd_key) {
        return false;
    }

    bool Win32InputHandler::IsKeyReleased(BND_Keys bnd_key) {
        return false;
    }

    bool Win32InputHandler::IsKeyHeld(BND_Keys bnd_key) {
        return false;
    }

    bool Win32InputHandler::IsMouseBtnPressed(BND_Buttons bnd_button) {
        return false;
    }

    bool Win32InputHandler::IsMouseBtnReleased(BND_Buttons bnd_button) {
        return false;
    }

    void Win32InputHandler::ProcessEvents(EVENT::BND_Event event)
    {

    }



} // BINDU