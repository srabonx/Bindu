#ifndef BINDU_DIRECTX_SHADER_H
#define BINDU_DIRECTX_SHADER_H

#include <unordered_map>
#include <wrl/client.h>

#include "DirectXCommandList.h"
#include "DirectXConstantBuffer.h"
#include "../Shader.h"

namespace BINDU
{
	using namespace Microsoft::WRL;
	class DirectXShader : public Shader
	{
		friend class DirectXRenderer;
	public:

		DirectXShader(const ShaderSpecification& specification, const ComPtr<ID3D12GraphicsCommandList>& cmdList);

		void Bind() override;

		const ShaderSpecification& GetSpecification() const override;

		void SetUniformBuffer(int offset, UniformBuffer* buffer) override;

		void BindUniformBuffer(int slot) override;

		void BindTexture(int slot, Texture* texture) override;

		void SetData(const std::string& name, const void* data, std::uint32_t size) override;

		void SetInt(const std::string& name, int value) override;

		void SetIntArray(const std::string& name, int* values, std::uint32_t count) override;

		void SetFloat(const std::string& name, float value) override;

		void SetFloat2(const std::string& name, XMFLOAT2 value) override;

		void SetFloat3(const std::string& name, XMFLOAT3 value) override;

		void SetFloat4(const std::string& name, XMFLOAT4 value) override;

		void SetMat4(const std::string& name, XMMATRIX value) override;

	private:
		DirectXConstantBuffer*				m_tempBuffer{ nullptr };

		DynamicVoidBuffer					m_dataAccumulator;

		int									m_offset{ 0 };

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

	};
}

#endif

