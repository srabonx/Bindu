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
        Keys m_keys;
    };

    Keyboard::Keyboard() : m_impl(new Impl)
    {
        for(auto& k : m_impl->m_keys)
        {
            k.isDown = false;
            k.wasDown = false;
        }
    }


    Keyboard::~Keyboard()
    {
        delete m_impl;
    }

    void Keyboard::SetKeyboardState(const Keys &state)
    {
        m_impl->m_keys = state;
    }

    Keys Keyboard::GetKeyboardState() const
    {
        return m_impl->m_keys;
    }

    KeyState Keyboard::GetKeyState(BND_Key key)
    {
        return m_impl->m_keys[static_cast<int>(key)];
    }

    void Keyboard::SetKeyState(BND_Key key, const KeyState &state)
    {
        m_impl->m_keys[static_cast<int>(key)] = state;
    }



} // BINDU