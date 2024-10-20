#ifndef DX12IMGUI_H
#define DX12IMGUI_H

#include "../IGui.h"

#include "../Common/imgui.h"

#include <memory>
#include <string>

namespace BINDU
{
	class DX12Imgui : public IGui
	{
	public:
		DX12Imgui();

		~DX12Imgui();

		void Initialize(IWindow* window, IRenderer* renderer) override;

		void BeginDraw() override;

		void DrawToCentral(ImTextureID texture);

		void  EndDraw() override;
		
		void Close() override;

		void		WriteToConsole(const std::string& str) const;

		ImVec2		GetCentralNodeSize() const;

		ImGuiIO* GetIo() override;

		ImGuiContext* GetContext() override;

		float*	GetClearColor();

	private:
		class Impl;
		std::unique_ptr<Impl> m_impl{ nullptr };

	};
}

#endif