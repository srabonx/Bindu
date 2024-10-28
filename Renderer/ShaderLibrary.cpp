#include "ShaderLibrary.h"


namespace BINDU
{
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& specification = shader->GetSpecification();
		auto name = specification.Name;

		if (name.empty())
			THROW_EXCEPTION(3, "Invalid shader name");

		m_shaderMap[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& shaderName)
	{
		if (Has(shaderName))
			return m_shaderMap[shaderName];

		return nullptr;
	}

	bool ShaderLibrary::Has(const std::string& shaderName)
	{
		return m_shaderMap.find(shaderName) != m_shaderMap.end();
	}

	void ShaderLibrary::Remove(const std::string& shaderName)
	{
		m_shaderMap.erase(shaderName);
	}
}
