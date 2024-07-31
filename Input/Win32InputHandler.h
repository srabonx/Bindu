//
// Created by letsd on 26-Jul-24.
//

#ifndef BINDU_WIN32INPUTHANDLER_H
#define BINDU_WIN32INPUTHANDLER_H

#include "IInputHandler.h"


namespace BINDU {

    class Keyboard;
    class Mouse;

	class Win32InputHandler : public IInputHandler
    {
    public:

        Win32InputHandler(Keyboard* keyboard, Mouse* mouse);

        ~Win32InputHandler() override;

        bool IsKeyPressed(BND_Key key) override;

        bool IsKeyReleased(BND_Key key) override;

        bool IsKeyHeld(BND_Key key) override;

        bool IsMouseBtnPressed(BND_Button button) override;

        bool IsMouseBtnReleased(BND_Button button) override;

        bool IsMouseBtnHeld(BND_Button button) override;

        void ProcessEvents(EVENT::BND_Event event) override ;

        bool IsMouseDragStart(BND_Button button) override;

        MousePos GetMousePosition() override;

        bool IsMouseMoved() override;

    private:
        class Impl;
        Impl* m_impl{nullptr};
    };

} // BINDU

#endif //BINDU_WIN32INPUTHANDLER_H
