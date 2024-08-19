#include "DX12Imgui.h"

#include <Common/imgui.h>
#include <dx12/imgui_impl_dx12.h>
#include <dx12/imgui_impl_win32.h>
#include <wrl/client.h>

#include "../Window/IWindow.h"
#include "../Core/Renderer/DirectX/DX12Renderer.h"
#include "../Core/Renderer/DirectX/Utility/D3DUtillity.h"
#include "../Logger/Logger.h"

using namespace Microsoft::WRL;

namespace BINDU
{



	class DX12Imgui::Impl
	{
	public:
		IWindow* m_window{ nullptr };
		DX12Renderer* m_renderer{ nullptr };

		ComPtr<ID3D12DescriptorHeap>	m_imguiSrvHeap{ nullptr };

		// ImGUi context
		ImGuiContext* m_imguiContext{ nullptr };

		// ImGui IO
		ImGuiIO	m_imguiIo;

		bool m_showDemoWindow{ true };

		// Create Descriptor heap
		void CreateSRVDescriptorHeap(ID3D12Device* pd3dDevice, ID3D12DescriptorHeap** ppDescriptorHeap);
	};

	void DX12Imgui::Impl::CreateSRVDescriptorHeap(ID3D12Device* pd3dDevice, ID3D12DescriptorHeap** ppDescriptorHeap)
	{

		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NumDescriptors = 1;
		heapDesc.NodeMask = 0;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		DXThrowIfFailed(pd3dDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(ppDescriptorHeap)));

	}


	DX12Imgui::DX12Imgui() : m_impl(std::make_unique<Impl>())
	{
		
	}

	DX12Imgui::~DX12Imgui()
	{

	}


	void DX12Imgui::Initialize(IWindow* window, IRenderer* renderer)
	{
		Logger::Get()->Log(LogType::Info, "Initializing GUI components");

		m_impl->m_window = window;
		m_impl->m_renderer = reinterpret_cast<DX12Renderer*>(renderer);

		// Setup IMGUI context
		IMGUI_CHECKVERSION();

		m_impl->m_imguiContext = ImGui::CreateContext();
		m_impl->m_imguiIo = ImGui::GetIO();

		m_impl->m_imguiIo.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable keyboard controls
		m_impl->m_imguiIo.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Enable docking
		m_impl->m_imguiIo.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		// Enable Multi-Viewports

		// Setup ImGui style
		ImGui::StyleColorsDark();

		// When Viewports are enable we tweak WindowRounding/ WindowBg so platform windows can look identical to regular ones
		ImGuiStyle& style = ImGui::GetStyle();
		if(m_impl->m_imguiIo.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Logger::Get()->Log(LogType::Info, "Creating SRV heap for GUI components");
		m_impl->CreateSRVDescriptorHeap(m_impl->m_renderer->GetD3DDevice(), m_impl->m_imguiSrvHeap.ReleaseAndGetAddressOf());

		// Setup platform renderer backends
		bool win32Init = ImGui_ImplWin32_Init(m_impl->m_window->GetHandle());

		if(!win32Init)
		{
			Logger::Get()->Log(LogType::Error, "Error with ImGui win32 Init");
			return;
		}

		bool dx12Init = ImGui_ImplDX12_Init(m_impl->m_renderer->GetD3DDevice(), 1, m_impl->m_renderer->GetRTVFormat(),
			m_impl->m_imguiSrvHeap.Get(),
			m_impl->m_imguiSrvHeap->GetCPUDescriptorHandleForHeapStart(),
			m_impl->m_imguiSrvHeap->GetGPUDescriptorHandleForHeapStart());

		if (!dx12Init)
		{
			Logger::Get()->Log(LogType::Error, "Error with ImGui dx12 Init");
			return;
		}

	}

	void DX12Imgui::Draw()
	{
		// Start ImGui Frame
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (m_impl->m_showDemoWindow)
			ImGui::ShowDemoWindow(&m_impl->m_showDemoWindow);

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &m_impl->m_showDemoWindow);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &m_impl->m_showDemoWindow);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit4("clear color", (float*)&m_impl->m_showDemoWindow); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_impl->m_imguiIo.Framerate, m_impl->m_imguiIo.Framerate);
			ImGui::End();
		}

		ImGui::Render();

		m_impl->m_renderer->GetCommandList()->SetDescriptorHeaps(1, m_impl->m_imguiSrvHeap.GetAddressOf());
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_impl->m_renderer->GetCommandList());
		
		if (m_impl->m_imguiIo.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault(nullptr, m_impl->m_renderer->GetCommandList());
		}
		
	}

	void DX12Imgui::Close()
	{
		Logger::Get()->Log(LogType::Info, "Closing Gui Components");

		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext(m_impl->m_imguiContext);
	}

}
