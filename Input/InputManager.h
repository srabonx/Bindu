//
// Created by letsd on 26-Jul-24.
//

#ifndef BINDU_INPUTMANAGER_H
#define BINDU_INPUTMANAGER_H

#include <memory>

#include "Keyboard.h"
#include "Mouse.h"
#include "../Event/IEventListener.h"

namespace BINDU {
	class IInputHandler;

	// The manager class for managing inputs
    class InputManager final : public IEventListener
    {
    public:
        InputManager() = default;
        ~InputManager() override = default;

        static void Initialize();

        static bool IsKeyPressed(BND_Key key);

        static bool IsKeyReleased(BND_Key key);

        static bool IsKeyHeld(BND_Key key);

        static bool IsButtonPressed(BND_Button button);

        static bool IsButtonReleased(BND_Button button);

        static bool IsButtonHeld(BND_Button button);

        static bool IsMouseDragged(BND_Button button);

        static Mouse& GetMouse();

        static Keyboard& GetKeyboard();

    	void ProcessEvent(EVENT::BND_Event event) override;

    private:
        static std::unique_ptr<IInputHandler> m_inputHandler;

        static Keyboard m_keyboard;
        static Mouse m_mouse;
    };

} // BINDU

#endif //BINDU_INPUTMANAGER_H
