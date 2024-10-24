#ifndef BINDU_DIRECTX_SHADER_H
#define BINDU_DIRECTX_SHADER_H

#include <unordered_map>
#include <wrl/client.h>
#include <d3d12.h>

#include "../Shader.h"

namespace BINDU
{
	class Texture;
	using namespace Microsoft::WRL;

	class DirectXShader : public Shader
	{
		friend class DirectXRenderer;
	public:

		DirectXShader(const ShaderSpecification& specification, const ComPtr<ID3D12GraphicsCommandList>& cmdList);

		void Bind() override;

		const ShaderSpecification& GetSpecification() const override;

		void BindUniformBuffer(int slot, int offset, UniformBuffer* buffer) override;

		void BindTexture(int slot, Texture* texture) override;

		void SetInt(const std::string& name, int slot, int value) override;

		void SetIntArray(const std::string& name, int slot, int* values, std::uint32_t count) override;

		void SetFloat(const std::string& name, int slot, float value) override;

		void SetFloat2(const std::string& name, int slot, XMFLOAT2 value) override;

		void SetFloat3(const std::string& name, int slot, XMFLOAT3 value) override;

		void SetFloat4(const std::string& name, int slot, XMFLOAT4 value) override;

		void SetMat4(const std::string& name, int slot, XMFLOAT4X4 value) override;

		bool operator ==(const Shader& other) override;

	private:
		ShaderSpecification					m_specification;

		ComPtr<ID3D12GraphicsCommandList>	m_commandList{ nullptr };

		ComPtr<ID3D12RootSignature>			m_rootSig{ nullptr };

		std::vector<D3D12_INPUT_ELEMENT_DESC>	m_inputElementDesc;

		ComPtr<ID3D12PipelineState>			m_graphicsPSO{ nullptr };

		ComPtr<ID3D12PipelineState>			m_computePSO{ nullptr };

		ComPtr<ID3DBlob>					m_vsBlob{ nullptr };
		ComPtr<ID3DBlob>					m_psBlob{ nullptr };
		ComPtr<ID3DBlob>					m_gsBlob{ nullptr };
		ComPtr<ID3DBlob>					m_csBlob{ nullptr };

		D3D12_SHADER_BYTECODE				m_vsByteCode{ nullptr };
		D3D12_SHADER_BYTECODE				m_psByteCode{ nullptr };
		D3D12_SHADER_BYTECODE				m_gsByteCode{ nullptr };
		D3D12_SHADER_BYTECODE				m_csByteCode{ nullptr };

		std::unordered_map<int, int>		m_cbSlotToRootParam;
		std::unordered_map<int, int>		m_texSlotToRootParam;
		std::unordered_map<int, int>		m_constantsSlotToRootParam;

	};
}

#endif

