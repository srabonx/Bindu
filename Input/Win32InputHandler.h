//
// Created by letsd on 26-Jul-24.
//

#ifndef BINDU_WIN32INPUTHANDLER_H
#define BINDU_WIN32INPUTHANDLER_H

#include "IInputHandler.h"


namespace BINDU {

    class Win32InputHandler : public IInputHandler
    {
    public:

        Win32InputHandler();

        ~Win32InputHandler() override;

        bool IsKeyPressed(BND_Keys bnd_key) override;

        bool IsKeyReleased(BND_Keys bnd_key) override;

        bool IsKeyHeld(BND_Keys bnd_key) override;

        bool IsMouseBtnPressed(BND_Buttons bnd_button) override;

        bool IsMouseBtnReleased(BND_Buttons bnd_button) override;

        void ProcessEvents(EVENT::BND_Event event) override ;

    private:
        class Impl;
        Impl* m_impl{nullptr};
    };

} // BINDU

#endif //BINDU_WIN32INPUTHANDLER_H
