#ifndef BINDU_RENDERER_H
#define BINDU_RENDERER_H

#include <string>

#include "DataFormat.h"
#include "../Utility/Common/CommonUtility.h"


namespace BINDU
{
	struct ShaderSpecification;
	class Shader;
	struct TextureSpecification;
	class Texture;

	class VertexBuffer;
	class IndexBuffer;
	class UniformBuffer;


	class Renderer
	{
	public:
		virtual ~Renderer() = default;

		virtual void Initialize() = 0;

		virtual void Close() = 0;

		virtual Ref<Texture> CreateTexture(const std::string& path) = 0;

		virtual Ref<Texture> CreateDepthStencilTexture(const TextureSpecification& specification, bool isShaderVisible = false) = 0;

		virtual Ref<Texture> CreateRenderTexture(const Texture* depthStencilTexture,
			const TextureSpecification& specification, bool isShaderVisible = false) = 0;

		virtual Ref<Shader>	 CreateGraphicsShader(const ShaderSpecification& specification) = 0;

		virtual Ref<Shader>	 CreateComputeShader(const ShaderSpecification& specification) = 0;

		virtual Ref<VertexBuffer>	CreateVertexBuffer(const void* initData, std::uint32_t count, std::uint32_t byteStride) = 0;

		virtual Ref<IndexBuffer>	CreateIndexBuffer(const void* initData, std::uint32_t count, std::uint32_t byteStride, DataFormat dataFormat) = 0;

		virtual Ref<UniformBuffer>	CreateUniformBuffer(std::uint32_t elementCount, std::uint32_t elementByteSize) const = 0;
	};
}

#endif