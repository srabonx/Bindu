//
// Created by letsd on 26-Jul-24.
//

#ifndef BINDU_IINPUTHANDLER_H
#define BINDU_IINPUTHANDLER_H

#include <initializer_list>

namespace BINDU
{
	struct MousePos;
}

namespace BINDU {

    namespace EVENT
    {
        struct BND_Event;
    }

    enum class BND_Key;
    enum class BND_Button;

    class IInputHandler
    {
    public:
        virtual ~IInputHandler() = default;

        // Checks whether the bnd_key Key is pressed or not.
        virtual bool IsKeyPressed(BND_Key key) const = 0;

        // Checks whether the bnd_key is released or not
        virtual bool IsKeyReleased(BND_Key key) const = 0;

        // Checks whether the bnd_key is held or not
        virtual bool IsKeyHeld(BND_Key key) const = 0;

        // Checks whether a combination of key is pressed
        virtual bool IsKeyCombinationPressed(std::initializer_list<BND_Key> keys) const = 0;

        // Checks whether the bnd_button is pressed or not
        virtual bool IsMouseBtnPressed(BND_Button button) const = 0;

        // Checks whether the bnd_button is Released or not
        virtual bool IsMouseBtnReleased(BND_Button button) const = 0;

        // Checks whether the button is held or not
        virtual bool IsMouseBtnHeld(BND_Button button) const = 0;

        // Get the current mouse pos
        virtual MousePos GetMousePosition() const = 0;

        virtual int  GetMouseX() const = 0;

        virtual int  GetMouseY() const = 0;

        virtual int  GetMouseDeltaX(BND_Button button) const = 0;

        virtual int  GetMouseDeltaY(BND_Button button) const = 0;

        virtual int  GetMouseWheelDelta() const = 0;

        virtual void ResetMouseDeltas() = 0;

        // Handle the Keyboard or Mouse event
        virtual void ProcessEvents(EVENT::BND_Event event) = 0;

    };

} // BINDU

#endif //BINDU_IINPUTHANDLER_H
