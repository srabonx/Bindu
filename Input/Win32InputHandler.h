//
// Created by letsd on 26-Jul-24.
//

#ifndef BINDU_WIN32INPUTHANDLER_H
#define BINDU_WIN32INPUTHANDLER_H

#include <initializer_list>

#include "IInputHandler.h"


namespace BINDU {

    class Keyboard;
    class Mouse;

	class Win32InputHandler : public IInputHandler
    {
    public:

        Win32InputHandler(Keyboard* keyboard, Mouse* mouse);

        ~Win32InputHandler() override;

        bool IsKeyPressed(BND_Key key) const override;

        bool IsKeyReleased(BND_Key key) const override;

        bool IsKeyHeld(BND_Key key) const override;

        bool IsKeyCombinationPressed(std::initializer_list<BND_Key> keys) const override;

        bool IsMouseBtnPressed(BND_Button button) const override;

        bool IsMouseBtnReleased(BND_Button button) const override;

        bool IsMouseBtnHeld(BND_Button button) const override;

        void ProcessEvents(EVENT::BND_Event event) override ;

        MousePos GetMousePosition() const override;

        int  GetMouseX() const override;

        int  GetMouseY() const override;

        int  GetMouseDeltaX(BND_Button button) const override;

        int  GetMouseDeltaY(BND_Button button) const override;

		int  GetMouseWheelDelta() const override;

        void ResetMouseDeltas() override;


    private:
        class Impl;
        Impl* m_impl{nullptr};
    };

} // BINDU

#endif //BINDU_WIN32INPUTHANDLER_H
