
//
// Created by letsd on 31-Jul-24.
//

#ifndef BINDU_MOUSE_H
#define BINDU_MOUSE_H

namespace BINDU
{
	enum class BND_Button
	{
		BND_NONE,
		BND_LEFT,
		BND_RIGHT,
		BND_MIDDLE
	};

	struct MousePos
	{
		int x{ 0 };
		int y{ 0 };
	};

	struct ButtonState
	{
		bool isDown{ false };
		bool wasDown{ false };
	};

	class Mouse
	{
	public:
		Mouse();
		~Mouse();

		// Get the state of the given button
		ButtonState GetButtonState(BND_Button button) const;

		// Set the state of the given button
		void SetButtonState(BND_Button button, const ButtonState& state) const;

		// Get the mouse position
		MousePos GetMousePos() const;

		// Set the mouse position
		void SetMousePos(MousePos pos) const;

		bool IsMouseDragged() const;

		void SetMouseDragged(bool drag) const;

		bool IsMouseMoved() const;

		void SetMouseMove(bool move) const;

	private:
		class Impl;
		Impl* m_impl{ nullptr };
	};
}


#endif //BINDU_MOUSE_H
