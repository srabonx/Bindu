#include "RendererFactory.h"
#include "RendererFactory.h"

#include "../Debug/Assert.h"
#include "../Debug/Profiler.h"
#include "DirectX/DirectXRenderer.h"
#include "DirectX/DirectXGraphicsContext.h"
#include "DirectX/DirectXCommandList.h"

namespace BINDU
{
	RendererFactory::API RendererFactory::s_API = RendererFactory::API::None;


	Ref<Renderer> RendererFactory::CreateRenderer(API api)
	{
		BINDU_PROFILE_FUNCTION();

		s_API = api;

		switch (api)
		{
		case API::None:
			BINDU_CORE_ASSERT(false, "RenderAPI::None is not supported!"); return nullptr;
			break;
		case API::DirectX:
			return CreateRef<DirectXRenderer>();
			break;
		}

		BINDU_CORE_ASSERT(false, "Unsupported Render API");

		return nullptr;
	}

	Ref<GraphicsContext> RendererFactory::CreateGraphicsContext(IWindow* window, const Ref<Renderer>& renderer)
	{
		BINDU_PROFILE_FUNCTION();

		switch (s_API)
		{
		case API::None:
			BINDU_CORE_ASSERT(false, "GraphicsContext::None is not supported!"); return nullptr;
			break;
		case API::DirectX:
			auto dxRenderer = std::static_pointer_cast<DirectXRenderer>(renderer);
			return CreateRef<DirectXGraphicsContext>(window, dxRenderer);
				break;
		}

		BINDU_CORE_ASSERT(false, "Unsupported Graphics Context");

		return nullptr;
	}


	RendererFactory::API RendererFactory::GetAPI()
	{
		return s_API;
	}
}
