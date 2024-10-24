#include "DirectXShader.h"

#include "DirectXConstantBuffer.h"
#include "DirectXTexture.h"

namespace BINDU
{
	DirectXShader::DirectXShader(const ShaderSpecification& specification, const ComPtr<ID3D12GraphicsCommandList>& cmdList) :
			m_specification(specification), m_commandList(cmdList)
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

	void DirectXShader::BindUniformBuffer(int slot, int offset, UniformBuffer* buffer)
	{
		auto rootParamSlot = m_cbSlotToRootParam[slot];
		auto* directXConstantBuffer = static_cast<DirectXConstantBuffer*>(buffer);
		m_commandList->SetGraphicsRootConstantBufferView(rootParamSlot, directXConstantBuffer->GetGPUVirtualAddressAt(offset));
	}

	void DirectXShader::BindTexture(int slot, Texture* texture)
	{
		auto rootParamSlot = m_texSlotToRootParam[slot];
		texture->Bind(rootParamSlot);
	}

	void DirectXShader::SetInt(const std::string& name, int slot, int value)
	{
		auto rootParamIndex = m_constantsSlotToRootParam[slot];
		m_commandList->SetGraphicsRoot32BitConstant(rootParamIndex, value, 0);
	}

	void DirectXShader::SetIntArray(const std::string& name, int slot, int* values, std::uint32_t count)
	{
		auto rootParamIndex = m_constantsSlotToRootParam[slot];
		m_commandList->SetGraphicsRoot32BitConstants(rootParamIndex, count, values, 0);
	}

	void DirectXShader::SetFloat(const std::string& name, int slot, float value)
	{
		auto rootParamIndex = m_constantsSlotToRootParam[slot];
		UINT castedData = *reinterpret_cast<UINT*>(&value);
		m_commandList->SetGraphicsRoot32BitConstant(rootParamIndex, castedData, 0);
	}

	void DirectXShader::SetFloat2(const std::string& name, int slot, XMFLOAT2 value)
	{
		auto rootParamIndex = m_constantsSlotToRootParam[slot];
		m_commandList->SetGraphicsRoot32BitConstants(rootParamIndex, 2, &value, 0);
	}

	void DirectXShader::SetFloat3(const std::string& name, int slot, XMFLOAT3 value)
	{
		auto rootParamIndex = m_constantsSlotToRootParam[slot];
		m_commandList->SetGraphicsRoot32BitConstants(rootParamIndex, 3, &value, 0);
	}

	void DirectXShader::SetFloat4(const std::string& name, int slot, XMFLOAT4 value)
	{
		auto rootParamIndex = m_constantsSlotToRootParam[slot];
		m_commandList->SetGraphicsRoot32BitConstants(rootParamIndex, 4, &value, 0);
	}

	void DirectXShader::SetMat4(const std::string& name, int slot, XMFLOAT4X4 value)
	{
		auto rootParamIndex = m_constantsSlotToRootParam[slot];
		m_commandList->SetGraphicsRoot32BitConstants(rootParamIndex, 16, &value, 0);
	}

	bool DirectXShader::operator==(const Shader& other)
	{
		return this == &other;
	}
}
