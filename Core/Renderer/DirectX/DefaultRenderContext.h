#ifndef DEFAULTRENDERCONTEXT_H
#define DEFAULTRENDERCONTEXT_H

#include <DirectXMath.h>
#include <memory>

#include "D3DDeviceManager.h"
#include "D3DCommandContext.h"
#include "D3DFence.h"
#include "D3DSwapChain.h"
#include "RenderTexture.h"
#include "D3DPipelineStateManager.h"

namespace BINDU
{
	using namespace DirectX;
	//class D3DDeviceManager;
	//class D3DCommandContext;
	//class D3DSwapChain;
	//class D3DFence;
	class GameObject;
	//class RenderTexture;

	struct PerPassConstants
	{
		XMFLOAT4X4	ViewMatrix;

		XMFLOAT4X4	ProjMatrix;

		XMFLOAT4X4	ViewProjMatrix;

		XMFLOAT4X4	InvViewMatrix;

		XMFLOAT4X4	InvProjMatrix;

		XMFLOAT4X4	InvViewProjMatrix;

		XMFLOAT3	EyePos;		// Camera Pos
	};


	class DefaultRenderContext
	{
	public:

		DefaultRenderContext(std::uint16_t width, std::uint16_t height, HWND hwnd);
		~DefaultRenderContext() = default;

		void		Initialize();

		void		BeginRender(float clearColor[4]);

		void		BindPipeline(const std::string& pipelineName) const;

		void		RenderGameObject(GameObject* gameObject) const;

		void		EndRender();

		void		Resize(std::uint16_t width, std::uint16_t height);

		void		Close() const;
		
		D3DPipelineStateManager*	GetPipelineStateManager() const;

		D3DDeviceManager*			GetDeviceManager() const;

		D3DCommandContext*			GetCommandContext() const;

		D3DFence*					GetFence() const;

		D3DSwapChain*				GetSwapChain() const;



	private:
		std::shared_ptr<D3DDeviceManager>		m_deviceManager{ nullptr };

		std::unique_ptr<D3DCommandContext>		m_commandContext{ nullptr };

		std::unique_ptr<D3DSwapChain>			m_swapChain{ nullptr };

		std::unique_ptr<D3DFence>				m_fence{ nullptr };

		std::unique_ptr<D3DPipelineStateManager> m_pipelineStateManager{nullptr};

		D3D12_VIEWPORT							m_viewport;

		D3D12_RECT								m_scissorRect;

		std::uint16_t							m_width{ 0 };
		std::uint16_t							m_height{ 0 };

		HWND									m_hwnd;
	};
}

#endif