#ifndef D3DSHADER_H
#define D3DSHADER_H

#include <wrl/client.h>
#include <d3d12.h>
#include <string>

namespace BINDU
{
	using namespace Microsoft::WRL;

	class D3DShader
	{
	public:
		D3DShader() = default;
		~D3DShader() = default;

		void					CompileFromFile(const std::string& fileName, const std::string& entryPoint, const std::string& target);

		D3D12_SHADER_BYTECODE	GetShaderByteCode() const;

	private:
		ComPtr<ID3DBlob>	m_shaderBlob{ nullptr };
	};
}

#endif