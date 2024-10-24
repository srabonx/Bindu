#include "ShaderManager.h"

#include "../Renderer/DirectX/D3DShader.h"

namespace BINDU
{
	D3DShader* ShaderManager::CreateShader(const std::string& name, const std::string& fileName,
		const D3D_SHADER_MACRO* pDefines, const std::string& entryPoint, const std::string& target)
	{
		auto shader = std::make_unique<D3DShader>();

		shader->CompileFromFile(fileName, pDefines, entryPoint, target);

		return m_shaderPool.emplace(name, std::move(shader)).first->second.get();
	}

	D3DShader* ShaderManager::AddShader(const std::string& name, std::unique_ptr<D3DShader>&& shader)
	{
		return m_shaderPool.emplace(name, std::move(shader)).first->second.get();
	}

	D3DShader* ShaderManager::GetShader(const std::string& name)
	{
		return m_shaderPool[name].get();
	}

	std::unique_ptr<D3DShader> ShaderManager::RemoveShader(const std::string& name)
	{
		auto shader = std::move(m_shaderPool[name]);
		m_shaderPool.erase(name);

		return std::move(shader);
	}
}
