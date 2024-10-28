#include "DirectXGraphicsShader.h"

#include "DirectXConstantBuffer.h"
#include "DirectXTexture.h"

namespace BINDU
{
	DirectXGraphicsShader::DirectXGraphicsShader(const ShaderSpecification& specification, const ComPtr<ID3D12GraphicsCommandList>& cmdList) :
			m_specification(specification), m_commandList(cmdList)
	{
	}

	void DirectXGraphicsShader::Bind()
	{
		if (m_graphicsPSO)
		{
			m_commandList->SetPipelineState(m_graphicsPSO.Get());
			m_commandList->SetGraphicsRootSignature(m_rootSig.Get());
		}
	}

	const ShaderSpecification& DirectXGraphicsShader::GetSpecification() const
	{
		return m_specification;
	}

	void DirectXGraphicsShader::BindUniformBuffer(std::string name, int offset, UniformBuffer* buffer)
	{
		auto rootParamSlot = m_shaderParameterMap[name].second;
		auto* directXConstantBuffer = static_cast<DirectXConstantBuffer*>(buffer);
		m_commandList->SetGraphicsRootConstantBufferView(rootParamSlot, directXConstantBuffer->GetGPUVirtualAddressAt(offset));
	}

	void DirectXGraphicsShader::BindTexture(std::string name, Texture* texture)
	{
		auto rootParamSlot = m_shaderParameterMap[name].second;
		texture->Bind(rootParamSlot);
	}

	void DirectXGraphicsShader::SetInt(const std::string& name, int value)
	{
		auto rootParamIndex = m_shaderParameterMap[name].second;
		m_commandList->SetGraphicsRoot32BitConstant(rootParamIndex, value, 0);
	}

	void DirectXGraphicsShader::SetIntArray(const std::string& name, int* values, std::uint32_t count)
	{
		auto rootParamIndex = m_shaderParameterMap[name].second;
		m_commandList->SetGraphicsRoot32BitConstants(rootParamIndex, count, values, 0);
	}

	void DirectXGraphicsShader::SetFloat(const std::string& name, float value)
	{
		auto rootParamIndex = m_shaderParameterMap[name].second;
		UINT castedData = *reinterpret_cast<UINT*>(&value);
		m_commandList->SetGraphicsRoot32BitConstant(rootParamIndex, castedData, 0);
	}

	void DirectXGraphicsShader::SetFloat2(const std::string& name, XMFLOAT2 value)
	{
		auto rootParamIndex = m_shaderParameterMap[name].second;
		m_commandList->SetGraphicsRoot32BitConstants(rootParamIndex, 2, &value, 0);
	}

	void DirectXGraphicsShader::SetFloat3(const std::string& name, XMFLOAT3 value)
	{
		auto rootParamIndex = m_shaderParameterMap[name].second;
		m_commandList->SetGraphicsRoot32BitConstants(rootParamIndex, 3, &value, 0);
	}

	void DirectXGraphicsShader::SetFloat4(const std::string& name, XMFLOAT4 value)
	{
		auto rootParamIndex = m_shaderParameterMap[name].second;
		m_commandList->SetGraphicsRoot32BitConstants(rootParamIndex, 4, &value, 0);
	}

	void DirectXGraphicsShader::SetMat4(const std::string& name, XMFLOAT4X4 value)
	{
		auto rootParamIndex = m_shaderParameterMap[name].second;
		m_commandList->SetGraphicsRoot32BitConstants(rootParamIndex, 16, &value, 0);
	}

	bool DirectXGraphicsShader::operator==(const Shader& other)
	{
		return this == &other;
	}
}
