#ifndef BINDU_APPLICATION_H
#define BINDU_APPLICATION_H

#include <string>
#include "IBindu_App.h"
#include "../../Event/EventManager.h"
#include "../../Event/EventStruct.h"
#include "AppLayerStack.h"
#include "../../Input/InputManager.h"
#include "../../Utility/Common/CommonUtility.h"
#include "../../Window/IWindow.h"
#include "../../Renderer/GraphicsContext.h"
#include "../../Renderer/Renderer.h"

//int main();

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
		Application(AppSpecification specification);

		virtual ~Application() = default;

		void	ProcessEvent(EVENT::BND_Event event);

		virtual void	Initialize();

		void	Run();

		void	Update(double dt);

		void	Render();

		void	Close();

		virtual void GetAppStat(AppStats stats);

		void	PushLayer(AppLayer* layer);
		void	PopLayer(AppLayer* layer);

		static Application& Get() { return *s_instance; }

		const AppSpecification& GetSpecification() const { return m_specification; }

		GraphicsContext* GetGraphicsContext() const;
		Renderer*		 GetRenderer() const;

	private:


		void	OnMinimize();

		void	OnRestored();

		void	OnResize(std::uint16_t width, std::uint16_t height) const;


	protected:
		Scoped<IWindow>		m_window{ nullptr };

	private:
		static Application* s_instance;
		//friend int ::main();

	private:
		AppSpecification	m_specification;
		AppStats			m_appStats;

		Ref<Renderer>		m_renderer{ nullptr };
		Ref<GraphicsContext>	m_graphicsContext{ nullptr };
		Ref<CommandList>	m_commandList{ nullptr };

		bool				m_isRunning{ true };
		bool				m_minimized{ false };

		AppLayerStack		m_layerStack;

		EventCallbackToken	m_callbackToken;
		InputManager		m_inputManager;
	};



	// To be defined in client
//	Application* CreateApplication();


}

#endif