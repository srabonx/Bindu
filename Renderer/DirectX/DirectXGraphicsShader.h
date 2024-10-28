#ifndef BINDU_DIRECTX_GRAPHICS_SHADER_H
#define BINDU_DIRECTX_GRAPHICS_SHADER_H

#include <unordered_map>
#include <wrl/client.h>
#include <d3d12.h>

#include "../Shader.h"

namespace BINDU
{
	class Texture;
	using namespace Microsoft::WRL;

	using ShaderSlot = std::uint16_t;
	using RootParamIndex = std::uint16_t;

	class DirectXGraphicsShader : public Shader
	{
		friend class DirectXRenderer;
	public:

		DirectXGraphicsShader(const ShaderSpecification& specification, const ComPtr<ID3D12GraphicsCommandList>& cmdList);

		void Bind() override;

		const ShaderSpecification& GetSpecification() const override;

		void BindUniformBuffer(std::string name, int offset, UniformBuffer* buffer) override;

		void BindTexture(std::string name, Texture* texture) override;

		void SetInt(const std::string& name, int value) override;

		void SetIntArray(const std::string& name, int* values, std::uint32_t count) override;

		void SetFloat(const std::string& name, float value) override;

		void SetFloat2(const std::string& name, XMFLOAT2 value) override;

		void SetFloat3(const std::string& name, XMFLOAT3 value) override;

		void SetFloat4(const std::string& name, XMFLOAT4 value) override;

		void SetMat4(const std::string& name, XMFLOAT4X4 value) override;

		bool operator ==(const Shader& other) override;

	private:
		ShaderSpecification					m_specification;

		ComPtr<ID3D12GraphicsCommandList>	m_commandList{ nullptr };

		ComPtr<ID3D12RootSignature>			m_rootSig{ nullptr };

		std::vector<D3D12_INPUT_ELEMENT_DESC>	m_inputElementDesc;

		ComPtr<ID3D12PipelineState>			m_graphicsPSO{ nullptr };

		ComPtr<ID3DBlob>					m_vsBlob{ nullptr };
		ComPtr<ID3DBlob>					m_psBlob{ nullptr };
		ComPtr<ID3DBlob>					m_gsBlob{ nullptr };

		D3D12_SHADER_BYTECODE				m_vsByteCode{ nullptr };
		D3D12_SHADER_BYTECODE				m_psByteCode{ nullptr };
		D3D12_SHADER_BYTECODE				m_gsByteCode{ nullptr };

		std::unordered_map<std::string, std::pair<ShaderSlot, RootParamIndex>>		m_shaderParameterMap;
	};
}

#endif

