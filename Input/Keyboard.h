//
// Created by letsd on 26-Jul-24.
//

#ifndef BINDU_KEYBOARD_H
#define BINDU_KEYBOARD_H

#include <array>

namespace BINDU {

    enum class BND_Key
    {
        BND_A = 0x41,
        BND_B,
        BND_C,
        BND_D,
        BND_E,
        BND_F,
        BND_G,
        BND_H,
        BND_I,
        BND_J,
        BND_K,
        BND_L,
        BND_M,
        BND_N,
        BND_O,
        BND_P,
        BND_Q,
        BND_R,
        BND_S,
        BND_T,
        BND_U,
        BND_V,
        BND_W,
        BND_X,
        BND_Y,
        BND_Z
    };

    struct KeyState
    {
        bool isDown{false};
        bool wasDown{false};
    };

    using Keys = std::array<KeyState,256>;

    class Keyboard
    {
    public:
        Keyboard();
        ~Keyboard() ;

        // Get the current state of the given key
        KeyState GetKeyState(BND_Key key);

        // Set the state of the given key
        void SetKeyState(BND_Key key, const KeyState& state);

        // Get the current state of the keyboard
        [[nodiscard]] Keys GetKeyboardState() const;

        // Set the state of the keyboard
        void SetKeyboardState(const Keys& state);

    private:
        class Impl;
        Impl* m_impl{nullptr};
    };

} // BINDU

#endif //BINDU_KEYBOARD_H
