//
// Created by letsd on 26-Jul-24.
//

#ifndef BINDU_IINPUTHANDLER_H
#define BINDU_IINPUTHANDLER_H


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
        virtual bool IsKeyPressed(BND_Key key) = 0;

        // Checks whether the bnd_key is released or not
        virtual bool IsKeyReleased(BND_Key key) = 0;

        // Checks whether the bnd_key is held or not
        virtual bool IsKeyHeld(BND_Key key) = 0;

        // Checks whether the bnd_button is pressed or not
        virtual bool IsMouseBtnPressed(BND_Button button) = 0;

        // Checks whether the bnd_button is Released or not
        virtual bool IsMouseBtnReleased(BND_Button button) = 0;

        // Checks whether the button is held or not
        virtual bool IsMouseBtnHeld(BND_Button button) = 0;

        // Checks whether the mouse is being dragged while pressing the button
        virtual bool IsMouseDragStart(BND_Button button) = 0;

        // Get the current mouse pos
        virtual MousePos GetMousePosition() = 0;

        // Checks whether the mouse is being moved or not
        virtual bool IsMouseMoved() = 0;

        // Handle the Keyboard or Mouse event
        virtual void ProcessEvents(EVENT::BND_Event event) = 0;

    };

} // BINDU

#endif //BINDU_IINPUTHANDLER_H
