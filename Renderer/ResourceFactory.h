#ifndef BINDU_RESOURCE_FACTORY_H
#define BINDU_RESOURCE_FACTORY_H

#include "Renderer.h"
#include "UniformBuffer.h"


namespace BINDU
{
	class ResourceFactory
	{
	public:
		static void Initialize(const Ref<Renderer>& renderer);

		static Ref<Texture> CreateTexture(const std::string& path);

		static Ref<Texture> CreateDepthStencilTexture(const TextureSpecification& specification, bool isShaderVisible = false);

		static Ref<Texture> CreateRenderTexture(const Texture* depthStencilTexture,
			const TextureSpecification& specification, bool isShaderVisible = false);

		static Ref<Shader>	 CreateGraphicsShader(const ShaderSpecification& specification);

		static Ref<Shader>	 CreateComputeShader(const ShaderSpecification& specification);

		static Ref<VertexBuffer>	CreateVertexBuffer(const void* initData, std::uint32_t count, std::uint32_t byteStride);

		static Ref<IndexBuffer>	CreateIndexBuffer(const void* initData, std::uint32_t count, std::uint32_t byteStride, DataFormat dataFormat);

		static Ref<UniformBuffer>	CreateUniformBuffer(std::uint32_t elementCount, std::uint32_t elementByteSize);


	private:
		static WeakRef<Renderer>		m_renderer;
	};
}

#endif