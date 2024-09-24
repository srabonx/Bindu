#ifndef DEFAULTRENDERCONTEXT_H
#define DEFAULTRENDERCONTEXT_H

#include <memory>

#include "D3DDeviceManager.h"
#include "D3DCommandContext.h"
#include "D3DConstantBuffer.h"
#include "D3DFence.h"
#include "D3DSwapChain.h"
#include "RenderTexture.h"
#include "D3DPipelineStateManager.h"
#include "FlyFrame.h"

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

		void		BeginRender(float clearColor[4]);

		void		BindPipeline(const std::string& pipelineName, const std::string& rootSigName) const;

		void		BindPipeline(ID3D12PipelineState* pipelineState, ID3D12RootSignature* rootSignature) const;

		void		BindPerPassConstant(std::uint16_t rootParamSlot) const;

		void		RenderGameObject(GameObject* gameObject);

		void		EndRender();

		void		UpdatePerPassConstants(const PerPassConstants& perPassConstants) const;

		void		Resize(std::uint16_t width, std::uint16_t height);

		void		Close() const;
		
		D3DPipelineStateManager*	GetPipelineStateManager() const;

		D3DDeviceManager*			GetDeviceManager() const;

		D3DCommandContext*			GetCommandContext() const;

		D3DFence*					GetFence() const;

		D3DSwapChain*				GetSwapChain() const;

		std::shared_ptr<D3DConstantBuffer>&			GetConstantBuffer();



	private:
		std::shared_ptr<D3DDeviceManager>		m_deviceManager{ nullptr };

		std::unique_ptr<D3DCommandContext>		m_commandContext{ nullptr };

		std::unique_ptr<D3DSwapChain>			m_swapChain{ nullptr };

		std::unique_ptr<D3DFence>				m_fence{ nullptr };

		std::unique_ptr<D3DPipelineStateManager> m_pipelineStateManager{nullptr};

		std::unique_ptr<FlyFrame>				m_flyFrame{ nullptr };

		std::shared_ptr<D3DConstantBuffer>		m_constantBuffer;

		D3D12_VIEWPORT							m_viewport;

		D3D12_RECT								m_scissorRect;

		std::uint16_t							m_width{ 0 };
		std::uint16_t							m_height{ 0 };

		HWND									m_hwnd;
	};
}

#endif