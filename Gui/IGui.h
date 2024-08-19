#ifndef IGUI_H
#define IGUI_H

namespace BINDU
{
	class IWindow;
	class IRenderer;

	class IGui
	{
	public:
		virtual ~IGui() {}

		virtual void Initialize(IWindow* window, IRenderer* renderer) = 0;

		virtual void Draw() = 0;

		virtual void Close() = 0;

	};
}

#endif