//
// Created by letsd on 26-Jul-24.
//

#ifndef BINDU_IINPUTHANDLER_H
#define BINDU_IINPUTHANDLER_H


namespace BINDU {

    namespace EVENT
    {
        struct BND_Event;
    }

    enum class BND_Keys;
    enum class BND_Buttons;

    class IInputHandler
    {
    public:
        virtual ~IInputHandler() = default;

        // Checks whether the bnd_key Key is pressed or not.
        virtual bool IsKeyPressed(BND_Keys bnd_key) = 0;

        // Checks whether the bnd_key is released or not
        virtual bool IsKeyReleased(BND_Keys bnd_key) = 0;

        // Checks whether the bnd_key is held or not
        virtual bool IsKeyHeld(BND_Keys bnd_key) = 0;

        // Checks whether the bnd_button is pressed or not
        virtual bool IsMouseBtnPressed(BND_Buttons bnd_button) = 0;

        // Checks whether the bnd_button is Released or not
        virtual bool IsMouseBtnReleased(BND_Buttons bnd_button) = 0;

        // Handle the Keyboard or Mouse event
        virtual void ProcessEvents(EVENT::BND_Event event) = 0;

    };

} // BINDU

#endif //BINDU_IINPUTHANDLER_H
