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

	enum class RenderMode : std::uint8_t
	{
		SOLID,
		WIRE_FRAME
	};

	enum class CullMode : std::uint8_t
	{
		BACK,
		FRONT,
		NONE
	};

	enum class Primitive : std::uint8_t
	{
		POINT,
		LINE,
		TRIANGLE
	};

	enum class ShaderParameterType : std::uint8_t
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
		std::string			Name = std::string();

		ShaderParameterType Type;

		std::uint16_t		NumberOfElements{ 1 };		// For ShaderParameterType of TEXTURE and CONSTANTS

		std::uint16_t		BaseSlot{ 0 };
	};

	struct ShaderSpecification
	{
		std::string Name = std::string();

		std::vector<ShaderInputElement>	ShaderInputElements;

		std::vector<ShaderParameter>	ShaderParameters;		// Order from Frequent to less Frequent 

		std::string Filepath = std::string();

		RenderMode  RenderMode = RenderMode::SOLID;

		CullMode	CullMode = CullMode::BACK;

		Primitive	Primitive{ Primitive::TRIANGLE };

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

		virtual void BindUniformBuffer(std::string name, int offset, UniformBuffer* buffer) = 0;

		virtual void BindTexture(std::string name, Texture* texture) = 0;

		virtual void SetInt(const std::string& name, int value) = 0;

		virtual void SetIntArray(const std::string& name, int* values, std::uint32_t count) = 0;

		virtual void SetFloat(const std::string& name, float value) = 0;

		virtual void SetFloat2(const std::string& name, XMFLOAT2 value) = 0;

		virtual void SetFloat3(const std::string& name, XMFLOAT3 value) = 0;

		virtual void SetFloat4(const std::string& name, XMFLOAT4 value) = 0;

		virtual void SetMat4(const std::string& name, XMFLOAT4X4 value) = 0;

		virtual bool operator ==(const Shader& other) = 0;

	};
}

#endif