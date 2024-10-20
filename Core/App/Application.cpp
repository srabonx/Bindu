#include "Application.h"

#include "../../Debug/Profiler.h"
#include "../../Debug//Assert.h"
#include "../../Window/Win32Window.h"
#include "../Renderer/RendererFactory.h"


namespace  BINDU
{

	Application* Application::s_instance = nullptr;

	Application::Application(const AppSpecification& specification) : m_specification(specification)
	{
		BINDU_PROFILE_FUNCTION();

		BINDU_CORE_ASSERT(!s_instance, "Application already exists!");

		s_instance = this;
	}

	Application::~Application()
	{
	}

	void Application::ProcessEvent(EVENT::BND_Event event)
	{
		BINDU_PROFILE_FUNCTION();

		switch (event.type)
		{
		case EVENT::Type::DESTROY:
		case EVENT::Type::QUIT:
			Close();
			break;
		case EVENT::Type::WINDOW_MINIMIZED:
			OnMinimize();
			break;
		case EVENT::Type::WINDOW_RESTORED:
			OnRestored();
			break;
		case EVENT::Type::WINDOW_EXIT_RESIZING:
			OnResize(event.Ev_Window.Width, event.Ev_Window.Height);
			break;

		default:
			break;
		}

		for(const auto& layer : m_layerStack)
		{
			layer->OnEvent(event);
		}
	}

	void Application::Initialize()
	{
		BINDU_PROFILE_FUNCTION();

		m_eventManager.BindListenerFn(BINDU_BIND_EVENT_FN(Application::ProcessEvent));

		HINSTANCE hInstance = GetModuleHandle(NULL);

		m_window = CreateScoped<Win32Window>(hInstance);
		m_window->Create(m_specification.Width, m_specification.Height, m_specification.Name);

		m_window->RegisterEventManager(&m_eventManager);

		// TODO: Add renderer Initialization
		m_renderer = RendererFactory::CreateRenderer(RendererFactory::API::DirectX);
		m_graphicsContext = RendererFactory::CreateGraphicsContext(m_window.get(), m_renderer);

	}

	void Application::Update(double dt)
	{
		BINDU_PROFILE_FUNCTION();

		for (const auto& layer : m_layerStack)
			layer->OnUpdate(dt);
	}

	void Application::Render()
	{
		BINDU_PROFILE_FUNCTION();

		for (const auto& layer : m_layerStack)
			layer->OnRender();
	}

	void Application::Close()
	{
		BINDU_PROFILE_FUNCTION();
		m_eventManager.Clear();

		m_isRunning = false;
	}

	void Application::GetAppStat(AppStats stats)
	{
	}

	void Application::PushLayer(AppLayer* layer)
	{
		BINDU_PROFILE_FUNCTION();

		m_layerStack.PushLayer(layer);
	}

	void Application::PopLayer(AppLayer* layer)
	{
		BINDU_PROFILE_FUNCTION();

		m_layerStack.PopLayer(layer);
	}

	void Application::Run()
	{
		BINDU_PROFILE_FUNCTION();

		using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

		TimePoint endTime = std::chrono::steady_clock::now();
		TimePoint startTime = std::chrono::steady_clock::now();

		std::chrono::steady_clock::duration elapsedTime = {};

		double          fpsElapsedTime{ 0.0 };
		std::uint32_t   fpsCounter{ 0 };

		while(m_isRunning)
		{
			BINDU_PROFILE_SCOPE("MainLoop");


			startTime = std::chrono::steady_clock::now();

			elapsedTime = startTime - endTime;

			endTime = startTime;

			long delta = static_cast<long>(elapsedTime.count());

			double deltaSec = delta * 0.000000001;

			fpsElapsedTime += deltaSec;

			if(!m_minimized)
			{
				Update(deltaSec);

				Render();
			}

			{
				BINDU_PROFILE_SCOPE("Window Update");

				m_window->Update();
			}

			fpsCounter++;

			if (fpsElapsedTime >= 1.000000000)
			{
				m_appStats.UpTime++;
				m_appStats.FPS = fpsCounter;
				m_appStats.TPS = fpsCounter;

				s_instance->GetAppStat(m_appStats);

				fpsCounter = 0;

				fpsElapsedTime = 0.0;
				}
		}
	}

	void Application::OnMinimize()
	{
		m_minimized = true;
	}

	void Application::OnRestored()
	{
		m_minimized = false;
	}

	void Application::OnResize(std::uint16_t width, std::uint16_t height)
	{
		BINDU_PROFILE_FUNCTION();


	}
}


