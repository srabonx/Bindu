#ifndef BINDU_ENTRY_POINT_H
#define BINDU_ENTRY_POINT_H
#include "Application.h"
#include "../../Logger/Logger.h"
#include "../../Debug/Profiler.h"

/* extern BINDU::Application* BINDU::CreateApplication();

int main()
{
	BINDU::Logger::Init();

	BINDU_PROFILE_BEGIN_SESSION("StartUp", "BINDU-Profile-Startup.json");

	auto app = BINDU::CreateApplication();

	app->Initialize();

	BINDU_PROFILE_END_SESSION();

	BINDU_PROFILE_BEGIN_SESSION("Runtime", "BINDU-Profile-Runtime.json");

	app->Run();

	BINDU_PROFILE_END_SESSION();

	BINDU_PROFILE_BEGIN_SESSION("ShutDown", "BINDU-Profile-Shutdown.json");

	app->Close();

	delete app;

	BINDU_PROFILE_END_SESSION();

} */


#endif