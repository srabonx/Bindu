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

    // The manager class for managing inputs
    class InputManager final : public IEventListener
    {
    public:
        InputManager();
        ~InputManager() override;

        bool IsKeyPressed(BND_Key key) const;

        bool IsKeyReleased(BND_Key key) const;

        bool IsKeyHeld(BND_Key key) const;

        bool IsButtonPressed(BND_Button button) const;

        bool IsButtonReleased(BND_Button button) const;

        bool IsButtonHeld(BND_Button button) const;

        bool IsMouseDragged(BND_Button button) const;

        Mouse& GetMouse() const;

        Keyboard& GetKeyboard() const;

        void ProcessEvent(EVENT::BND_Event event) override;

    private:
        class Impl;
        Impl* m_impl{nullptr};
    };

} // BINDU

#endif //BINDU_INPUTMANAGER_H
