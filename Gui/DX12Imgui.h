#ifndef DX12IMGUI_H
#define DX12IMGUI_H

#include "IGui.h"
#include <memory>

namespace BINDU
{
	class DX12Imgui : public IGui
	{
	public:
		DX12Imgui();

		~DX12Imgui();

		void Initialize(IWindow* window, IRenderer* renderer) override;

		void Draw() override;
		
		void Close() override;

	private:
		class Impl;
		std::unique_ptr<Impl> m_impl{ nullptr };

	};
}

#endif