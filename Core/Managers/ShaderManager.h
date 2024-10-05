#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <memory>
#include <string>
#include <map>
#include <d3dcommon.h>

namespace BINDU
{
	class D3DShader;

	class ShaderManager
	{
	public:
		ShaderManager() = default;
		~ShaderManager() = default;

		// Creates and returns a pointer to that shader
		D3DShader*					CreateShader(const std::string& name, const std::string& fileName, const D3D_SHADER_MACRO* pDefines,
													const std::string& entryPoint, const std::string& target);

		// Adds a shader to the manager and returns a raw pointer to that shader
		D3DShader*					AddShader(const std::string& name, std::unique_ptr<D3DShader>&& shader);

		// Returns the shader specified by name from the shader pool
		D3DShader*					GetShader(const std::string& name);

		// Removes a shader from the manager and returns a pointer to the removed shader. If not caught the shader object will be destroyed.
		std::unique_ptr<D3DShader>  RemoveShader(const std::string& name);

	private:

		using ShaderMap = std::map<std::string, std::unique_ptr<D3DShader>>;

		ShaderMap			m_shaderPool;
	};
}

#endif