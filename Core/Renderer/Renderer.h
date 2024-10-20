#ifndef BINDU_RENDERER_H
#define BINDU_RENDERER_H

#include <string>
#include "../../Utility/Common/CommonUtility.h"


namespace BINDU
{
	struct ShaderSpecification;
	class Shader;
	struct TextureSpecification;
	class Texture;


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

		virtual Ref<Shader>	 CreateShader(const ShaderSpecification& specification) = 0;
	};
}

#endif