#ifndef DEFAULTRENDERCONTEXT_H
#define DEFAULTRENDERCONTEXT_H

#include <memory>

#include "D3DDeviceManager.h"
#include "D3DCommandContext.h"
#include "D3DFence.h"
#include "D3DSwapChain.h"
#include "RenderTexture.h"
#include "D3DPipelineStateManager.h"

namespace BINDU
{
	//class D3DDeviceManager;
	//class D3DCommandContext;
	//class D3DSwapChain;
	//class D3DFence;
	class GameObject;
	//class RenderTexture;


	class DefaultRenderContext
	{
	public:

		DefaultRenderContext(std::uint16_t width, std::uint16_t height, HWND hwnd);
		~DefaultRenderContext() = default;

		void		Initialize();

		void		BeginRender();

		void		RenderGameObject(GameObject* gameObject, const std::string& pipelineName) const;

		void		EndRender();

		void		Resize(std::uint16_t width, std::uint16_t height);

		void		Close() const;

		D3DPipelineStateManager* GetPipelineStateManager() const;

		D3DDeviceManager*	GetDeviceManager() const;

		D3DCommandContext*	GetCommandContext() const;

	private:
		std::shared_ptr<D3DDeviceManager>		m_deviceManager{ nullptr };

		std::unique_ptr<D3DCommandContext>		m_commandContext{ nullptr };

		std::unique_ptr<D3DSwapChain>			m_swapChain{ nullptr };

		std::unique_ptr<D3DFence>				m_fence{ nullptr };

		std::unique_ptr<D3DPipelineStateManager> m_pipelineStateManager{nullptr};

		RenderTexture*							m_renderTexture{ nullptr };

		std::uint16_t	m_width{ 0 };
		std::uint16_t	m_height{ 0 };

		HWND m_hwnd;
	};
}

#endif