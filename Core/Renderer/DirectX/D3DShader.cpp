#include "D3DShader.h"

#include "D3DUtillity.h"

namespace BINDU
{
	void D3DShader::CompileFromFile(const std::string& fileName, const std::string& entryPoint, const std::string& target)
	{
		m_shaderBlob = D3DUtility::CompileShader(fileName, nullptr, entryPoint, target);
	}

	D3D12_SHADER_BYTECODE D3DShader::GetShaderByteCode() const
	{
		D3D12_SHADER_BYTECODE shaderByteCode = {};
		shaderByteCode.BytecodeLength = m_shaderBlob->GetBufferSize();
		shaderByteCode.pShaderBytecode = m_shaderBlob->GetBufferPointer();

		return shaderByteCode;
	}
}
