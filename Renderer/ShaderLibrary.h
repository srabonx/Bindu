#ifndef BINDU_SHADER_LIBRARY_H
#define BINDU_SHADER_LIBRARY_H

#include <unordered_map>
#include "Shader.h"
#include "../Utility/Common/CommonUtility.h"
#include <string>

namespace BINDU
{
	class ShaderLibrary
	{
	public:
		ShaderLibrary() = default;
		~ShaderLibrary() = default;

		void		Add(const Ref<Shader>& shader);

		Ref<Shader> Get(const std::string& shaderName);

		bool		Has(const std::string& shaderName);

		void		Remove(const std::string& shaderName);

	private:
		std::unordered_map<std::string, Ref<Shader>>	m_shaderMap;
	};
}

#endif