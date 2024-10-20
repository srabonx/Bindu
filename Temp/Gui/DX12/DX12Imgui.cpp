#include "DX12Imgui.h"

#include <sstream>

#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include <wrl/client.h>

#include "../../Window/IWindow.h"
#include "../../Core/Renderer/DirectX/DX12Renderer.h"
#include "../../Core/Renderer/DirectX/Utility/D3DUtillity.h"
#include "../../Core/Renderer/DirectX/Utility/d3dx12.h"
#include "../../Core/Renderer/DirectX/Utility/DescriptorHeap.h"
#include "../../Logger/Logger.h"
#include "../Common/imgui_internal.h"

using namespace Microsoft::WRL;

namespace BINDU
{



	class DX12Imgui::Impl
	{
	public:
		IWindow*				m_window{ nullptr };
		DX12Renderer*			m_renderer{ nullptr };

		// ImGUi context
		ImGuiContext*			m_imguiContext{ nullptr };

		// ImGui IO
		ImGuiIO*				m_imguiIo;

		ImGuiID					m_dockspaceId;

		// Central dock node
		ImGuiDockNode*			m_centralDockNode{ nullptr };

		// Central ImGui window
		ImGuiWindowClass	    m_centralWindow;

		bool					m_showDemoWindow;

		float					clearColor[4] = {0.0f,0.0f,0.0f,0.0f};

		// Console buffer
		std::stringstream		m_consoleBuffer;

	};



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
		m_impl->m_imguiIo = &ImGui::GetIO();
		m_impl->m_imguiIo->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable keyboard controls
		m_impl->m_imguiIo->ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Enable docking
		//m_impl->m_imguiIo->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		// Enable Multi-Viewports

		// Setup ImGui style
		ImGui::StyleColorsDark();

		// When Viewports are enable we tweak WindowRounding/ WindowBg so platform windows can look identical to regular ones
		ImGuiStyle& style = ImGui::GetStyle();
		if(m_impl->m_imguiIo->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Logger::Get()->Log(LogType::Info, "Creating SRV heap for GUI components");

		//m_impl->CreateSRVDescriptorHeap(m_impl->m_renderer->GetD3DDevice(), m_impl->m_imguiSrvHeap.ReleaseAndGetAddressOf());

		// Setup platform renderer backends
		bool win32Init = ImGui_ImplWin32_Init(m_impl->m_window->GetHandle());

		if(!win32Init)
		{
			Logger::Get()->Log(LogType::Error, "Error with ImGui win32 Init");
			return;
		}

		ID3D12DescriptorHeap*		srvHeap = m_impl->m_renderer->GetSrvHeap()->Heap().Get();


		D3D12_CPU_DESCRIPTOR_HANDLE	srvCpuHandle = m_impl->m_renderer->GetSrvHeap()->GetCPUHandleAt(SRDescriptors::SRGui);

		D3D12_GPU_DESCRIPTOR_HANDLE	srvGpuHandle = m_impl->m_renderer->GetSrvHeap()->GetGPUHandleAt(SRDescriptors::SRGui);


		bool dx12Init = ImGui_ImplDX12_Init(m_impl->m_renderer->GetD3DDevice(), 1, m_impl->m_renderer->GetRTVFormat(),
			srvHeap,
			srvCpuHandle,
			srvGpuHandle);

		if (!dx12Init)
		{
			Logger::Get()->Log(LogType::Error, "Error with ImGui dx12 Init");
			return;
		}

	}

	void DX12Imgui::BeginDraw()
	{
		// Start ImGui Frame
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		m_impl->m_dockspaceId = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_NoDockingInCentralNode | ImGuiDockNodeFlags_PassthruCentralNode, nullptr);
		m_impl->m_centralDockNode = ImGui::DockBuilderGetCentralNode(m_impl->m_dockspaceId);

		m_impl->m_centralWindow.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDockingOverMe;
		
	}

	void DX12Imgui::DrawToCentral(ImTextureID texture)
	{

		ImGui::SetNextWindowClass(&m_impl->m_centralWindow);
		ImGui::SetNextWindowDockID(m_impl->m_centralDockNode->ID, ImGuiCond_Always);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("GH-5921");
		ImGui::PopStyleVar();

		ImGui::Image(texture,
		             ImVec2(m_impl->m_centralDockNode->Size.x,
		                     m_impl->m_centralDockNode->Size.y));

		ImGui::End();
	}

	void DX12Imgui::EndDraw()
	{


		if (m_impl->m_showDemoWindow)
			ImGui::ShowDemoWindow(&m_impl->m_showDemoWindow);


		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, Engine!");

			ImGui::Checkbox("Demo Window", &m_impl->m_showDemoWindow);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
			ImGui::ColorEdit4("clear color", (float*)&m_impl->clearColor);

			if (ImGui::Button("Button"))
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			//			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / Fps, Fps);
			//			ImGui::Text("Application average %.3f ms/tick (%.1f TPS)", 1000.0f / Tps, Tps);
			ImGui::End();
		}

		{
			ImGui::Begin("Console");

			ImGui::Text("Central node: (%f, %f)..(%fx%f)", m_impl->m_centralDockNode->Pos.x,
				m_impl->m_centralDockNode->Pos.y, m_impl->m_centralDockNode->Size.x,
				m_impl->m_centralDockNode->Size.y);

			ImGui::Text(m_impl->m_consoleBuffer.str().c_str());

			m_impl->m_consoleBuffer.str(std::string());


			ImGui::End();
		}



		ImGui::Render();

		//m_impl->m_renderer->GetCommandList()->SetDescriptorHeaps(1, m_impl->m_imguiSrvHeap.GetAddressOf());
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_impl->m_renderer->GetCommandList());

		if (m_impl->m_imguiIo->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
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

	void DX12Imgui::WriteToConsole(const std::string& str) const
	{
		m_impl->m_consoleBuffer << str << "\n";
	}

	ImVec2 DX12Imgui::GetCentralNodeSize() const
	{
		if (m_impl->m_centralDockNode)
			return m_impl->m_centralDockNode->Size;
		else
			return ImVec2(200, 200);
	}

	ImGuiIO* DX12Imgui::GetIo()
	{
		return m_impl->m_imguiIo;
	}

	ImGuiContext* DX12Imgui::GetContext()
	{
		return m_impl->m_imguiContext;
	}

	float* DX12Imgui::GetClearColor()
	{
		return m_impl->clearColor;
	}
}
