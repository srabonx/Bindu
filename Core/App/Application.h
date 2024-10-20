#ifndef BINDU_APPLICATION_H
#define BINDU_APPLICATION_H

#include <string>
#include "IBindu_App.h"
#include "../../Event/EventManager.h"
#include "../../Event/EventStruct.h"
#include "../Engine/IEngine.h"
#include "AppLayerStack.h"
#include "../../Utility/Common/CommonUtility.h"
#include "../../Window/IWindow.h"
#include "../Renderer/GraphicsContext.h"
#include "../Renderer/Renderer.h"

int main();

namespace BINDU
{
	struct AppSpecification
	{
		std::string Name = "Bindu Application";

		std::uint16_t Width{ 800 };
		std::uint16_t Height{ 600 };
	};

	struct AppStats
	{
		std::uint32_t UpTime{ 0 };
		std::uint16_t FPS{ 0 };
		std::uint16_t TPS{ 0 };
	};

	class Application
	{
	public:
		Application(const AppSpecification& specification);

		virtual ~Application();

		void	ProcessEvent(EVENT::BND_Event event);

		void	Initialize();

		void	Update(double dt);

		void	Render();

		void	Close();

		void	GetAppStat(AppStats stats);

		void	PushLayer(AppLayer* layer);
		void	PopLayer(AppLayer* layer);

		static Application& Get() { return *s_instance; }

		const AppSpecification& GetSpecification() const { return m_specification; }

	private:

		void	Run();

		void	OnMinimize();

		void	OnRestored();

		void	OnResize(std::uint16_t width, std::uint16_t height);

	private:
		static Application* s_instance;
		friend int ::main();

	private:
		AppSpecification	m_specification;
		AppStats			m_appStats;

		Scoped<IWindow>	m_window{ nullptr };

		Ref<Renderer>		m_renderer{ nullptr };
		Ref<GraphicsContext>	m_graphicsContext{ nullptr };

		bool				m_isRunning{ true };
		bool				m_minimized{ false };

		AppLayerStack		m_layerStack;

		EventManager		m_eventManager;
	};



	// To be defined in client
	Application* CreateApplication();


}

#endif