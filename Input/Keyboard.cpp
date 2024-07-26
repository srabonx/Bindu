//
// Created by letsd on 26-Jul-24.
//

#include <cstdint>
#include <array>
#include "Keyboard.h"

namespace BINDU {

    class Keyboard::Impl
    {
    public:
        Keys m_state;
    };

    Keyboard::Keyboard() : m_impl(new Impl)
    {

    }


    Keyboard::~Keyboard()
    {
        delete m_impl;
    }

    void Keyboard::SetKeyboardState(const Keys &state)
    {
        m_impl->m_state = state;
    }

    Keys Keyboard::GetKeyboardState() const
    {
        return m_impl->m_state;
    }

    KeyState Keyboard::GetKeyState(BND_Keys key)
    {
        return m_impl->m_state[(int)key];
    }

    void Keyboard::SetKeyState(BND_Keys key, const KeyState &state)
    {
        m_impl->m_state[(int)key] = state;
    }



} // BINDU