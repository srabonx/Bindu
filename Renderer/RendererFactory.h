#ifndef BINDU_RENDERER_FACTORY_H
#define BINDU_RENDERER_FACTORY_H

#include "CommandList.h"
#include "../Utility/Common/CommonUtility.h"
#include "Renderer.h"
#include "GraphicsContext.h"
#include "../Window/IWindow.h"


namespace BINDU
{
	class RendererFactory
	{
	public:
		enum class API
		{
			None = 0, DirectX = 1
		};

		static Ref<Renderer>				CreateRenderer(API api);

		static Ref<GraphicsContext>			CreateGraphicsContext(IWindow* window,const Ref<Renderer>& renderer);

		static API							GetAPI();

	private:
		static API s_API;
	};
}

#endif