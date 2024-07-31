#include "Mouse.h"

#include <array>


class BINDU::Mouse::Impl
{
public:
	std::array<ButtonState, 4> m_buttons;
	MousePos m_mousePos;

	bool	m_isDragged{ false };
	bool	m_isMoved{ false };
};


BINDU::Mouse::Mouse() : m_impl(new Impl())
{

}

BINDU::Mouse::~Mouse()
{
	delete m_impl;
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

void BINDU::Mouse::SetMousePos(MousePos pos) const
{
	m_impl->m_mousePos = pos;
}

bool BINDU::Mouse::IsMouseDragged() const
{
	return m_impl->m_isDragged;
}

void BINDU::Mouse::SetMouseDragged(bool drag) const
{
	m_impl->m_isDragged = drag;
}

bool BINDU::Mouse::IsMouseMoved() const
{
	return m_impl->m_isMoved;
}

void BINDU::Mouse::SetMouseMove(bool move) const
{
	m_impl->m_isMoved = move;
}
