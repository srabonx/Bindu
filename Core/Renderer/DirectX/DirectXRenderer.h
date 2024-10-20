#ifndef BINDU_DIRECTX_RENDERER_H
#define BINDU_DIRECTX_RENDERER_H

#include "D3DCommandContext.h"
#include "../Renderer.h"
#include "D3DDeviceManager.h"
#include "../../../Utility/Common/CommonUtility.h"

namespace BINDU
{

	class DirectXRenderer : public Renderer
	{
		friend class DirectXGraphicsContext;

	public:
		void Initialize() override;

		void Close() override;

		Ref<Texture> CreateTexture(const std::string& path) override;

		Ref<Texture> CreateDepthStencilTexture(const TextureSpecification& specification, bool isShaderVisible = false) override;

		Ref<Texture> CreateRenderTexture(const Texture* depthStencilTexture, const TextureSpecification& specification, bool isShaderVisible = false) override;

		Ref<Shader>	 CreateShader(const ShaderSpecification& specification) override;


		[[nodiscard]] const Ref<D3DCommandContext>&	GetCommandContext() const;


		Ref<Texture> CreateRenderTexture(const ComPtr<ID3D12Resource>& d3dResource, const Texture* depthStencilTexture, const TextureSpecification& specification, bool isShaderVisible = false);
	private:

		Ref<D3DDeviceManager>		m_deviceManager{ nullptr };

		ComPtr<ID3D12GraphicsCommandList>	m_commandList{ nullptr };

		Ref<D3DCommandContext>		m_commandContext{ nullptr };
	};
}

#endif