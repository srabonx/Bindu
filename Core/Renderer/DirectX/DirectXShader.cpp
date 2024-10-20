#include "DirectXShader.h"

#include "../../../Debug/Assert.h"

namespace BINDU
{
	DirectXShader::DirectXShader(const ShaderSpecification& specification, const ComPtr<ID3D12GraphicsCommandList>& cmdList) :
			m_specification(specification), m_commandList(cmdList), m_dataAccumulator(256)
	{
	}

	void DirectXShader::Bind()
	{
		if (m_graphicsPSO)
		{
			m_commandList->SetPipelineState(m_graphicsPSO.Get());
			m_commandList->SetGraphicsRootSignature(m_rootSig.Get());
		}
		if (m_computePSO)
		{
			m_commandList->SetPipelineState(m_computePSO.Get());
			m_commandList->SetComputeRootSignature(m_rootSig.Get());
		}
	}

	const ShaderSpecification& DirectXShader::GetSpecification() const
	{
		return m_specification;
	}

	void DirectXShader::SetUniformBuffer(int offset, UniformBuffer* buffer)
	{
		m_tempBuffer = static_cast<DirectXConstantBuffer*>(buffer);

		BINDU_CORE_ASSERT((m_tempBuffer != nullptr), "Invalid Uniform Buffer!");

		m_offset = offset;
	}

	void DirectXShader::BindUniformBuffer(int slot)
	{
		m_tempBuffer->SetData(m_dataAccumulator.GetBufferData(), m_dataAccumulator.GetBufferSize(), m_offset);
		auto rootParamSlot = m_cbSlotToRootParam[slot];
		m_commandList->SetGraphicsRootConstantBufferView(rootParamSlot, m_tempBuffer->GetGPUVirtualAddressAt(m_offset));
	}

	void DirectXShader::BindTexture(int slot, Texture* texture)
	{
		auto rootParamSlot = m_texSlotToRootParam[slot];
		texture->Bind(rootParamSlot);
	}

	void DirectXShader::SetData(const std::string& name, const void* data, std::uint32_t size)
	{
		m_dataAccumulator.Append(data, size);
	}
	void DirectXShader::SetInt(const std::string& name, int value)
	{
		m_dataAccumulator.Append(&value, sizeof(value));
	}
	void DirectXShader::SetIntArray(const std::string& name, int* values, std::uint32_t count)
	{
		m_dataAccumulator.Append(values, sizeof(int) * count);
	}
	void DirectXShader::SetFloat(const std::string& name, float value)
	{
		m_dataAccumulator.Append(&value, sizeof(float));
	}
	void DirectXShader::SetFloat2(const std::string& name, XMFLOAT2 value)
	{
		m_dataAccumulator.Append(&value, sizeof(XMFLOAT2));
	}
	void DirectXShader::SetFloat3(const std::string& name, XMFLOAT3 value)
	{
		m_dataAccumulator.Append(&value, sizeof(XMFLOAT3));
	}
	void DirectXShader::SetFloat4(const std::string& name, XMFLOAT4 value)
	{
		m_dataAccumulator.Append(&value, sizeof(XMFLOAT4));
	}
	void DirectXShader::SetMat4(const std::string& name, XMMATRIX value)
	{
		XMFLOAT4X4 shaderMat = {};
		XMStoreFloat4x4(&shaderMat, value);
		m_dataAccumulator.Append(&shaderMat, sizeof(XMFLOAT4X4));
	}
}
