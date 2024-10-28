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
        for(auto& key : m_impl->m_keys)
        {
            key = KeyState::NONE;
        }
    }


    Keyboard::~Keyboard()
    {
        delete m_impl;
    }

    void Keyboard::SetKeyboardState(const Keys &state) const
    {
        m_impl->m_keys = state;
    }

    void Keyboard::Update() const
    {
        for(auto& state : m_impl->m_keys)
        {
            if (state == KeyState::PRESSED)
                state = KeyState::HELD;
            else if (state == KeyState::RELEASED)
                state = KeyState::NONE;
        }
    }

    Keys Keyboard::GetKeyboardState() const
    {
        return m_impl->m_keys;
    }

    KeyState Keyboard::GetKeyState(BND_Key key) const
    {
        return m_impl->m_keys[static_cast<int>(key)];
    }

    void Keyboard::SetKeyState(BND_Key key, const KeyState &state) const
    {
        m_impl->m_keys[static_cast<int>(key)] = state;
    }



} // BINDU