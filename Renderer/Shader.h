#ifndef BINDU_SHADER_H
#define BINDU_SHADER_H

#include <string>
#include <DirectXMath.h>

#include "DataFormat.h"

namespace BINDU
{
	class Texture;
	using namespace DirectX;

	class UniformBuffer;

	enum class ShaderParameterType
	{
		TEXTURE,
		UNIFORM_BUFFER,
		CONSTANTS
	};

	struct ShaderInputElement
	{
		std::string SemanticName = std::string();

		DataFormat	Format;

		std::uint16_t DataByteSize;
	};

	struct ShaderParameter
	{
		ShaderParameterType Type;

		std::uint16_t		NumberOfElements{ 1 };

		std::uint16_t		BaseSlot{ 0 };
	};

	struct ShaderSpecification
	{
		std::vector<ShaderInputElement>	ShaderInputElements;

		std::vector<ShaderParameter>	ShaderParameters;		// Order from Frequent to less Frequent 

		std::string Name = std::string();

		std::string Filepath = std::string();

		std::string VertexShader = std::string();
		std::string PixelShader = std::string();
		std::string GeometryShader = std::string();
		std::string ComputeShader = std::string();
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() = 0;

		virtual const ShaderSpecification& GetSpecification() const = 0;

		virtual void BindUniformBuffer(int slot, int offset, UniformBuffer* buffer) = 0;

		virtual void BindTexture(int slot, Texture* texture) = 0;

		virtual void SetInt(const std::string& name, int slot, int value) = 0;

		virtual void SetIntArray(const std::string& name, int baseSlot, int* values, std::uint32_t count) = 0;

		virtual void SetFloat(const std::string& name, int slot, float value) = 0;

		virtual void SetFloat2(const std::string& name, int slot, XMFLOAT2 value) = 0;

		virtual void SetFloat3(const std::string& name, int slot, XMFLOAT3 value) = 0;

		virtual void SetFloat4(const std::string& name, int slot, XMFLOAT4 value) = 0;

		virtual void SetMat4(const std::string& name, int slot, XMFLOAT4X4 value) = 0;

		virtual bool operator ==(const Shader& other) = 0;

	};
}

#endif