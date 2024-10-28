#include "Mouse.h"

#include <array>


class BINDU::Mouse::Impl
{
public:
	std::array<ButtonState, 4> m_buttons;
	MousePos m_mousePos;

	MouseDelta m_mouseDelta;

	int m_mouseWheelDelta{ 0 };

};


BINDU::Mouse::Mouse() : m_impl(new Impl())
{
	for(auto& b : m_impl->m_buttons)
	{
		b = ButtonState::NONE;
	}
}

BINDU::Mouse::~Mouse()
{
	delete m_impl;
}

void BINDU::Mouse::Update() const
{
	for(auto& state : m_impl->m_buttons)
	{
		if (state == ButtonState::PRESSED)
			state = ButtonState::HELD;
		else if (state == ButtonState::RELEASED)
			state = ButtonState::NONE;
	}
}

BINDU::ButtonState BINDU::Mouse::GetButtonState(BND_Button button) const
{
	return m_impl->m_buttons[static_cast<int>(button)];
}

void BINDU::Mouse::SetButtonState(BND_Button button, const ButtonState& state) const
{
	m_impl->m_buttons[static_cast<int>(button)] = state;
}

BINDU::MousePos BINDU::Mouse::GetMousePos() const
{
	return m_impl->m_mousePos;
}

int BINDU::Mouse::GetMouseX() const
{
	return m_impl->m_mousePos.x;
}

int BINDU::Mouse::GetMouseY() const
{
	return m_impl->m_mousePos.y;
}


void BINDU::Mouse::SetMousePos(MousePos pos) const
{
	m_impl->m_mousePos = pos;
}

