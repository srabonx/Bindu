#include "ResourceFactory.h"

namespace BINDU
{
	WeakRef<Renderer> ResourceFactory::m_renderer;

	void ResourceFactory::Initialize(const Ref<Renderer>& renderer)
	{
		m_renderer = renderer;
	}

	Ref<Texture> ResourceFactory::CreateTexture(const std::string& path)
	{
		if(auto renderer = m_renderer.lock())
		{
			return renderer->CreateTexture(path);
		}

		return nullptr;
	}

	Ref<Texture> ResourceFactory::CreateDepthStencilTexture(const TextureSpecification& specification,
		bool isShaderVisible)
	{
		if (auto renderer = m_renderer.lock())
		{
			return renderer->CreateDepthStencilTexture(specification, isShaderVisible);
		}

		return nullptr;
	}

	Ref<Texture> ResourceFactory::CreateRenderTexture(const Texture* depthStencilTexture,
		const TextureSpecification& specification, bool isShaderVisible)
	{
		if (auto renderer = m_renderer.lock())
		{
			return renderer->CreateRenderTexture(depthStencilTexture, specification, isShaderVisible);
		}

		return nullptr;
	}

	Ref<Shader> ResourceFactory::CreateGraphicsShader(const ShaderSpecification& specification)
	{
		if (auto renderer = m_renderer.lock())
		{
			return renderer->CreateGraphicsShader(specification);
		}
		return nullptr;
	}

	Ref<Shader>	 ResourceFactory::CreateComputeShader(const ShaderSpecification& specification)
	{
		if (auto renderer = m_renderer.lock())
		{
			return renderer->CreateComputeShader(specification);
		}
		return nullptr;
	}


	Ref<VertexBuffer> ResourceFactory::CreateVertexBuffer(const void* initData, std::uint32_t count,
		std::uint32_t byteStride)
	{
		if (auto renderer = m_renderer.lock())
		{
			return renderer->CreateVertexBuffer(initData, count, byteStride);
		}
		return nullptr;
	}

	Ref<IndexBuffer> ResourceFactory::CreateIndexBuffer(const void* initData, std::uint32_t count,
		std::uint32_t byteStride, DataFormat dataFormat)
	{
		if (auto renderer = m_renderer.lock())
		{
			return renderer->CreateIndexBuffer(initData, count, byteStride, dataFormat);
		}
		return nullptr;
	}

	Ref<UniformBuffer> ResourceFactory::CreateUniformBuffer(std::uint32_t elementCount, std::uint32_t elementByteSize)
	{
		if (auto renderer = m_renderer.lock())
		{
			return renderer->CreateUniformBuffer(elementCount, elementByteSize);
		}
		return nullptr;
	}
}
