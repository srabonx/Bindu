#ifndef IGUI_H
#define IGUI_H

#include "Common/imgui.h"

namespace BINDU
{
	class IWindow;
	class IRenderer;

	class IGui
	{
	public:
		virtual ~IGui() {}

		virtual void Initialize(IWindow* window, IRenderer* renderer) = 0;

		virtual void BeginDraw() = 0;

		virtual void EndDraw() = 0;

		virtual void Close() = 0;

		virtual ImGuiIO* GetIo() = 0;
		virtual ImGuiContext* GetContext() = 0;

	};
}

#endif